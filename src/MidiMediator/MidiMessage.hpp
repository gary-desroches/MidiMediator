#pragma once

#include <vector>
#include <array>
#include <utility>
#include "RtMidi.h"
#include "pch.hpp"

enum class [[nodiscard]] MidiMessageCode : uint8_t
{
	// Channel Voice
	NoteOff = 0x80,
	NoteOn = 0x90,
	PolyphonicKeyPressure = 0xa0,
	ControlChange = 0xb0,
	ProgramChange = 0xc0,
	ChannelPressure = 0xd0,
	PitchBend = 0xe0,

	// System Messages
	TimingClock = 0xf8,
	StartSequence = 0xfa,
	ContinueSequence = 0xfb,
	StopSequence = 0xfc,
	ActiveSensing = 0xfe,
	SystemReset = 0xff,

	// System Common Messages
	MidiTimingCode = 0xf1,
	SongPositionPointer = 0xf2,
	SongSelect = 0xf3,
	TuneRequest = 0xf6,

	// System Exclusive Messages
	SystemExclusiveStart = 0xf0,
	SystemExclusiveEnd = 0xf7
};

enum class ChannelModeMessageCode
{
	ResetAllControllers = 0x79,
	LocalControl = 0x7a,
	AllNotesOff = 0x7b,
	OmniModeOff = 0x7c,
	OmniModeOn = 0x7d,
	MonoModeOn = 0x7e,
	PolyModeOn = 0x7f
};

class MidiMessage
{
public:
	MidiMessage(std::vector<uint8_t> const& data);
	MidiMessage(uint8_t const channel, uint8_t const code, std::vector<uint8_t> const& data);

public:
	MidiMessage(MidiMessage const& source) = delete;
	MidiMessage& operator=(MidiMessage const& source) = delete;

public:
	MidiMessage(MidiMessage&& source) noexcept;
	MidiMessage& operator=(MidiMessage&& source) noexcept;

private:
	void validate() const;

public:
	uint8_t channel() const;
	MidiMessageCode code() const;
	std::vector<uint8_t> const& data() const;
	void send(RtMidiOut& output) const;

private:
	uint8_t m_channel;
	uint8_t m_code;
	std::vector<uint8_t> m_data;
	bool m_invalidated;

private:
	static std::array< std::pair<uint8_t, uint8_t>, 19> const DataLengths;
	static std::array< std::pair<uint8_t, bool>, 19 > const ChannelCommands;
};
