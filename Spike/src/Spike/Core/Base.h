//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Spike/Core/PlatformDetection.h"
#include "Spike/Renderer/RendererAPISwitch.h"

#ifdef SPK_DEBUG
    #if defined(SPK_PLATFORM_WINDOWS)
        #define SPK_DEBUGBREAK() __debugbreak()
    #elif defined(SPK_PLATFORM_LINUX)
        #include<signal.h>
        #define SPK_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Spike doesn't support debugbreak on this platform!"
    #endif
    #define SPK_ENABLE_ASSERTS
#else
    #define SPK_DEBUGBREAK()
#endif

#ifdef SPK_ENABLE_ASSERTS
    #define SPK_CORE_ASSERT(x, ...) { if(!(x)) { SPK_CORE_LOG_ERROR("Assertion Failed: %s", __VA_ARGS__); SPK_DEBUGBREAK(); } }
    #define SPK_INTERNAL_ASSERT(x) {  SPK_CORE_LOG_CRITICAL(x); SPK_DEBUGBREAK(); }
#else
    #define SPK_CORE_ASSERT(x, ...)
    #define SPK_INTERNAL_ASSERT(...)
#endif

#define BIT(x) (1 << x)

#define SPK_BIND_EVENT_FN(fn) [this](auto&&... args)->decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define SPK_NAMESPACE_BEGIN namespace Spike {
#define SPK_NAMESPACE_END }

namespace Spike
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    /* [Spike] Engine TypeDefs & Usings[Spike] */
    typedef uint8_t byte;
    typedef std::string String;

    typedef void* RendererID;
    typedef uint32_t Uint;

    template<typename T>
    using Vector = std::vector<T>;
}
