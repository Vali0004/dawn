#pragma once
#define HEX(v) "0x" << std::uppercase << std::hex << v << std::nouppercase
#define DEC(v) std::dec << v
#define ONCE(...) [&]()##__VA_ARGS__##()
#define nodisc [[nodiscard]]
#define unlikely [[unlikely]]
#define likely [[likely]]
#define nil 0
#define nilptr nullptr

#define rtry try
#define rcatchall catch(...)
#define rverifyall
#define rage_new new
#define UNUSED_PARAM(x)
#define DEBUGF1 printf
#define DEBUGF3 printf
#define WARNINGF printf
#define DISPLAYF printf
#define ERRORF printf
#define OUTPUT_ONLY(...) __VA_ARGS__
#define AssertVerify(...) true
#define AssertMsg(...) ;
#define Assert(...) ;
#define Verifyf(...) true
#define Warningf(...) ;
#define I64FMT PRIi64
#define formatf snprintf
#define sysMemCpy memcpy
#define safecpy memcpy
#define COUNTOF NELEM

#define BASE_NAME "Dawn"
#define BASE_BRANCH "main"
#define BASE_CANDIDATE "Dev"