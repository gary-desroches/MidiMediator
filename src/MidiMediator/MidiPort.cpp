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
		return boost::iequals(trimNumericSuffix(m_name), comparison);
	}
}
