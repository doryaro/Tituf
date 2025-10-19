#pragma once
#include <memory>
#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
namespace Tituf
{
#pragma warning(push)
#pragma warning(disable:4251)
	class TITUF_API Log
	{
	public:  
		Log();  
		virtual ~Log();

	static void Init();

	inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:	
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;	

	};
#pragma warning(pop)

}

// Core log macros
#define TF_CORE_TRACE(...)    ::Tituf::Log::GetCoreLogger()->trace(__VA_ARGS__)	
#define TF_CORE_INFO(...)     ::Tituf::Log::GetCoreLogger()->info(__VA_ARGS__)	
#define TF_CORE_WARN(...)     ::Tituf::Log::GetCoreLogger()->warn(__VA_ARGS__)	
#define TF_CORE_ERROR(...)    ::Tituf::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TF_CORE_FATAL(...)    ::Tituf::Log::GetCoreLogger()->fatal(__VA_ARGS__)	
  
// Client log macros	
#define TF_Client_TRACE(...)    ::Tituf::Log::GetClientLogger()->trace(__VA_ARGS__)	
#define TF_Client_INFO(...)     ::Tituf::Log::GetClientLogger()->info(__VA_ARGS__)	
#define TF_Client_WARN(...)     ::Tituf::Log::GetClientLogger()->warn(__VA_ARGS__)	
#define TF_Client_ERROR(...)    ::Tituf::Log::GetClientLogger()->error(__VA_ARGS__)
#define TF_Client_FATAL(...)    ::Tituf::Log::GetClientLogger()->fatal(__VA_ARGS__)	





