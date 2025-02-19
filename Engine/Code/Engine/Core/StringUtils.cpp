#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>
#include <string>
#include <cctype>

//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;


Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn, bool removeEmpty)
{
	Strings result;
	std::string currentString;

	if (originalString.empty())
	{
		result.emplace_back("");
		return result;
	}
	for (int i = 0; i < (int)originalString.size(); i++)
	{
		if (originalString[i] == delimiterToSplitOn)
		{
			if (!currentString.empty())
			{
				result.push_back(currentString);
			}
			currentString.clear();
		}
		else
		{
			if (removeEmpty && originalString[i] == ' ')
			{
				continue;
			}

			currentString.push_back(originalString[i]);

			if (i == (int)originalString.size() - 1)
			{
				result.push_back(currentString);
			}
		}
	}
	return result;
}

int SplitStringOnDelimiter(Strings& out_splitStrings, std::string const& originalString, std::string const& delimiterToSplitOn, bool removeEmpty)
{
	Strings result;
	std::string currentString;

	if (originalString.empty())
	{
		result.emplace_back("");
		out_splitStrings = result;
		return (int)result.size();
	}

	int currentSameWithDelimiterCharIndex = 0;
	std::string currentSameWithDelimiterString;

	for (int i = 0; i < (int)originalString.size(); i++)
	{
		char currentChar = originalString[i];

		if (currentChar == delimiterToSplitOn[currentSameWithDelimiterCharIndex])
		{
			currentSameWithDelimiterString.push_back(currentChar);
			currentSameWithDelimiterCharIndex++;

			if (currentSameWithDelimiterString == delimiterToSplitOn)
			{
				result.push_back(currentString);
				currentString.clear();
				currentSameWithDelimiterString.clear();
				currentSameWithDelimiterCharIndex = 0;
			}
		}
		else
		{
			if (removeEmpty && (currentChar == ' '))
			{
				continue;
			}

			if (currentChar == 10 || currentChar == 13)
			{
				continue;
			}

			if (currentSameWithDelimiterString.empty())
			{
				currentString.push_back(currentChar);
			}
			else
			{
				currentString.append(currentSameWithDelimiterString);
				currentString.push_back(currentChar);
				currentSameWithDelimiterString.clear();
				currentSameWithDelimiterCharIndex = 0;
			}
		}

		if (i == (int)originalString.size() - 1)
		{
			if (!currentSameWithDelimiterString.empty())
			{
				currentString.push_back(currentChar);
			}
			result.push_back(currentString);
		}
	}

	out_splitStrings = result;

	return (int)result.size();
}

Strings SplitStringOnDelimiter(std::string const& originalString, std::string const& delimiterToSplitOn, bool removeEmpty /*= false*/)
{
	Strings result;
	std::string currentString;

	if (originalString.empty())
	{
		result.emplace_back("");
		return result;
	}

	int currentSameWithDelimiterCharIndex = 0;
	std::string currentSameWithDelimiterString;

	for (int i = 0; i < (int)originalString.size(); i++)
	{
		char currentChar = originalString[i];

		if (currentChar == delimiterToSplitOn[currentSameWithDelimiterCharIndex])
		{
			currentSameWithDelimiterString.push_back(currentChar);
			currentSameWithDelimiterCharIndex++;

			if (currentSameWithDelimiterString == delimiterToSplitOn)
			{
				result.push_back(currentString);
				currentString.clear();
				currentSameWithDelimiterString.clear();
				currentSameWithDelimiterCharIndex = 0;
			}
		}
		else
		{
			if (removeEmpty && (currentChar == ' '))
			{
				continue;
			}

			if (currentChar == 10 || currentChar == 13)
			{
				continue;
			}

			if (currentSameWithDelimiterString.empty())
			{
				currentString.push_back(currentChar);
			}
			else
			{
				currentString.append(currentSameWithDelimiterString);
				currentString.push_back(currentChar);
				currentSameWithDelimiterString.clear();
				currentSameWithDelimiterCharIndex = 0;
			}
		}

		if (i == (int)originalString.size() - 1)
		{
			if (!currentSameWithDelimiterString.empty())
			{
				currentString.push_back(currentChar);
			}
			result.push_back(currentString);
		}
	}

	return result;
}

std::string ToLower(const std::string& string)
{
	std::string result = string;
	for (char& c : result)
	{
		c = (char)std::tolower(static_cast<unsigned char>(c));
	}
	return result;
}

void TrimString(std::string& stringToTrim, char delimiter)
{
	if (stringToTrim.empty())
		return;

	size_t start = stringToTrim.find_first_not_of(delimiter);

	if (start == std::string::npos)
	{
		stringToTrim.clear();
		return;
	}
	size_t end = stringToTrim.find_last_not_of(delimiter);
	stringToTrim = stringToTrim.substr(start, end - start + 1);
}

Strings SplitStringWithQuotes(const std::string& string, char delimiter)
{
	Strings result;
	std::string current;
	bool inQuotes = false;
	char quoteChar = '\0';

	for (char c : string)
	{
		if (c == '"' || c == '\'')
		{
			if (inQuotes && c == quoteChar)
			{
				inQuotes = false;
				quoteChar = '\0';
			}
			else if (!inQuotes)
			{
				inQuotes = true;
				quoteChar = c;
			}

			current += c;
		}
		else if (c == delimiter && !inQuotes)
		{
			if (!current.empty())
			{
				result.push_back(current);
				current.clear();
			}
		}
		else
		{
			current += c;
		}
	}

	if (!current.empty())
	{
		result.push_back(current);
	}

	return result;
}

//-----------------------------------------------------------------------------------------------
const std::string Stringf(char const* format, ...)
{
	char textLiteral[STRINGF_STACK_LOCAL_TEMP_LENGTH];
	va_list variableArgumentList;
	va_start(variableArgumentList, format);
	vsnprintf_s(textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList);
	va_end(variableArgumentList);
	textLiteral[STRINGF_STACK_LOCAL_TEMP_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string(textLiteral);
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf(int maxLength, char const* format, ...)
{
	char textLiteralSmall[STRINGF_STACK_LOCAL_TEMP_LENGTH];
	char* textLiteral = textLiteralSmall;
	if (maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH)
		textLiteral = new char[maxLength];

	va_list variableArgumentList;
	va_start(variableArgumentList, format);
	vsnprintf_s(textLiteral, maxLength, _TRUNCATE, format, variableArgumentList);
	va_end(variableArgumentList);
	textLiteral[maxLength - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue(textLiteral);
	if (maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH)
		delete[] textLiteral;

	return returnValue;
}





