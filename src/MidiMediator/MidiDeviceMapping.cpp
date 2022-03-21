#include "pch.hpp"
#include "MidiDeviceMapping.hpp"

MidiDeviceMapping::MidiDeviceMapping(std::string const& name, std::string const& inputDeviceName, std::vector<std::string> const& outputDeviceNames, bool passthrough, int32_t multiMessageSendDelay, command_map_t const& commandMaps) :
	m_name(name),
	m_inputDeviceName(inputDeviceName),
	m_outputDeviceNames(outputDeviceNames),
	m_passthrough(passthrough),
	m_multiMessageSendDelay(multiMessageSendDelay),
	m_commandMaps(commandMaps)
{
}

[[nodiscard]] std::string const& MidiDeviceMapping::name() const
{
	return m_name;
}

[[nodiscard]] std::string const& MidiDeviceMapping::inputDeviceName() const
{
	return m_inputDeviceName;
}

[[nodiscard]] std::vector< std::string > const& MidiDeviceMapping::outputDeviceNames() const
{
	return m_outputDeviceNames;
}

[[nodiscard]] bool MidiDeviceMapping::passthrough() const
{
	return m_passthrough;
}

[[nodiscard]] MidiDeviceMapping::command_map_t const& MidiDeviceMapping::commandMaps() const
{
	return m_commandMaps;
}
