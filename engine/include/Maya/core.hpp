#pragma once

#if !defined(__cplusplus)
#error cpp is required
#endif

#if defined (_MSC_VER) // msvc compiler
#define MAYA_CXX_VERSION _MSVC_LANG
#ifdef _DEBUG // MSVC Debug
#define MAYA_DEBUG 1
#endif
#elif defined(__GNUC__) || defined(__clang__) // gcc or clang
#define MAYA_CXX_VERSION __cplusplus
#ifdef __OPTIMIZE__ // GCC or clang Debug
#define MAYA_DEBUG 1
#endif
#else // otherwise
#error unknown compiler detected, only MSVC, GCC and CLANG are supported
#endif

#if MAYA_CXX_VERSION < 202002L // version lower than C++20
#error C++20 is the minimum version requried, try compile with /std:c++20 or -std=c++20
#endif

#define MAYA_LOG(...) std::cout << "\033[0m[\033[91mMayaEngine\033[0m] " << __VA_ARGS__ << '\n';

// Indicate this identifiers should be implemented by the client
#define MAYA_CLIENT_IMPLEMENTATION

// Indicate this function or functions inside this class is thread-safe and can be called concurrently
#define MAYA_THREAD_SAFE

#include <string>
#include <string_view>
#include <cmath>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <type_traits>
#include <concepts>
#include <cmath>
#include <stdexcept>
#include <memory>