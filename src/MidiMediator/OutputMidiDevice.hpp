#pragma once

#include "MidiDevice.hpp"
#include "OutputMidiPort.hpp"

#include <memory>

class OutputMidiDevice : public MidiDevice
{
public:
    OutputMidiDevice(RtMidi::Api api, std::string const& rtMidiDeviceName, std::string const& deviceName, std::string const& configName);

public:
    OutputMidiDevice(OutputMidiDevice const& source) = delete;
    OutputMidiDevice& operator=(OutputMidiDevice const& source) = delete;

public:
    OutputMidiDevice(OutputMidiDevice&& source) = default;
    OutputMidiDevice& operator=(OutputMidiDevice&& source) = default;

public:
    OutputMidiPort openPort();

private:
    std::unique_ptr<RtMidiOut> m_rtMidiOut;
};
