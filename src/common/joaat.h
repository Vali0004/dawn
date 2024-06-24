#pragma once
#include <string_view>

inline constexpr char jTolower(char const c) {
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

inline constexpr uint32_t atStringHash(std::string_view string, bool const forceLowercase = true) {
	uint32_t hash{};
	const char* str = string.data();
	while (*str != '\0') {
		hash += forceLowercase ? jTolower(*str) : *str, ++str;
		hash += hash << 10, hash ^= hash >> 6;
	}
	hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);
	return hash;
}

inline uint32_t atDataHash(const char* data, size_t size, const uint32_t initValue) {
	uint32_t key{ initValue };
	for (size_t i{}; i != size; ++i) {
		key += data[i];
		key += (key << 10);
		key ^= (key >> 6);
	}
	key += (key << 3);
	key ^= (key >> 11);
	key += (key << 15);
	return key;
}

inline consteval uint32_t operator ""_j(const char* data, size_t) {
	return atStringHash(data);
}