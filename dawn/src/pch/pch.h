#ifndef PCH_H
#define PCH_H

#include "common/macros.h"
#include "common/vectors.h"
#include "framework.h"
#include "logger.h"

inline constexpr std::array<char, 27> g_alphabet{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
inline constexpr bool is_character_alphanumerical(char c) {
	for (auto& a : g_alphabet) {
		if (c == a) {
			return true;
		}
	}
	return false;
}
inline constexpr bool is_string_alphanumerical(const std::string_view& str) {
	for (auto& c : str) {
		if (!is_character_alphanumerical(c)) {
			return false;
		}
	}
	return true;
}
inline constexpr bool fingerprint_hex(const std::string_view& str) {
	// Fingerprint the first bytes "0x"
	if (str.at(0) == '0' && str.at(1) == 'x') {
		return true;
	}
	// Check if it contains alphanumerical characters
	if (is_string_alphanumerical(str)) {
		return true;
	}
	// Otherwise, asume decimal
	return false;
}
inline std::vector<std::string> split_string(const std::string& str, char split) {
	std::vector<std::string> strings{};
	std::string string{};
	for (s32 i{}; i != str.length(); ++i) {
		if (str.at(i) == split) {
			strings.push_back(string);
			string.erase();
		}
		else {
			string += str.at(i);
			if (i == str.length() - 1)
				strings.push_back(string);
		}
	}
	return strings;
}
inline void strip_character(std::string& str, char c) {
	str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

inline HMODULE g_main{ GetModuleHandleA(NULL) };
inline HMODULE g_entry{};
inline bool g_running{ true };
inline bool g_was_injected_early{};
inline bool g_game_speedup_active{};

#endif //PCH_H