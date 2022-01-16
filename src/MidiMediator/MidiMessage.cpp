#include "pch.hpp"
#include "MidiMessage.hpp"

#include <cassert>

std::array< std::pair< uint8_t, uint8_t>, 19> const MidiMessage::DataLengths =
{{
	{ static_cast<uint8_t>(MidiMessageCode::NoteOff), static_cast<uint8_t>(2U) },
	{ static_cast<uint8_t>(MidiMessageCode::NoteOn), static_cast<uint8_t>(2U) },
	{ static_cast<uint8_t>(MidiMessageCode::PolyphonicKeyPressure), static_cast<uint8_t>(2U) },
	{ static_cast<uint8_t>(MidiMessageCode::ControlChange), static_cast<uint8_t>(2U) },
	{ static_cast<uint8_t>(MidiMessageCode::ProgramChange), static_cast<uint8_t>(1U) },
	{ static_cast<uint8_t>(MidiMessageCode::ChannelPressure), static_cast<uint8_t>(1U) },
	{ static_cast<uint8_t>(MidiMessageCode::PitchBend), static_cast<uint8_t>(2U) },
	{ static_cast<uint8_t>(MidiMessageCode::TimingClock), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::StartSequence), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::ContinueSequence), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::StopSequence), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::ActiveSensing), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::SystemReset), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::MidiTimingCode), static_cast<uint8_t>(1U) },
	{ static_cast<uint8_t>(MidiMessageCode::SongPositionPointer), static_cast<uint8_t>(2U) },
	{ static_cast<uint8_t>(MidiMessageCode::SongSelect), static_cast<uint8_t>(1U) },
	{ static_cast<uint8_t>(MidiMessageCode::TuneRequest), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::SystemExclusiveStart), static_cast<uint8_t>(0U) },
	{ static_cast<uint8_t>(MidiMessageCode::SystemExclusiveEnd), static_cast<uint8_t>(0U) }
}};

std::array< std::pair<uint8_t, bool>, 19 > const MidiMessage::ChannelCommands =
{{
	{ static_cast<uint8_t>(MidiMessageCode::NoteOff), true },
	{ static_cast<uint8_t>(MidiMessageCode::NoteOn), true },
	{ static_cast<uint8_t>(MidiMessageCode::PolyphonicKeyPressure), true },
	{ static_cast<uint8_t>(MidiMessageCode::ControlChange), true },
	{ static_cast<uint8_t>(MidiMessageCode::ProgramChange), true },
	{ static_cast<uint8_t>(MidiMessageCode::ChannelPressure), true },
	{ static_cast<uint8_t>(MidiMessageCode::PitchBend), true },
	{ static_cast<uint8_t>(MidiMessageCode::TimingClock), false },
	{ static_cast<uint8_t>(MidiMessageCode::StartSequence), false },
	{ static_cast<uint8_t>(MidiMessageCode::ContinueSequence),false },
	{ static_cast<uint8_t>(MidiMessageCode::StopSequence), false },
	{ static_cast<uint8_t>(MidiMessageCode::ActiveSensing), false },
	{ static_cast<uint8_t>(MidiMessageCode::SystemReset), false },
	{ static_cast<uint8_t>(MidiMessageCode::MidiTimingCode), false },
	{ static_cast<uint8_t>(MidiMessageCode::SongPositionPointer), false },
	{ static_cast<uint8_t>(MidiMessageCode::SongSelect), false },
	{ static_cast<uint8_t>(MidiMessageCode::TuneRequest), false },
	{ static_cast<uint8_t>(MidiMessageCode::SystemExclusiveStart), false },
	{ static_cast<uint8_t>(MidiMessageCode::SystemExclusiveEnd), false }
}};

MidiMessage::MidiMessage(uint8_t const channel, uint8_t const code, std::vector<uint8_t> const& data) :
	m_channel(channel),
	m_code(code),
	m_data(data),
	m_invalidated(false)
{
	validate();
}

MidiMessage::MidiMessage(std::vector<uint8_t> const& data) :
	m_invalidated(false)
{
	if (data.size() == 0)
	{
		throw std::invalid_argument("data.size() == 0");
	}

	m_code = static_cast<uint8_t>(data[0] & 0b11110000);
	for (auto&& command : ChannelCommands)
	{
		if (command.first == m_code)
		{
			m_channel = static_cast<uint8_t>(data[0] & 0b00001111);
			break;
		}
	}

	for (size_t dataIndex = 1; dataIndex < data.size(); ++dataIndex)
	{
		m_data.push_back(data[dataIndex]);
	}

	validate();
}

void MidiMessage::validate() const
{
	if (code() == MidiMessageCode::SystemExclusiveStart)
	{
		if (m_data.size() == 0)
		{
			throw std::logic_error("m_data.size() == 0 for SYSEXSTART");
		}

		if (static_cast<MidiMessageCode>(m_data.back()) != MidiMessageCode::SystemExclusiveEnd)
		{
			throw std::logic_error("m_data.back() != SYSEXEND for SYSEXSTART");
		}
	}
	else
	{
		for (auto&& pair : DataLengths)
		{
			if (pair.first == m_code)
			{
				if (pair.second != m_data.size())
				{
					throw std::logic_error("pair.second != _data.size()");
				}

				return;
			}
		}

		throw std::logic_error("MidiMessage::_code not found in _dataLengths");
	}
}

uint8_t MidiMessage::channel() const
{
	assert(!m_invalidated);
	return m_channel;
}

MidiMessageCode MidiMessage::code() const
{
	assert(!m_invalidated);
	return static_cast<MidiMessageCode>(m_code);
}

std::vector<uint8_t> const& MidiMessage::data() const
{
	assert(!m_invalidated);
	return m_data;
}

void MidiMessage::send(RtMidiOut& output) const
{
	assert(!m_invalidated);

	std::vector<uint8_t> data;
	data.reserve(data.size() + 1);
	data.push_back(static_cast<uint8_t>(m_code | m_channel));
	for (auto&& d : m_data)
	{
		data.push_back(d);
	}

	try
	{
		output.sendMessage(&data);
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not send message.\n";
		throw;
	}
}

MidiMessage::MidiMessage(MidiMessage&& source) noexcept :
	m_channel(source.m_channel),
	m_code(source.m_code),
	m_data(std::move(source.m_data)),
	m_invalidated(false)
{
	source.m_invalidated = true;
}

MidiMessage& MidiMessage::operator=(MidiMessage&& source) noexcept
{
	m_invalidated = false;
	m_channel = source.m_channel;
	m_code = source.m_code;
	m_data = std::move(source.m_data);

	source.m_invalidated = true;

	return *this;
}
