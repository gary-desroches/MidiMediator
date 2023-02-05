/*

Copyright (c) 2023 Gary Des Roches

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

Any person wishing to distribute modifications to the Software is
asked to send the modifications to the original developer so that
they can be incorporated into the canonical version.  This is,
however, not a binding provision of this license.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

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
    static std::string const ApplicationName;
    static int32_t const MajorVersion;
    static int32_t const MinorVersion;

    std::vector<std::string> m_arguments;
    std::vector<std::string> m_inputDevices;
    std::vector<std::string> m_outputDevices;
    Configuration const m_config;
};
