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


#ifdef TF_DEBUG
	#define TF_ASSERT(x)        { if (!(x)) { TF_ERROR("Assertion Failed: {0}", #x); __debugbreak(); } }
	#define TF_CORE_ASSERT(x)   { if (!(x)) { TF_CORE_ERROR("Assertion Failed: {0}", #x); __debugbreak(); } }
#else
	#define TF_ASSERT(x)
	#define TF_CORE_ASSERT(x)
#endif
 


#define BIT(x) (1 << x)
