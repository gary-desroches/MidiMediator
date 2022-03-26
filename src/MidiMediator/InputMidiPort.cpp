#include "pch.hpp"
#include "InputMidiPort.hpp"

#include <sstream>

InputMidiPort::InputMidiPort(RtMidiIn& midi, std::string const& name, uint8_t const port) :
    MidiPort(midi, name, port),
	m_callback(nullptr),
	m_init(false)
{
}

InputMidiPort::InputMidiPort(InputMidiPort&& source) noexcept :
    MidiPort(std::move(source)),
	m_callback(std::move(source.m_callback)),
	m_init(source.m_init)
{
	source.m_callback.reset(nullptr);
	if (m_callback)
	{
		static_cast<RtMidiIn&>(midi()).cancelCallback();
		static_cast<RtMidiIn&>(midi()).setCallback(
			&InputMidiPort::onIncomingMessage,
			this
		);
	}
}

void InputMidiPort::open(callback_t callback)
{
	try
	{
		if (!isOpen())
		{
			m_callback.reset(new callback_t(callback));
			if (!m_callback)
			{
				std::stringstream errorMessage;
				errorMessage << __PRETTY_FUNCTION__ << ": m_callback ptr is null";
				throw std::logic_error(errorMessage.str());
			}
			m_init = true;
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
	if (!m_callback)
	{
		std::stringstream errorMessage;
		errorMessage << __PRETTY_FUNCTION__ << ": m_callback ptr is null";
		throw std::logic_error(errorMessage.str());
	}

	(*m_callback)(*this, timeStamp, messageBytes);
}
