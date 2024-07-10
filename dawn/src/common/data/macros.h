#pragma once
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