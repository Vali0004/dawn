#pragma once
#define HEX(v) "0x" << std::uppercase << std::hex << v << std::nouppercase
#define DEC(v) std::dec << v
#define ONCE(...) [&]()##__VA_ARGS__##()
#define nodisc [[nodiscard]]
#define unlikely [[unlikely]]
#define likely [[likely]]
#define nil NULL
#define nilptr nullptr

#define rtry try
#define rcatchall catch(...)
#define rverifyall
#define rage_new new

#define BASE_NAME "Dawn"
#define BASE_BRANCH "main"
#define BASE_CANDIDATE "Dev"