#include "pch.hpp"
#include "MidiDevice.hpp"

#include <sstream>
#include <boost/algorithm/string/predicate.hpp>

MidiDevice::MidiDevice(std::string const& deviceName, std::string const& configName) :
    m_rtMidi(nullptr),
    m_deviceName(deviceName),
	m_configName(configName)
{
}

[[nodiscard]] uint8_t MidiDevice::findPortNumber()
{
	for (size_t port = 0; port < m_rtMidi->getPortCount(); ++port)
	{
		std::string const currentPortName = m_rtMidi->getPortName(static_cast<unsigned int>(port));
		if (boost::iequals(currentPortName, m_deviceName))
		{
			return static_cast<uint8_t>(port);
		}
	}

	std::stringstream errorMessageBuilder;
	errorMessageBuilder << __PRETTY_FUNCTION__ << ": " << m_configName << " port " << m_deviceName << " could not be found.";
	std::string const errorMessage = errorMessageBuilder.str();
	std::cout << errorMessage << std::endl;
	throw std::logic_error(errorMessage);
}

RtMidi& MidiDevice::midi()
{
	return *m_rtMidi;
}

RtMidi& MidiDevice::midi() const
{
	return *m_rtMidi;
}

std::string const& MidiDevice::deviceName() const
{
	return m_deviceName;
}

std::string const& MidiDevice::configName() const
{
	return m_configName;
}

MidiDevice::MidiDevice(MidiDevice&& source) noexcept :
	m_rtMidi(source.m_rtMidi),
	m_deviceName(std::move(source.m_deviceName)),
	m_configName(std::move(source.m_configName))
{
}

MidiDevice& MidiDevice::operator=(MidiDevice&& source) noexcept
{
	m_rtMidi = source.m_rtMidi;
	m_deviceName = std::move(source.m_deviceName);
	m_configName = std::move(source.m_configName);
	return *this;
}

void MidiDevice::setMidiPtr(RtMidi* rtMidi)
{
	m_rtMidi = rtMidi;
}