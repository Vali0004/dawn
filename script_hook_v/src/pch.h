#ifndef PCH_H
#define PCH_H

#include "framework.h"

inline constexpr char jTolower(char const c)
{
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

inline constexpr uint32_t atStringHash(std::string_view string, bool const forceLowercase = true)
{
	uint32_t hash{};

	const char* str = string.data();
	while (*str != '\0')
	{
		hash += forceLowercase ? jTolower(*str) : *str, ++str;
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

inline bool g_running{ true };
inline HMODULE g_module{ reinterpret_cast<HMODULE>(atStringHash("dwn")) };

#endif //PCH_H