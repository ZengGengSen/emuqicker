#pragma once

#include <platform/intrinsics.hpp>
#include <platform/stdint.hpp>

namespace Math {
  static const long double e  = 2.71828182845904523536;
  static const long double Pi = 3.14159265358979323846;
}

#if defined(PLATFORM_WINDOWS)
  #if defined(NALL_HEADER_ONLY)
    #include <nall/windows/windows.hpp>
  #endif
  #include <direct.h>
  #include <io.h>
  #include <wchar.h>
  #include <sys/utime.h>
  // #include <nall/windows/utf8.hpp>
#endif

#include <atomic>
#include <limits>
#include <mutex>
#include <condition_variable>
#include <utility>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <fenv.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#if !defined(PLATFORM_WINDOWS)
  #include <dlfcn.h>
  #include <unistd.h>
  #include <utime.h>
  #include <pwd.h>
  #include <grp.h>
  #include <sys/mman.h>
  #include <sys/socket.h>
  #include <sys/wait.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <poll.h>
#endif

#if defined(ARCHITECTURE_X86) || defined(ARCHITECTURE_AMD64)
  #include <immintrin.h>
  #undef _serialize
#endif

#if !defined(__has_builtin)
  #define __has_builtin(x) 0
#endif

#if defined(PLATFORM_WINDOWS)
  #define dllexport __declspec(dllexport)
  #define MSG_NOSIGNAL 0
  #define PATH_MAX 260

  typedef void* HANDLE;
#else
  #define dllexport
#endif

#undef bswap16
#undef bswap32
#undef bswap64
#undef bswap128
#undef likely
#undef unlikely
#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
  #define bswap16(value) __builtin_bswap16(value)
  #define bswap32(value) __builtin_bswap32(value)
  #define bswap64(value) __builtin_bswap64(value)
  #if defined(__SIZEOF_INT128__)
  inline auto bswap128(u128 value) -> u128 {
    #if defined(__SSSE3__)
    static const __m128i shuffle = _mm_setr_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    return reinterpret_cast<u128>(_mm_shuffle_epi8(reinterpret_cast<__m128i>(value), shuffle));
    #else
    return (u128)__builtin_bswap64(value) << 64 | __builtin_bswap64(value >> 64);
    #endif
  }
  #endif
  #define likely(expression) __builtin_expect(bool(expression), true)
  #define unlikely(expression) __builtin_expect(bool(expression), false)
#elif defined(COMPILER_MICROSOFT)
  #define bswap16(value) _byteswap_ushort(value)
  #define bswap32(value) _byteswap_ulong(value)
  #define bswap64(value) _byteswap_uint64(value)
  #define likely(expression) expression
  #define unlikely(expression) expression
#else
  inline auto bswap16(u16 value) -> u16 {
    return value << 8 | value >> 8;
  }
  inline auto bswap32(u32 value) -> u32 {
    return (u32)bswap16(value) << 16 | bswap16(value >> 16);
  }
  inline auto bswap64(u64 value) -> u64 {
    return (u64)bswap32(value) << 32 | bswap32(value >> 32);
  }
  #if defined(__SIZEOF_INT128__)
  inline auto bswap128(u128 value) -> u128 {
    return (u128)bswap64(value) << 64 | bswap64(value >> 64);
  }
  #endif
  #define likely(expression) expression
  #define unlikely(expression) expression
#endif

#if defined(PLATFORM_MACOS) && !defined(MSG_NOSIGNAL)
  #define MSG_NOSIGNAL 0
#endif

#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
  #define no_optimize __attribute__((optnone))
  #define noinline __attribute__((noinline))
  #define alwaysinline inline __attribute__((always_inline))
#elif defined(COMPILER_MICROSOFT)
  #define no_optimize
  #define noinline __declspec(noinline)
  #define alwaysinline inline __forceinline
#else
  #define no_optimize
  #define noinline
  #define alwaysinline inline
#endif

#if defined(ARCHITECTURE_X86)
  #if defined(__clang__) || defined(__GNUC__)
    #define fastcall __attribute__((fastcall))
  #elif defined(_MSC_VER)
    #define fastcall __fastcall
  #else
    #define fastcall
  #endif
#else
  #define fastcall
#endif

//P0627: [[unreachable]] -- impossible to simulate with identical syntax, must omit brackets ...
#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
  #define unreachable __builtin_unreachable()
#elif defined(COMPILER_MICROSOFT)
  #define unreachable __assume(0)
#else
  #define unreachable throw
#endif

#if defined(COMPILER_GCC)
  #undef _serialize
#endif
