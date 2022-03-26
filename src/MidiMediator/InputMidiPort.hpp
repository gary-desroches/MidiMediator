#pragma once

#include "MidiPort.hpp"

#include <functional>
#include <memory>

class InputMidiPort : public MidiPort
{
public:
    typedef std::function<void(InputMidiPort& port, double, std::vector<uint8_t> const&)> callback_t;

public:
    InputMidiPort(RtMidiIn& midi, std::string const& name, uint8_t const port);
    InputMidiPort(InputMidiPort const& source) = delete;
    InputMidiPort(InputMidiPort&& source) noexcept;
    InputMidiPort& operator=(InputMidiPort const& source) = delete;

public:
    void open(callback_t callback);

private:
    static void onIncomingMessage(double /* timeStamp */, std::vector<uint8_t>* messageBytes, void* userData);
    void onIncomingMessage(double const /* timeStamp */, std::vector<uint8_t> const& messageBytes);

private:
    std::unique_ptr<callback_t> m_callback;
    bool m_init;
};
