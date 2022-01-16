#pragma once

#include "OutputMidiPort.hpp"
#include "Configuration.hpp"
#include "pch.hpp"
#include "InputMidiDevice.hpp"
#include "OutputMidiDevice.hpp"
#include "MidiMessage.hpp"

class Application
{
public:
    Application(int argc, char* argv[]);

public:
    Application(Application const& source) = delete;
    Application& operator=(Application const& source) = delete;

public:
    Application(Application&& source) = delete;
    Application& operator=(Application&& source) = delete;

public:
    int run();

private:
    void listPorts();

private:
    static std::string const ApiName;
    static std::string const ConfigPath;
    static RtMidi::Api const Api;

    std::vector<std::string> m_arguments;
    std::vector<std::string> m_inputDevices;
    std::vector<std::string> m_outputDevices;
    Configuration const m_config;
};
