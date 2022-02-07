#pragma once

#include "RtMidi.h"

class MidiPort
{
public:
    MidiPort(RtMidi& rtMidi, std::string const& name, uint8_t const port);

public:
    MidiPort(MidiPort const& source) = delete;
    MidiPort& operator=(MidiPort const& source) = delete;

public:
    MidiPort(MidiPort&& source) noexcept;
    MidiPort& operator=(MidiPort&& source) noexcept = delete;

protected:
    virtual void open();

protected:
    RtMidi& midi();
    bool isOpen() const;

public:
    std::string const& deviceName() const;
    uint8_t port() const;
    std::string uniqueDeviceName() const;
    bool compareName(std::string const& comparison, bool matchUnique);

private:
    RtMidi& m_midi;
    std::string m_name;
    uint8_t m_port;
    bool m_isOpen;
};
