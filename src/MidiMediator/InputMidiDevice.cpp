#include "pch.hpp"
#include "InputMidiDevice.hpp"

InputMidiDevice::InputMidiDevice(RtMidi::Api api, std::string const& rtMidiDeviceName, uint32_t queueSizeLimit, const std::string& deviceName, const std::string& configName) :
	MidiDevice(deviceName, configName),
	m_rtMidiIn(new RtMidiIn(api, rtMidiDeviceName, queueSizeLimit))
{
	setMidiPtr(m_rtMidiIn.get());
}

InputMidiPort InputMidiDevice::openPort()
{
	return InputMidiPort(static_cast<RtMidiIn&>(midi()), deviceName(), findPortNumber());
}
