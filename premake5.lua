workspace "Tituf"
	architecture "x64"
	startproject "Tituf"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Tituf"
	location "Tituf"
	kind "SharedLib"	
	language "C++"
	cppdialect "C++20"	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")	
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}			

	-- common include dirs for all configurations
	includedirs 
	{	
		"%{prj.name}/vendor/spdlog/include"
	}

	-- common Windows-specific settings (applies to all configurations)
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		-- Ensure MSVC compiles source files as UTF-8 for spdlog/fmt
		buildoptions { "/utf-8" }
		defines
		{
			"TF_PLATFORM_WINDOWS",
			"TF_BUILD_DLL",
		}
		-- create destination and copy the built DLL for all configs
		postbuildcommands
		{
			("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
			("powershell -Command \"Copy-Item -Path '%{cfg.buildtarget.abspath}' -Destination '../bin/" .. outputdir .. "/Sandbox' -Force\"")
		}

	-- clear filter so next filters don't get combined unexpectedly
	filter {}

	-- configuration-specific settings
	filter "configurations:Debug"
		defines "TF_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TF_RELEASE"
		optimize "On"	

	filter "configurations:Dist"	
		defines "TF_DIST"	
		optimize "On"	

-- Sandbox project kept similarly (only snippet shown here for clarity)
project "Sandbox"
	location "Sandbox"	
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
		
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}	  

	includedirs
	{	
		"Tituf/vendor/spdlog/include",
		"Tituf/src"
	}

	links
	{
		"Tituf"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }
		defines { "TF_PLATFORM_WINDOWS" }

	filter {}

	filter "configurations:Debug"
		defines "TF_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TF_RELEASE"
		optimize "On"	

	filter "configurations:Dist"	
		defines "TF_DIST"	
		optimize "On"