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
