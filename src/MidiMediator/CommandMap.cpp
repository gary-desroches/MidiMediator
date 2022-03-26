#include "pch.hpp"
#include "CommandMap.hpp"

CommandMap::CommandMap() :
	m_resetWhenAway(false)
{
}

CommandMap::CommandMap(
	const std::vector<uint8_t>& inputMessage,
	RevolvingCollection<
	std::vector<
	std::vector<uint8_t>
	>
	> outputMessages,
	bool resetWhenAway
) :
	m_inputMessage(inputMessage),
	m_outputMessages(outputMessages),
	m_resetWhenAway(resetWhenAway)
{
}

std::vector<uint8_t>& CommandMap::inputMessage()
{
	return m_inputMessage;
}

const std::vector<uint8_t>& CommandMap::inputMessage() const
{
	return m_inputMessage;
}

RevolvingCollection<
	std::vector<
	std::vector<uint8_t>
	>
>&
CommandMap::outputMessages()
{
	return m_outputMessages;
}

const RevolvingCollection<
	std::vector<
	std::vector<uint8_t>
	>
>&
CommandMap::outputMessages() const
{
	return m_outputMessages;
}

bool& CommandMap::resetWhenAway()
{
	return m_resetWhenAway;
}

bool CommandMap::resetWhenAway() const
{
	return m_resetWhenAway;
}
