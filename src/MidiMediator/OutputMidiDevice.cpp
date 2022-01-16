#include "pch.hpp"
#include "OutputMidiDevice.hpp"

OutputMidiDevice::OutputMidiDevice(RtMidi::Api api, std::string const& rtMidiDeviceName, std::string const& deviceName, std::string const& configName) :
	MidiDevice(deviceName, configName),
	m_rtMidiOut(new RtMidiOut(api, rtMidiDeviceName))
{
	setMidiPtr(m_rtMidiOut.get());
}

OutputMidiPort OutputMidiDevice::openPort()
{
	return OutputMidiPort(static_cast<RtMidiOut&>(midi()), deviceName(), findPortNumber());
}
