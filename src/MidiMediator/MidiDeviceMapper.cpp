#include "pch.hpp"
#include "MidiDeviceMapper.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <thread>

MidiDeviceMapper::MidiDeviceMapper(RtMidi::Api api, uint32_t queueSizeLimit, std::vector<MidiDeviceMapping> const& deviceMaps) :
	m_api(api),
	m_queueSizeLimit(queueSizeLimit),
	m_deviceMaps(deviceMaps),
	m_initialized(false)
{
}

void MidiDeviceMapper::listen()
{
	initialize();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void MidiDeviceMapper::findDevices()
{
	try
	{
		RtMidiIn mainInputDevice(m_api, "Main Input", m_queueSizeLimit);
		size_t inputPortCount = mainInputDevice.getPortCount();
		std::cout << "Input port count: " << inputPortCount << '\n';
		if (inputPortCount == 0)
		{
			std::stringstream errorMessage;
			errorMessage << __PRETTY_FUNCTION__ << ": No input ports to listen on.\n";
			throw std::logic_error(errorMessage.str());
		}

		for (unsigned int port = 0; port < inputPortCount; port += 1)
		{
			std::string portName = mainInputDevice.getPortName(port);
			std::cout << "Port " << port << ": " << portName << '\n';
			std::stringstream configName;
			configName << portName << ": Input " << port;
			m_inputDevices.push_back(
				InputMidiDevice(
					m_api,
					portName,
					m_queueSizeLimit,
					portName,
					configName.str()
				)
			);
		}
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not find input devices.\n";
		throw;
	}

	if (m_inputDevices.size() == 0)
	{
		std::stringstream errorMessage;
		errorMessage << __PRETTY_FUNCTION__ << ": No input devices listed in device mappings found.\n";
		throw std::logic_error(errorMessage.str());
	}

	try
	{
		RtMidiOut mainOutputDevice(m_api, "Main Output");
		size_t outputPortCount = mainOutputDevice.getPortCount();
		std::cout << "Output port count: " << outputPortCount << '\n';
		for (unsigned int port = 0; port < outputPortCount; port += 1)
		{
			std::string portName = mainOutputDevice.getPortName(port);
			std::cout << "Port " << port << ": " << portName << '\n';
			std::stringstream configName;
			configName << portName << "Output " << port;
			m_outputDevices.push_back(
				OutputMidiDevice(
					m_api,
					portName,
					portName,
					configName.str()
				)
			);
		}
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not find output devices.\n";
		throw;
	}

	if (m_outputDevices.size() == 0)
	{
		std::stringstream errorMessage;
		errorMessage << __PRETTY_FUNCTION__ << ": No output devices listed in device mappings found.\n";
		throw std::logic_error(errorMessage.str());
	}
}

void MidiDeviceMapper::openPorts()
{
	try
	{
		std::cout << "Opening output ports..." << '\n';
		for (auto&& outputDevice : m_outputDevices)
		{
			m_outputPorts.push_back(
				outputDevice.openPort()
			);

			m_outputPorts.back().open();
		}
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not create input port objects.\n";
		throw;
	}

	try
	{
		std::cout << "Opening input ports..." << '\n';
		for (auto&& inputDevice : m_inputDevices)
		{
			m_inputPorts.push_back(inputDevice.openPort());
			m_inputPorts.back().open(
				std::bind(
					&MidiDeviceMapper::onIncomingMessage, this, 
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
				)
			);
		}
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not create output port objects.\n";
		throw;
	}
	std::cout << "Now listening for messages..." << '\n';
}

void MidiDeviceMapper::initialize()
{
	if (m_initialized)
	{
		return;
	}

	try
	{
		findDevices();
		openPorts();
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not initialize the MIDI Device Mapper.\n";
		throw;
	}

	m_initialized = true;
}

void MidiDeviceMapper::logMessage(std::string const& deviceName, MessageDirection direction, MidiMessage const& message)
{
	std::cout << std::string("Message ") << (direction == MessageDirection::Sent ? "sent to " : "received from ") << deviceName << " : Code " << static_cast<uint32_t>(message.code()) << std::string(", Data[");
	auto isFirst = true;
	for (auto&& b : message.data())
	{
		if (isFirst)
		{
			isFirst = false;
		}
		else
		{
			std::cout << ",";
		}
		std::cout << static_cast<uint32_t>(b);
	}
	std::cout << std::string("]") << std::endl;
}

void MidiDeviceMapper::sendMessageToDevices(MidiDeviceMapping const& deviceMap, std::vector<uint8_t> const& messageBytes)
{
	MidiMessage message(messageBytes);
	for (auto&& outputDeviceName : deviceMap.outputDeviceNames())
	{
		try
		{
			for (auto&& outputPort : m_outputPorts)
			{
				if (boost::iequals(outputPort.deviceName(), outputDeviceName))
				{
					logMessage(outputPort.deviceName(), MessageDirection::Sent, message);
					outputPort.send(message);
				}
			}
		}
		catch (RtMidiError& error)
		{
			std::cout << __PRETTY_FUNCTION__ << ": Unhandled RtMidiError.\n";
			error.printMessage();
		}
		catch (std::exception& error)
		{
			std::cout << __PRETTY_FUNCTION__ << ": Unhandled std::exception: " << error.what() << ".\n";
		}
		catch (...)
		{
			std::cout << __PRETTY_FUNCTION__ << ": Could not send message to device \"" << outputDeviceName << "\".\n";
		}
	}
}

void MidiDeviceMapper::onIncomingMessage(InputMidiPort& inputMidiPort, double const /*timeStamp*/, std::vector<uint8_t> const& messageBytes)
{
	try
	{
		MidiMessage message(messageBytes);
		logMessage(inputMidiPort.deviceName(), MessageDirection::Received, message);
		for (auto&& deviceMap : m_deviceMaps)
		{
			if (boost::iequals(deviceMap.inputDeviceName(), inputMidiPort.deviceName()))
			{
				if (deviceMap.passthrough())
				{
					std::cout << "Passing through...\n";
					sendMessageToDevices(deviceMap, messageBytes);
				}
				else
				{
					for (auto&& commandMap : deviceMap.commandMaps())
					{
						if (std::equal(commandMap.first.begin(), commandMap.first.end(), messageBytes.begin()))
						{
							sendMessageToDevices(deviceMap, commandMap.second);
						}
					}
				}
			}
		}
	}
	catch (RtMidiError& error)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled RtMidiError.\n";
		error.printMessage();
	}
	catch (std::exception& error)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled std::exception: " << error.what() << ".\n";
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not process incoming message.\n";
	}
}
