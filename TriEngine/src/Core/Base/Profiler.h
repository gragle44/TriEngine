#pragma once

#ifdef TRACY_ENABLE
#define TRACY_ON_DEMAND
#include "tracy/Tracy.hpp"

#define TRI_PROFILE_TAG(y, x) ZoneText(x, strlen(x))
#define TRI_PROFILE_SECTION(name) ZoneScopedN(name)
#define TRI_PROFILE ZoneScoped
#define TRI_PROFILE_FRAME FrameMark

#else
	#define TRI_PROFILE_TAG(y, x) 
	#define TRI_PROFILE_SECTION(name) 
	#define TRI_PROFILE
	#define TRI_PROFILE_FRAME

#endif