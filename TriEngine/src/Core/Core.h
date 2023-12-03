#pragma once

#ifdef TRI_PLATFORM_WINDOWS
	#ifdef TRI_BUILD_DLL
		#define TRI_API __declspec(dllexport)
	#else
		#define TRI_API __declspec(dllimport)
	#endif	

#endif

#define BIT(x) (1 << x)