#pragma once

#include "CommandMap.hpp"
#include "RevolvingCollection.hpp"

#include <string>
#include <vector>
#include <utility>

class MidiDeviceMapping
{
public: 
	typedef std::vector<CommandMap> command_map_t;

public:
	MidiDeviceMapping(std::string const& name, std::string const& inputDeviceName, std::vector<std::string> const& outputDeviceNames, bool passthrough, int32_t multiMessageSendDelay, command_map_t const& commandMaps);

public:
	std::string const& name() const;
	std::string const& inputDeviceName() const;
	std::vector< std::string > const& outputDeviceNames() const;
	bool passthrough() const;
	int32_t multiMessageSendDelay() const;
	command_map_t const& commandMaps() const;

private:
	std::string m_name;
	std::string m_inputDeviceName;
	std::vector<std::string> m_outputDeviceNames;
	bool m_passthrough;
	int32_t m_multiMessageSendDelay;
	command_map_t m_commandMaps;
};
