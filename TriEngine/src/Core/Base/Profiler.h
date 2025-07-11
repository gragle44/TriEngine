#pragma once

#ifdef TRACY_ENABLE
	#include "tracy/Tracy.hpp"

	#define TRI_PROFILE_TAG(y, x) ZoneText(x, strlen(x))
	#define TRI_PROFILE_SECTION(name) ZoneScopedN(name)
	#define TRI_PROFILE_FRAME FrameMark
	#define TRI_PROFILE_FRAME_NAMED(name) FrameMarkNamed(name)
	#define TRI_PROFILE_FRAME_START(name) FrameMarkStart(name)
	#define TRI_PROFILE_FRAME_END(name) FrameMarkEnd(name)
	#define TRI_PROFILE ZoneScoped

#else
	#define TRI_PROFILE_TAG(y, x) 
	#define TRI_PROFILE_SECTION(name) 
	#define TRI_PROFILE_FRAME
	#define TRI_PROFILE_FRAME_START(name)
	#define TRI_PROFILE_FRAME_END(name)
	#define TRI_PROFILE 

#endif