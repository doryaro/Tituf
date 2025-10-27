workspace "Tituf"
	architecture "x64"
	startproject "Sandbox" 
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
	
	pchheader "tfpch.h"
	pchsource "Tituf/src/tfpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}			

	libdirs
	{
	    "%{wks.location}/vendor/glew/lib"  -- folder containing .lib files
	}
	links
	{
		"opengl32", 
		"glew32s"   
	}

	-- include directories
	includedirs 
	{	  
		"%{wks.location}/vendor/glew/include",
		"%{prj.name}/src",    
		"%{prj.name}/vendor/spdlog/include"
	}

	defines
	{
		"GLEW_STATIC"
	}

	-- Windows-specific settings
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }
		defines
		{
			"TF_PLATFORM_WINDOWS",
			"TF_BUILD_DLL"
		}

		-- create output folder and copy DLLs
		postbuildcommands
		{
			"{MKDIR} ../bin/" .. outputdir .. "/Sandbox",
			"powershell -Command \"Copy-Item -Path '%{cfg.buildtarget.abspath}' -Destination '../bin/" .. outputdir .. "/Sandbox' -Force\"",
		}
		     
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
		"Tituf/src",
		"%{wks.location}/vendor/glew/include",
	}
	libdirs
	{
	    "%{wks.location}/vendor/glew/lib"  -- folder containing .lib files
	}

	links
	{
		"Tituf",
		"opengl32", 
		"glew32s"  
	}  

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }
		defines 
		{ 
			"TF_PLATFORM_WINDOWS",
			"GLEW_STATIC"
		}

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
