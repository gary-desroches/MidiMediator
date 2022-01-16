#include "pch.hpp"
#include "InputMidiPort.hpp"

InputMidiPort::InputMidiPort(RtMidiIn& midi, std::string const& name, uint8_t const port) :
    MidiPort(midi, name, port),
	m_callback(nullptr)
{
}

InputMidiPort::InputMidiPort(InputMidiPort&& source) noexcept :
    MidiPort(std::move(source)),
	m_callback(std::move(source.m_callback))
{
}

/*
InputMidiPort& InputMidiPort::operator=(InputMidiPort&& source) noexcept
{
	m_callback = source.m_callback;
    return static_cast<InputMidiPort&>(MidiPort::operator=(std::move(source)));
}
*/

void InputMidiPort::open(callback_t callback)
{
	try
	{
		if (!isOpen())
		{
			m_callback = callback;
			static_cast<RtMidiIn&>(midi()).setCallback(
				&InputMidiPort::onIncomingMessage,
				this
			);
		}
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not open port \"" << deviceName() << "\" port " << port() << ".\n";
		throw;
	}

	MidiPort::open();
}

void InputMidiPort::onIncomingMessage(double timeStamp, std::vector<uint8_t>* messageBytes, void* userData)
{
	auto inputMidiPort = static_cast<InputMidiPort*>(userData);
	inputMidiPort->onIncomingMessage(timeStamp, *messageBytes);
}

void InputMidiPort::onIncomingMessage(double const timeStamp, std::vector<uint8_t> const& messageBytes)
{
	m_callback(*this, timeStamp, messageBytes);
}
