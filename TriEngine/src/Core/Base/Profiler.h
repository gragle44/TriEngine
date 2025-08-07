#pragma once

#ifdef TRACY_ENABLE
#define TRACY_ON_DEMAND
#include "tracy/Tracy.hpp"

#define TRI_PROFILE_TAG(y, x) ZoneText(x, strlen(x))
#define TRI_PROFILE_NAMED(name) ZoneScopedN(name)
#define TRI_PROFILE ZoneScoped
#define TRI_PROFILE_FRAME FrameMark

// #define TRI_PROFILE_GPU_CONTEXT TracyGpuContext
// #define TRI_PROFILE_GPU_COLLECT TracyGpuCollect
// #define TRI_PROFILE_GPU(name) TracyGpuZone(name)

#else
#define TRI_PROFILE_TAG(y, x) 
#define TRI_PROFILE_NAMED(name) 
#define TRI_PROFILE
#define TRI_PROFILE_FRAME

// #define TRI_PROFILE_GPU_CONTEXT
// #define TRI_PROFILE_GPU_COLLECT
// #define TRI_PROFILE_GPU

#endif