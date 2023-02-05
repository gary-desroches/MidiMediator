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
#include "MidiPort.hpp"

#include <boost/algorithm/string.hpp>

MidiPort::MidiPort(RtMidi& midi, std::string const& name, uint8_t const port) :
	m_midi(midi),
	m_name(name),
	m_port(port),
	m_isOpen(false)
{
}

MidiPort::MidiPort(MidiPort&& source) noexcept :
	m_midi(source.m_midi),
	m_name(std::move(source.m_name)),
	m_port(source.m_port),
	m_isOpen(source.m_isOpen)
{
}

/*
MidiPort& MidiPort::operator=(MidiPort&& source) noexcept
{
	m_midi = source.m_midi;
	m_name = std::move(source.m_name);
	m_port = source.m_port;
	m_isOpen = source.m_isOpen;

	return MidiPort(std::move(source));
}
*/

void MidiPort::open()
{
	if (m_isOpen)
	{
		return;
	}

	try
	{
		m_midi.openPort(m_port);
	}
	catch (...)
	{
		std::cout << "Could not open " << m_name << " port.\n";
		throw;
	}

	m_isOpen = true;
}

RtMidi& MidiPort::midi()
{
	return m_midi;
}

bool MidiPort::isOpen() const
{
	return m_isOpen;
}

std::string const& MidiPort::deviceName() const
{
	return m_name;
}

uint8_t MidiPort::port() const
{
	return m_port;
}

std::string trimNumericSuffix(std::string const& deviceName);

std::string MidiPort::uniqueDeviceName() const
{
	return trimNumericSuffix(m_name);
}

bool MidiPort::compareName(std::string const& comparison, bool matchUnique)
{
	if (matchUnique)
	{
		return boost::iequals(m_name, comparison);
	}
	else
	{
		auto trimmedName = trimNumericSuffix(m_name);
		return boost::iequals(trimmedName, comparison);
	}
}
