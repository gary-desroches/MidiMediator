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
#include "Configuration.hpp"
#include "CommandMap.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <exception>
#include <sstream>
#include <algorithm>
#include <memory>
#include <boost/algorithm/string.hpp>

#if defined(WIN32)
	const std::string Configuration::InputFieldName = "input_windows";
	const std::string Configuration::OutputsFieldName = "outputs_windows";
#elif defined(TARGET_OS_MAC)
	const std::string Configuration::InputFieldName = "input_macos";
	const std::string Configuration::OutputsFieldName = "outputs_macos";
#else
	const std::string Configuration::InputFieldName = "input_linux";
	const std::string Configuration::OutputsFieldName = "outputs_linux";
#endif

Configuration::Configuration(std::string const& configPath) :
    m_configPath(configPath),
    m_initialized(false),
	m_matchUniqueDeviceNames(false)
{
	initialize();
}

static [[nodiscard]] bool tryGetJsonArray(json11::Json const& node, std::string const& name, json11::Json& value)
{
	if (!node.is_object())
	{
		//std::cout << __PRETTY_FUNCTION__ << ": JSON node is not an object.\n";
		return false;
	}

	if (node.object_items().find(name) == node.object_items().end())
	{
		//std::cout << __PRETTY_FUNCTION__ << ": JSON node does not contain the child named \"" << name << "\".\n";
		return false;
	}

	if (!node[name].is_array())
	{
		//std::cout << __PRETTY_FUNCTION__ << ": JSON node is not an array.\n";
		return false;
	}

	value = node[name];

	return true;
}

static [[nodiscard]] bool tryGetJsonValue(json11::Json const& node, std::string const& name, std::string& value)
{
	if (!node.is_object())
	{
		//std::cout << __PRETTY_FUNCTION__ << ": node is not an object." << '\n';
		if (node.is_null())
		{
			//std::cout << __PRETTY_FUNCTION__ << ": node is null." << '\n';
		}
		else if (node.is_array())
		{
			//std::cout << __PRETTY_FUNCTION__ << ": node is an array." << '\n';
		}
		else if (node.is_bool())
		{
			//std::cout << __PRETTY_FUNCTION__ << ": node is a boolean." << '\n';
		}
		else if (node.is_number())
		{
			//std::cout << __PRETTY_FUNCTION__ << ": node is a number." << '\n';
		}
		else if (node.is_string())
		{
			//std::cout << __PRETTY_FUNCTION__ << ": node is a string." << '\n';
		}

		return false;
	}

	if (node.object_items().find(name) == node.object_items().end())
	{
		//std::cout << __PRETTY_FUNCTION__ << ": Could not find value " << name << " in node." << '\n';
		return false;
	}

	if (node[name].is_array() || node[name].is_null() || node[name].is_object())
	{
		//std::cout << __PRETTY_FUNCTION__ << ": JSON node is not an object.\n";
		return false;
	}

	if (node[name].is_number())
	{
		std::stringstream buffer;
		buffer << node[name].number_value();
		value = buffer.str();
		return true;
	}
	else if (node[name].is_bool())
	{
		value = node[name].bool_value() ? "true" : "false";
		return true;
	}
	else if (node[name].is_string())
	{
		value = node[name].string_value();
		return true;
	}
	else
	{
		std::stringstream errorMessage;
		errorMessage << __PRETTY_FUNCTION__ << ": Unknown JSON type\n";
		throw std::logic_error(errorMessage.str());
	}
}

static [[nodiscard]] std::string getJsonValue(json11::Json const& node, std::string const& name)
{
	std::string value;
	if (!tryGetJsonValue(node, name, value))
	{
		std::stringstream errorMessage;
		errorMessage << __PRETTY_FUNCTION__ << ": JSON node does not contain value named \"" << name << "\".";
		throw std::logic_error(errorMessage.str());
	}

	return value;
}

static [[nodiscard]] int32_t getJsonInt32Value(json11::Json const& node, std::string const& name)
{
	std::string value;
	if (!tryGetJsonValue(node, name, value))
	{
		std::stringstream errorMessage;
		errorMessage << __PRETTY_FUNCTION__ << ": JSON node does not contain value named \"" << name << "\".";
		throw std::logic_error(errorMessage.str());
	}

	size_t pos = 0;
	int32_t numericValue = std::stoi(value, &pos, 10);

	return static_cast<int32_t>(numericValue);
}

static [[nodiscard]] json11::Json getJsonArray(json11::Json const& node, std::string const& name)
{
	json11::Json value;
	if (!tryGetJsonArray(node, name, value))
	{
		std::stringstream errorMessage;
		errorMessage << __PRETTY_FUNCTION__ << ": JSON node does not contain array named \"" << name << "\".";
		throw std::logic_error(errorMessage.str());
	}

	return value;
}

