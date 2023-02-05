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

#include "pch.hpp"
#include "Application.hpp"
#include "Configuration.hpp"
#include "MidiDeviceMapper.hpp"

#include <boost/algorithm/string.hpp>

#if defined(WIN32)
	RtMidi::Api const Application::Api = RtMidi::WINDOWS_MM;
	std::string const Application::ApiName = "Windows MM";
	std::string const Application::ConfigPath = "MidiMediator.json";
#elif defined(TARGET_OS_MAC)
	RtMidi::Api const Application::Api = RtMidi::MACOSX_CORE;
	std::string const Application::ApiName = "MacOS X Core";
	std::string const Application::ConfigPath = "/etc/MidiMediator.json";
#else
	RtMidi::Api const Application::Api = RtMidi::LINUX_ALSA;
	std::string const Application::ApiName = "Linux ALSA";
	std::string const Application::ConfigPath = "/etc/MidiMediator.json";
#endif

const std::string Application::ApplicationName = "MidiMediator";
const int32_t Application::MajorVersion = 0;
const int32_t Application::MinorVersion = 3;

Application::Application(int argc, char* argv[]) :
	m_config(ConfigPath)
{
	m_arguments.reserve(argc - 1);
	for (int32_t index = 1; index < argc; index += 1)
	{
		m_arguments.push_back(std::string(argv[index]));
	}
}

void Application::listPorts()
{
	try
	{
		RtMidiIn mainInputDevice(Api, "Main Input", m_config.queueSizeLimit());
		size_t inputPortCount = mainInputDevice.getPortCount();
		std::cout << "Input port count: " << inputPortCount << '\n';
		for (unsigned int port = 0; port < inputPortCount; port += 1)
		{
			std::string portName = mainInputDevice.getPortName(port);
			std::cout << "Port " << port << ": " << portName << '\n';
		}
	}
	catch (...)
	{
		std::cout << "Could not list input ports.\n";
		throw;
	}

	try
	{
		RtMidiOut mainOutputDevice(Api, "Main Output");
		size_t outputPortCount = mainOutputDevice.getPortCount();
		std::cout << "Output port count: " << outputPortCount << '\n';
		for (unsigned int port = 0; port < outputPortCount; port += 1)
		{
			std::string portName = mainOutputDevice.getPortName(port);
			std::cout << "Port " << port << ": " << portName << '\n';
		}
	}
	catch (...)
	{
		std::cout << "Could not list output ports.\n";
		throw;
	}
}

std::string trimNumericSuffix(std::string const& deviceName);

int Application::run()
{
	std::cout << ApplicationName << " v" << MajorVersion << "." << MinorVersion << '\n';
	std::cout << "Using " << ApiName << " as the MIDI API.\n";

	try
	{
		if (m_arguments.size() > 0)
		{
			if (boost::iequals(m_arguments[0], "--list"))
			{
				listPorts();
				return 0;
			}
		}

		MidiDeviceMapper mapper(
			Api, 
			m_config.queueSizeLimit(), 
			m_config.deviceMaps(), 
			m_config.matchUniqueDeviceNames()
		);

		mapper.listen();
	}
	catch (RtMidiError& error)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled RtMidi Exception\n";
		error.printMessage();

		return -1;
	}
	catch (std::exception& error)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled std::exception\n" << error.what() << '\n';
		return -1;
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Unhandled unknown exception\n";
		return -1;
	}

	return 0;
}
