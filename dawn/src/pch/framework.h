#pragma once
// Windows Includes (SDKs)
#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <winternl.h>
#include <d3d11.h>
#include <dxgi.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <vector>
#include <array>
#include <wrl/client.h>
#include <random>
#include <filesystem>
#include <timeapi.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <regex>
#include <xmmintrin.h>
// Namespace Defines
using namespace std::chrono;
using namespace std::chrono_literals;
namespace std
{
	namespace fs = std::filesystem;
}
// Types
#include "common/data/types.h"
// Quick operators
inline char* operator ""_char(const char* str, size_t) { return const_cast<char*>(str); }