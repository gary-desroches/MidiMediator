#include "pch.hpp"
#include "Application.hpp"
#include "InputMidiDevice.hpp"

#include "RtMidi.h"

int main(int argc, char* argv[])
{
	Application application(argc, argv);
	application.run();

    return 0;
}
