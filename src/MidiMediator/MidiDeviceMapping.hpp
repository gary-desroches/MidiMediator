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