static void splitToNumbers(std::string const text, std::vector<uint8_t>& numbers)
{
	try
	{
		if (text.size() == 0)
		{
			return;
		}

		std::vector<std::string> parts;
		boost::split(parts, text, boost::is_any_of(","));
		numbers.resize(parts.size());
		std::transform(parts.begin(), parts.end(), numbers.begin(),
			[](std::string const& text) { return static_cast<uint8_t>(std::atoi(text.c_str())); }
		);
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Invalid text format.  Expected comma-delimited byte values.\n";
		throw;
	}
}

[[nodiscard]] json11::Json Configuration::readJsonFromFile()
{
	try
	{
		std::cout << "Reading config file \"" << m_configPath << "\"...\n";
		std::ifstream file(m_configPath, std::ios_base::binary | std::ios_base::in);
		if (!file.good())
		{
			std::stringstream errorMessage;
			errorMessage << "Config file \"" << m_configPath << "\" could not be opened.";
			throw std::logic_error(errorMessage.str());
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string text = buffer.str();
		//std::cout << text << '\n';
		std::string error;
		json11::Json json(json11::Json::parse(text, error, json11::JsonParse::STANDARD));
		if (error.size() > 0)
		{
			throw std::logic_error(error);
		}

		return json;
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not read or parse config file.\n";
		throw;
	}
}

[[nodiscard]] MidiDeviceMapping::command_map_t Configuration::parseCommandMaps(json11::Json const& deviceMap)
{
	try
	{
		auto commandMapsNode = getJsonArray(deviceMap, "commandMaps");
		MidiDeviceMapping::command_map_t commandMaps;
		commandMaps.reserve(commandMapsNode.array_items().size());
		for (auto&& commandNode : commandMapsNode.array_items())
		{
			if (!commandNode.is_object())
			{
				continue;
			}

			commandMaps.push_back(
				CommandMap()
			);

			splitToNumbers(getJsonValue(commandNode, "in"), commandMaps.back().inputMessage());
			
			auto outNodeArray = getJsonArray(commandNode, "out");
			for (auto&& outNode : outNodeArray.array_items())
			{
				commandMaps.back().outputMessages().push_back({});
				for (auto&& outMessagesNode : outNode.array_items())
				{
					commandMaps.back().outputMessages().back().push_back({});
					splitToNumbers(outMessagesNode.string_value(), commandMaps.back().outputMessages().back().back());
				}
			}

			bool resetWhenAway = false;
			std::string resetWhenAwayText;
			if (tryGetJsonValue(commandNode, "resetWhenAway", resetWhenAwayText))
			{
				resetWhenAway = boost::iequals(resetWhenAwayText, "true");
			}

			commandMaps.back().resetWhenAway() = resetWhenAway;
		}

		return commandMaps;
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not parse command mappings.\n";
		throw;
	}
}

void Configuration::parseDeviceMap(json11::Json const& deviceMap)
{
	try
	{
		auto name = getJsonValue(deviceMap, "name");
		auto inputDeviceName = getJsonValue(deviceMap, InputFieldName);
		auto outputDeviceNamesNode = getJsonArray(deviceMap, OutputsFieldName);
		std::vector<std::string> outputDeviceNames;
		outputDeviceNames.reserve(outputDeviceNamesNode.array_items().size());
		for (auto&& outputDeviceNode : outputDeviceNamesNode.array_items())
		{
			outputDeviceNames.push_back(outputDeviceNode.string_value());
		}

		bool passthrough = boost::iequals(getJsonValue(deviceMap, "passthrough"), "true");
		int32_t multiMessageSendDelay = getJsonInt32Value(deviceMap, "multiMessageSendDelay");
		auto commandMaps = parseCommandMaps(deviceMap);

		m_deviceMaps.push_back(MidiDeviceMapping(name, inputDeviceName, outputDeviceNames, passthrough, multiMessageSendDelay, commandMaps));
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not parse the device map.\n";
		throw;
	}
}

void Configuration::initialize()
{
    if (m_initialized)
    {
        return;
    }

	try
	{
		json11::Json json = readJsonFromFile();

		m_queueSizeLimit = std::atoi(getJsonValue(json, "queueSizeLimit").c_str());
		m_matchUniqueDeviceNames = boost::iequals(getJsonValue(json, "matchUniqueDeviceNames"), "true");
		auto deviceMaps = getJsonArray(json, "deviceMaps");
		m_deviceMaps.reserve(deviceMaps.array_items().size());
		for (auto&& deviceMap : deviceMaps.array_items())
		{
			parseDeviceMap(deviceMap);
		}
	}
	catch (...)
	{
		std::cout << __PRETTY_FUNCTION__ << ": Could not parse the configuration file or data.\n";
		throw;
	}

    m_initialized = true;
}

[[nodiscard]] std::vector<MidiDeviceMapping> const& Configuration::deviceMaps() const
{
	return m_deviceMaps;
}

[[nodiscard]] uint32_t Configuration::queueSizeLimit() const
{
	return m_queueSizeLimit;
}

[[nodiscard]] bool Configuration::matchUniqueDeviceNames() const
{
	return m_matchUniqueDeviceNames;
}
