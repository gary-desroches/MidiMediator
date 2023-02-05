 # Copyright (c) 2023 Gary Des Roches

 # Permission is hereby granted, free of charge, to any person
 # obtaining a copy of this software and associated documentation files
 # (the "Software"), to deal in the Software without restriction,
 # including without limitation the rights to use, copy, modify, merge,
 # publish, distribute, sublicense, and/or sell copies of the Software,
 # and to permit persons to whom the Software is furnished to do so,
 # subject to the following conditions:

 # The above copyright notice and this permission notice shall be
 # included in all copies or substantial portions of the Software.

 # Any person wishing to distribute modifications to the Software is
 # asked to send the modifications to the original developer so that
 # they can be incorporated into the canonical version.  This is,
 # however, not a binding provision of this license.

 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 # EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 # MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 # IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 # ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 # CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 # WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# CMakeList.txt : CMake project for MidiMediator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.18)

include_directories(${VCPKG_INCLUDE_DIR})
#link_libraries(${VCPKG_LIB_DIR})

# Add source to this project's executable.
add_executable (MidiMediator "main.cpp" "Application.cpp" "Application.hpp" "Configuration.cpp" "Configuration.hpp" "InputMidiDevice.cpp" "InputMidiDevice.hpp" "InputMidiPort.cpp" "InputMidiPort.hpp" "MessageDirection.hpp" "MidiDevice.cpp" "MidiDevice.hpp" "MidiDeviceMapper.cpp" "MidiDeviceMapper.hpp" "MidiDeviceMapping.cpp" "MidiDeviceMapping.hpp" "MidiMessage.cpp" "MidiMessage.hpp" "MidiPort.cpp" "MidiPort.hpp" "OutputMidiDevice.cpp" "OutputMidiDevice.hpp" "OutputMidiPort.cpp" "OutputMidiPort.hpp" "pch.cpp" "pch.hpp" "targetver.hpp" "json11.cpp" "json11.hpp" "RevolvingCollection.hpp" "RevolvingCollection.cpp" "CommandMap.hpp" "CommandMap.cpp")

# TODO: Add tests and install targets if needed.
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#target_link_libraries(MidiMediator json11)
	target_link_libraries(MidiMediator rtmidi)
	target_link_libraries(MidiMediator asound)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	#target_link_libraries(MidiMediator ${VCPKG_LIB_DIR}/json11.lib)
	target_link_libraries(MidiMediator ${VCPKG_LIB_DIR}/rtmidi.lib)
	target_link_libraries(MidiMediator winmm)
endif()
