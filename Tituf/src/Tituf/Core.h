#pragma once

#ifdef TF_PLATFORM_WINDOWS
	#ifdef TF_BUILD_DLL 
		#define TITUF_API __declspec(dllexport)
	#else	
		#define TITUF_API __declspec(dllimport)
	#endif 
#else
	#error Tituf only supports Windows!	
#endif // TITUF_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
