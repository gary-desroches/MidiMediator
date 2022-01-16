// Linux Notes
// Ubuntu Server 21.xx ARM64

// sudo apt update
// sudo apt upgrade
// sudo reboot

// sudo nano /etc/hostname
// Change hostname, Ctrl+O, enter, Ctrl+X
// sudo apt update
// Copy /bin/MidiMediator and /etc/MidiMediator.json
// sudo chmod +X /bin/MidiMediator

// Configure SSH if it isn't already on:
// systemctl status sshd
// sudo ufw allow ssh
// sudo systemctl enable ssh
// ip a

// libpthread-stubs0-dev openssh-server 

// Install packages:
// sudo apt install zip unzip gcc g++ make cmake ninja-build alsa libasound2-dev git librtmidi-dev libjson11-1-dev libboost-all-dev gdb

// Create swap file:
// swapon --show
// sudo fallocate -l 8G /swapfile
// ls -lh /swapfile
// sudo chmod 600 /swapfile
// sudo mkswap /swapfile
// sudo swapon /swapfile
// sudo swapon --show
// free -h
// echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab
// sudo sysctl vm.swappiness=10
// sudo sysctl vm.vfs_cache_pressure=50
// sudo nano /etc/sysctl.conf
//   vm.swappiness=10
//   vm.vfs_cache_pressure = 50
// sudo reboot
// sudo swapon --show

// Clone and build vcpkg
// cd ~
// mkdir code
// cd code
// git clone https://github.com/microsoft/vcpkg.git
// ./vcpkg/bootstrap-vcpkg.sh

// Upgrade CMake to 3.21 if necessary:
// sudo apt install libssl-dev
// Download tar.gz source code
// untar
// ./bootstrap
// gmake
// make install?

// To use vcpkg with cmake:
// cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
// cmake --build[build directory]

// Build without cmake:
// g++ -Wall -D__LINUX_ALSA__ -std=c++17 -o MidiMediator main.cpp Application.cpp Configuration.cpp InputMidiDevice.cpp InputMidiPort.cpp MidiDevice.cpp MidiPort.cpp OutputMidiDevice.cpp OutputMidiPort.cpp  main.cpp RtMidi.cpp MidiMessage.cpp pch.cpp -lpthread -lasound -lrtmidi -ljson11 -I/usr/include/rtmidi -I/usr/include/json11

// Get MidiMediator to run at startup
// sudo nano /etc/systemd/system/rc-local.service
/*
[Unit]
 Description=/etc/init.d/rc.local Compatibility
 ConditionPathExists=/etc/init.d/rc.local

[Service]
 Type=forking
 ExecStart=/etc/init.d/rc.local start
 TimeoutSec=0
 StandardOutput=tty
 RemainAfterExit=yes
 SysVStartPriority=99

[Install]
 WantedBy=multi-user.target
*/
// sudo nano /etc/init.d/rc.local
/*

#!/bin/bash -e
/usr/bin/MidiMediator

*/
// sudo chmod +x /etc/init.d/rc.local
// sudo systemctl enable rc-local
// sudo reboot
// sudo systemctl status rc-local

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
