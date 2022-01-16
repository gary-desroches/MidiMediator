#include "pch.hpp"
#include "Application.hpp"
#include "Configuration.hpp"
#include "MidiDeviceMapper.hpp"

#include <boost/algorithm/string.hpp>

#if defined(WIN32)
	RtMidi::Api const Application::Api = RtMidi::WINDOWS_MM;
	std::string const Application::ApiName = "Windows MM";
	std::string const Application::ConfigPath = "MidiMediator.json";
#elif defined(TARGET_OS_MAC)
	RtMidi::Api const Application::Api = RtMidi::MACOSX_CORE;
	std::string const Application::ApiName = "MacOSX Core";
	std::string const Application::ConfigPath = "/etc/MidiMediator.json";
#else
	RtMidi::Api const Application::Api = RtMidi::LINUX_ALSA;
	std::string const Application::ApiName = "Linux ALSA";
	std::string const Application::ConfigPath = "/etc/MidiMediator.json";
#endif

Application::Application(int argc, char* argv[]) :
	m_config(ConfigPath)
{
	m_arguments.reserve(argc - 1);
	for (int32_t index = 1; index < argc; index += 1)
	{
		m_arguments.push_back(std::string(argv[index]));
	}
}

void Application::listPorts()
{
	try
	{
		RtMidiIn mainInputDevice(Api, "Main Input", m_config.queueSizeLimit());
		size_t inputPortCount = mainInputDevice.getPortCount();
		std::cout << "Input port count: " << inputPortCount << '\n';
		for (unsigned int port = 0; port < inputPortCount; port += 1)
		{
			std::string portName = mainInputDevice.getPortName(port);
			std::cout << "Port " << port << ": " << portName << '\n';
		}
	}
	catch (...)
	{
		std::cout << "Could not list input ports.\n";
		throw;
	}

	try
	{
		RtMidiOut mainOutputDevice(Api, "Main Output");
		size_t outputPortCount = mainOutputDevice.getPortCount();
		std::cout << "Output port count: " << outputPortCount << '\n';
		for (unsigned int port = 0; port < outputPortCount; port += 1)
		{
			std::string portName = mainOutputDevice.getPortName(port);
			std::cout << "Port " << port << ": " << portName << '\n';
		}
	}
	catch (...)
	{
		std::cout << "Could not list output ports.\n";
		throw;
	}
}

int Application::run()
{
	std::cout << "Using " << ApiName << " as the MIDI API." << std::endl;

	try
	{
		if (m_arguments.size() > 0)
		{
			if (boost::iequals(m_arguments[0], "--list"))
			{
				listPorts();
				return 0;
			}
		}

		MidiDeviceMapper mapper(Api, m_config.queueSizeLimit(), m_config.deviceMaps());
		mapper.listen();
	}
	catch (RtMidiError& error)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled RtMidi Exception\n";
		error.printMessage();

		return -1;
	}
	catch (std::exception& error)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled std::exception\n" << error.what() << '\n';
		return -1;
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled unknown exception\n";
		return -1;
	}

	return 0;
}
