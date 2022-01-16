# CMakeList.txt : CMake project for MidiMediator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.18)

include_directories(${VCPKG_INCLUDE_DIR})
#link_libraries(${VCPKG_LIB_DIR})

# Add source to this project's executable.
add_executable (MidiMediator "main.cpp" "Application.cpp" "Application.hpp" "Configuration.cpp" "Configuration.hpp" "InputMidiDevice.cpp" "InputMidiDevice.hpp" "InputMidiPort.cpp" "InputMidiPort.hpp" "MessageDirection.hpp" "MidiDevice.cpp" "MidiDevice.hpp" "MidiDeviceMapper.cpp" "MidiDeviceMapper.hpp" "MidiDeviceMapping.cpp" "MidiDeviceMapping.hpp" "MidiMessage.cpp" "MidiMessage.hpp" "MidiPort.cpp" "MidiPort.hpp" "OutputMidiDevice.cpp" "OutputMidiDevice.hpp" "OutputMidiPort.cpp" "OutputMidiPort.hpp" "pch.cpp" "pch.hpp" "targetver.hpp" "RtMidi.cpp" "RtMidi.h" "json11.cpp" "json11.hpp")

# TODO: Add tests and install targets if needed.
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#target_link_libraries(MidiMediator json11)
	#target_link_libraries(MidiMediator rtmidi)
	target_link_libraries(MidiMediator asound)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	#target_link_libraries(MidiMediator ${VCPKG_LIB_DIR}/json11.lib)
	#target_link_libraries(MidiMediator ${VCPKG_LIB_DIR}/rtmidi.lib)
	target_link_libraries(MidiMediator winmm)
endif()
