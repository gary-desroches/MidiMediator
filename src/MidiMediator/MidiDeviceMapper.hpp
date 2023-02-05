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
#include "InputMidiDevice.hpp"
#include "OutputMidiDevice.hpp"
#include "MessageDirection.hpp"

class MidiDeviceMapper
{
public:
	MidiDeviceMapper(RtMidi::Api api, uint32_t queueSizeLimit, std::vector<MidiDeviceMapping> const& deviceMaps, bool matchUniqueDeviceNames);

public:
	void listen();

private:
	void initialize();

private:
	void findDevices();
	void openPorts();
	void logMessage(std::string const& deviceName, MessageDirection direction, MidiMessage const& message);
	void sendMessagesToDevices(MidiDeviceMapping const& deviceMap, std::vector<uint8_t> const& messageBytes);
	void onIncomingMessage(InputMidiPort& inputMidiPort, double const timeStamp, std::vector<uint8_t> const& messageBytes);

private:
	std::vector<MidiDeviceMapping> const& m_deviceMaps;
	RtMidi::Api m_api;
	uint32_t m_queueSizeLimit;
	bool m_matchUniqueDeviceNames;

	std::vector<InputMidiDevice> m_inputDevices;
	std::vector<OutputMidiDevice> m_outputDevices;

	std::vector<InputMidiPort> m_inputPorts;
	std::vector<OutputMidiPort> m_outputPorts;

	bool m_initialized;
};