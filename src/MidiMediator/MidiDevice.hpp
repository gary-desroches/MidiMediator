#pragma once

#include <string>
#include "RtMidi.h"
#include "MidiPort.hpp"

class MidiDevice
{
public:
    MidiDevice() = delete;

public:
    MidiDevice(MidiDevice const& source) = delete;
    MidiDevice& operator=(MidiDevice const& source) = delete;

public:
    MidiDevice(MidiDevice&& source) noexcept;
    MidiDevice& operator=(MidiDevice&& source) noexcept;

protected:
    // Always call setMidiPtr() in subclasses
    MidiDevice(std::string const& deviceName, std::string const& configName);

public:
    [[nodiscard]] uint8_t findPortNumber();

protected:
    RtMidi& midi();
    RtMidi& midi() const;
    void setMidiPtr(RtMidi* rtMidi);
    std::string const& deviceName() const;
    std::string uniqueDeviceName() const;
    std::string const& configName() const;
    bool compareName(std::string const& comparison, bool matchUnique);

private:
    RtMidi* m_rtMidi;
    std::string m_deviceName;
    std::string m_configName;
};
