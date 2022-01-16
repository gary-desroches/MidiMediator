#pragma once

#include "MidiDeviceMapping.hpp"
#include "json11.hpp"

#include <string>

class Configuration
{
public:
    Configuration() = delete;
    Configuration(std::string const& iniPath);

public:
    Configuration(Configuration const& source) = delete;
    Configuration& operator=(Configuration const& source) = delete;

public:
    Configuration(Configuration&& source) = delete;
    Configuration& operator=(Configuration&& source) = delete;

public:
    std::vector<MidiDeviceMapping> const& deviceMaps() const;
    uint32_t queueSizeLimit() const;

private:
    void initialize();
    json11::Json readJsonFromFile();
    MidiDeviceMapping::command_map_t parseCommandMaps(json11::Json const& deviceMap);
    void parseDeviceMap(json11::Json const& deviceMap);

private:
    std::string const m_configPath;
    bool m_initialized;
    uint32_t m_queueSizeLimit;
    std::vector<MidiDeviceMapping> m_deviceMaps;
};