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
	void sendMessageToDevices(MidiDeviceMapping const& deviceMap, std::vector<uint8_t> const& messageBytes);
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