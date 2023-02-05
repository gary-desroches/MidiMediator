/*

Copyright (c) 2023 Gary Des Roches

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

Any person wishing to distribute modifications to the Software is
asked to send the modifications to the original developer so that
they can be incorporated into the canonical version.  This is,
however, not a binding provision of this license.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

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
