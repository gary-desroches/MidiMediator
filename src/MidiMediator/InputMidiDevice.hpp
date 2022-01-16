#pragma once

#include "MidiDevice.hpp"
#include "InputMidiPort.hpp"
#include "OutputMidiPort.hpp"

#include <functional>
#include <memory>

class InputMidiDevice : public MidiDevice
{
public:
    InputMidiDevice(RtMidi::Api api, std::string const& rtMidiDeviceName, uint32_t queueSizeLimit, std::string const& deviceName, std::string const& configName);

public:
    InputMidiDevice(InputMidiDevice const& source) = delete;
    InputMidiDevice& operator=(InputMidiDevice const& source) = delete;

public:
    InputMidiDevice(InputMidiDevice&& source) = default;
    InputMidiDevice& operator=(InputMidiDevice&& source) = default;

public:
    InputMidiPort openPort();

private:
    std::unique_ptr<RtMidiIn> m_rtMidiIn;
};
