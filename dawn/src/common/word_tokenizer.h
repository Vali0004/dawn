#pragma once
#include "pch/framework.h"
#include "logger.h"

namespace dwn
{
	struct token
	{
		std::string_view word;
		u32 value;
	};

	inline constexpr token g_tokens[]{
		{ "zero", 0 }, { "one", 1 }, { "two", 2 }, { "three", 3 }, { "four", 4 },
		{ "five", 5 }, { "six", 6 }, { "seven", 7 }, { "eight", 8 }, { "nine", 9 },
		{ "ten", 10 }, { "eleven", 11 }, { "twelve", 12 }, { "thirteen", 13 },
		{ "fourteen", 14 }, { "fifteen", 15 }, { "sixteen", 16 }, { "seventeen", 17 },
		{ "eighteen", 18 }, { "nineteen", 19 }, { "twenty", 20 }, { "thirty", 30 },
		{ "forty", 40 }, { "fifty", 50 }, { "sixty", 60 }, { "seventy", 70 },
		{ "eighty", 80 }, { "ninety", 90 }, { "hundred", 100 }, { "thousand", 1000 },
		{ "million", 1000000 }
	};

	inline constexpr token g_ordinal_tokens[]{
		{ "zeroth", 0 }, { "first", 1 }, { "second", 2 }, { "third", 3 }, { "fourth", 4 },
		{ "fifth", 5 }, { "sixth", 6 }, { "seventh", 7 }, { "eighth", 8 }, { "ninth", 9 },
		{ "tenth", 10 }, { "eleventh", 11 }, { "twelfth", 12 }, { "thirteenth", 13 },
		{ "fourteenth", 14 }, { "fifteenth", 15 }, { "sixteenth", 16 }, { "seventeenth", 17 },
		{ "eighteenth", 18 }, { "nineteenth", 19 }, { "twentieth", 20 }, { "thirtieth", 30 },
		{ "fortieth", 40 }, { "fiftieth", 50 }, { "sixtieth", 60 }, { "seventieth", 70 },
		{ "eightieth", 80 }, { "ninetieth", 90 }, { "hundredth", 100 }, { "thousandth", 1000 },
		{ "millionth", 1000000 }
	};

	inline std::optional<token> get_token(const std::string_view& input, bool ordinal = false)
	{
		const auto& tokens = ordinal ? g_ordinal_tokens : g_tokens;
		for (const auto& token : tokens)
		{
			if (!token.word.compare(input.data()))
			{
				return token;
			}
		}

		// If we find out it's nullopt, just check if it contains the inverse of the current operation.
		// However, this might go poorly.
		if (auto value{ get_token(input, ordinal ^ true) }; value.has_value())
		{
			return value;
		}

		return std::nullopt;
	}

	inline std::optional<token> get_token_by_value(uint32_t input, bool ordinal = false)
	{
		const auto& tokens = ordinal ? g_ordinal_tokens : g_tokens;
		for (const auto& token : tokens)
		{
			if (token.value == input)
			{
				return token;
			}
		}

		if (auto value{ get_token_by_value(input, ordinal ^ true) }; value.has_value())
		{
			return value;
		}

		return std::nullopt;
	}

	inline u64 word_to_number(const std::string& input)
	{
		std::istringstream ss{ input };
		std::string word{};
		std::vector<u64> numbers{};

		while (ss >> word)
		{
			auto token{ get_token(word) };
			if (token.has_value())
			{
				numbers.push_back(token->value);
			}
			else
			{
				LOG_TO_STREAM("Invalid found token for '" << input << "'");
				return ~0;
			}
		}

		u64 result{};
		u64 current_value{};
		for (auto& number : numbers)
		{
			if (number == 100 || number == 1000 || number == 1000000)
			{
				if (!current_value)
				{
					current_value = 1;
				}
				current_value *= number;
			}
			else
			{
				current_value += number;
			}

			if (number == 1000 || number == 1000000)
			{
				result += current_value;
				current_value = 0;
			}
		}

		result += current_value;

		return result;
	}

	inline std::string number_to_word(uint64_t number, bool ordinal = false)
	{
		if (number == 0)
		{
			return ordinal ? "zeroth" : "zero";
		}

		std::string result;

		if (number >= 1000000)
		{
			uint64_t millions = number / 1000000;
			result += number_to_word(millions) + " million ";
			number %= 1000000;
		}

		if (number >= 1000)
		{
			uint64_t thousands = number / 1000;
			result += number_to_word(thousands) + " thousand ";
			number %= 1000;
		}

		if (number >= 100)
		{
			uint64_t hundreds = number / 100;
			result += number_to_word(hundreds) + " hundred ";
			number %= 100;
		}

		if (number >= 20)
		{
			uint64_t tens = (number / 10) * 10;
			auto token = get_token_by_value(tens);
			if (token.has_value())
			{
				result += token->word;
				number %= 10;
				if (number != 0)
				{
					result += "-";
				}
			}
		}

		if (number > 0 && number < 20)
		{
			auto token = get_token_by_value(number, ordinal);
			if (token.has_value())
			{
				result += token->word;
			}
		}
		else if (number == 0 && ordinal)
		{
			result.pop_back();
			result += "th";
		}

		return result;
	}
}