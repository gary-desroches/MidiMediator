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

#pragma once

#include "MidiDeviceMapping.hpp"
#include "json11.hpp"
#include "CommandMap.hpp"

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
    bool matchUniqueDeviceNames() const;

private:
    void initialize();
    json11::Json readJsonFromFile();
    MidiDeviceMapping::command_map_t parseCommandMaps(json11::Json const& deviceMap);
    void parseDeviceMap(json11::Json const& deviceMap);

private:
    std::string const m_configPath;
    bool m_initialized;
    uint32_t m_queueSizeLimit;
    bool m_matchUniqueDeviceNames;
    std::vector<MidiDeviceMapping> m_deviceMaps;

    static std::string const InputFieldName;
    static std::string const OutputsFieldName;
};