#pragma once

#include <string>
#include <vector>
#include <utility>

class MidiDeviceMapping
{
public: 
	typedef std::vector<
		std::pair<
		std::vector<uint8_t>,
		std::vector<uint8_t>
		>
	> command_map_t;

public:
	MidiDeviceMapping(std::string const& name, std::string const& inputDeviceName, std::vector<std::string> const& outputDeviceNames, bool passthrough, command_map_t const& commandMaps);

public:
	std::string const& name() const;
	std::string const& inputDeviceName() const;
	std::vector< std::string > const& outputDeviceNames() const;
	bool passthrough() const;
	command_map_t const& commandMaps() const;

private:
	std::string m_name;
	std::string m_inputDeviceName;
	std::vector<std::string> m_outputDeviceNames;
	bool m_passthrough;
	command_map_t m_commandMaps;
};
