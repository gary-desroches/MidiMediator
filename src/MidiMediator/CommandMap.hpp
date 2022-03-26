#pragma once

#include "RevolvingCollection.hpp"

#include <vector>
#include <string>

class CommandMap
{
public:
	CommandMap();
	CommandMap(
		const std::vector<uint8_t>& inputMessage,
		RevolvingCollection<
			std::vector<
				std::vector<uint8_t>
			>
		> outputMessages,
		bool m_resetWhenAway
	);
	CommandMap(const CommandMap& source) = default;
	CommandMap(CommandMap&& source) = default;

public:
	std::vector<uint8_t>& inputMessage();
	const std::vector<uint8_t>& inputMessage() const;
	RevolvingCollection<
		std::vector<
		std::vector<uint8_t>
		>
	>& outputMessages();
	const RevolvingCollection<
		std::vector<
		std::vector<uint8_t>
		>
	>& outputMessages() const;
	bool& resetWhenAway();
	bool resetWhenAway() const;

private:
	std::vector<uint8_t> m_inputMessage;
	RevolvingCollection<
		std::vector<
		std::vector<uint8_t>
		>
	> m_outputMessages;
	bool m_resetWhenAway;
};