#pragma once
#include "memory.h"

// CONFIG_PATH relative to the project root
#define CONFIG_PATH PROJECT_ROOT "/Tituf" "/config/config.ini"

#ifdef TF_PLATFORM_WINDOWS
#if TF_DYNAMIC_LINK
	#ifdef TF_BUILD_DLL 
		#define TITUF_API __declspec(dllexport)
	#else	
		#define TITUF_API __declspec(dllimport)
	#endif 
#else
	#define TITUF_API
#endif // TITUF_PLATFORM_WINDOWS
#else
	#error Tituf only supports Windows!	
#endif


#ifdef TF_DEBUG
	#define TF_ASSERT(x) do { if (!(x)) { TF_ERROR("Assertion Failed: {0}", #x); __debugbreak(); } } while(0)
	#define TF_CORE_ASSERT(x) do { if (!(x)) { TF_CORE_ERROR("Assertion Failed: {0}", #x); __debugbreak(); } } while(0)
	#define TF_CORE_ASSERT_INFO(x,y) do { if (!(x)) { TF_CORE_ERROR("Assertion Failed: {0}, Info: {1}", #x, y); __debugbreak(); } } while(0)
#else
	#define TF_ASSERT(x) do { (void)(x); } while(0)
	#define TF_CORE_ASSERT(x) do { (void)(x); } while(0)
	#define TF_CORE_ASSERT_INFO(x,y) do { (void)(x); (void)(y); } while(0)
#endif

 


#define BIT(x) (1 << x)
#define TF_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif



#pragma once

// Keyboard key codes
#define TF_KEY_UNKNOWN       -1
#define TF_KEY_SPACE         32
#define TF_KEY_APOSTROPHE    39  // '
#define TF_KEY_COMMA         44  // ,
#define TF_KEY_MINUS         45  // -
#define TF_KEY_PERIOD        46  // .
#define TF_KEY_SLASH         47  // /
#define TF_KEY_0             48
#define TF_KEY_1             49
#define TF_KEY_2             50
#define TF_KEY_3             51
#define TF_KEY_4             52
#define TF_KEY_5             53
#define TF_KEY_6             54
#define TF_KEY_7             55
#define TF_KEY_8             56
#define TF_KEY_9             57
#define TF_KEY_SEMICOLON     59  // ;
#define TF_KEY_EQUAL         61  // =
#define TF_KEY_A             65
#define TF_KEY_B             66
#define TF_KEY_C             67
#define TF_KEY_D             68
#define TF_KEY_E             69
#define TF_KEY_F             70
#define TF_KEY_G             71
#define TF_KEY_H             72
#define TF_KEY_I             73
#define TF_KEY_J             74
#define TF_KEY_K             75
#define TF_KEY_L             76
#define TF_KEY_M             77
#define TF_KEY_N             78
#define TF_KEY_O             79
#define TF_KEY_P             80
#define TF_KEY_Q             81
#define TF_KEY_R             82
#define TF_KEY_S             83
#define TF_KEY_T             84
#define TF_KEY_U             85
#define TF_KEY_V             86
#define TF_KEY_W             87
#define TF_KEY_X             88
#define TF_KEY_Y             89
#define TF_KEY_Z             90

// Function keys
#define TF_KEY_ESCAPE        256
#define TF_KEY_ENTER         257
#define TF_KEY_TAB           258
#define TF_KEY_BACKSPACE     259
#define TF_KEY_INSERT        260
#define TF_KEY_DELETE        261
#define TF_KEY_RIGHT         262
#define TF_KEY_LEFT          263
#define TF_KEY_DOWN          264
#define TF_KEY_UP            265
#define TF_KEY_PAGE_UP       266
#define TF_KEY_PAGE_DOWN     267
#define TF_KEY_HOME          268
#define TF_KEY_END           269

// Modifier keys
#define TF_KEY_LEFT_SHIFT    340
#define TF_KEY_LEFT_CONTROL  341
#define TF_KEY_LEFT_ALT      342
#define TF_KEY_LEFT_SUPER    343
#define TF_KEY_RIGHT_SHIFT   344
#define TF_KEY_RIGHT_CONTROL 345
#define TF_KEY_RIGHT_ALT     346
#define TF_KEY_RIGHT_SUPER   347



namespace Tituf
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}