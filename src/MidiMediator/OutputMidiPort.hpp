#pragma once

#include "MidiPort.hpp"
#include "MidiMessage.hpp"

class OutputMidiPort : public MidiPort
{
public:
    OutputMidiPort(RtMidiOut& midi, std::string const& name, uint8_t const port);

public:
    OutputMidiPort(OutputMidiPort const& source) = delete;
    OutputMidiPort& operator=(OutputMidiPort const& source) = delete;

public:
    OutputMidiPort(OutputMidiPort&& source) noexcept;
    //OutputMidiPort& operator=(OutputMidiPort&& source) noexcept;

public:
    using MidiPort::open;
    void send(MidiMessage const& message);
};
