#include "pch.hpp"
#include "OutputMidiPort.hpp"

OutputMidiPort::OutputMidiPort(RtMidiOut& midi, std::string const& name, uint8_t const port) :
    MidiPort(midi, name, port)
{
}

OutputMidiPort::OutputMidiPort(OutputMidiPort&& source) noexcept :
    MidiPort(std::move(source))
{
}

/*
OutputMidiPort& OutputMidiPort::operator=(OutputMidiPort&& source) noexcept
{
    return static_cast<OutputMidiPort&>(MidiPort::operator=(std::move(source)));
}
*/

void OutputMidiPort::send(MidiMessage const& message)
{
    message.send(static_cast<RtMidiOut&>(midi()));
}
