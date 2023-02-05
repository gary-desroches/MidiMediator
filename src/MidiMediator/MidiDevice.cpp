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
#include "MidiDevice.hpp"

#include <sstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <cctype>

MidiDevice::MidiDevice(std::string const& deviceName, std::string const& configName) :
    m_rtMidi(nullptr),
    m_deviceName(deviceName),
	m_configName(configName)
{
}

std::string trimNumericSuffix(std::string const& deviceName)
{
	auto it = deviceName.end() - 1;
	while (it > deviceName.begin() && *it != ' ')
	{
		--it;
	}

	auto trimmed(std::string(deviceName.begin(), it));
	boost::trim_right(trimmed);

	return trimmed;
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

std::string trimNumericSuffix(std::string const& deviceName);

std::string MidiDevice::uniqueDeviceName() const
{
	return trimNumericSuffix(m_deviceName);
}

bool MidiDevice::compareName(std::string const& comparison, bool matchUnique)
{
	if (matchUnique)
	{
		return boost::iequals(m_deviceName, comparison);
	}
	else
	{
		return boost::iequals(trimNumericSuffix(m_deviceName), comparison);
	}
}
