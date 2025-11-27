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
	kind "StaticLib"	
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
		"%{prj.name}/config/*",  -- <-- include Config.cpp here
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl",

		-- stb_image
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",

		-- ImGui core + backend files
        "%{wks.location}/vendor/imgui/*.cpp",
        "%{wks.location}/vendor/imgui/backends/**.cpp",
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
        "%{wks.location}/vendor/imgui",
        "%{wks.location}/vendor/imgui/backends",
		"%{prj.name}/src",    
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/config"
	}

	defines
	{
		"GLEW_STATIC",
		"_CRT_SECURE_NO_WARNINGS",
        'PROJECT_ROOT="' .. os.getcwd() .. '"'
	}

	-- Windows-specific settings
	filter "system:windows"
		staticruntime "on"
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
		runtime "Debug"
		symbols "on" 

	filter "configurations:Release"
		defines "TF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TF_DIST"
		runtime "Release"
		optimize "on"	


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
		"%{prj.name}/src/**.cpp"
	}	  

	includedirs
	{	
		"Tituf/vendor/spdlog/include",
		"Tituf/vendor/glm",
		"Tituf/src",
		"%{wks.location}/vendor/glew/include",
		"%{wks.location}/vendor/imgui",
		"Tituf/config"
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
		staticruntime "on"
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
		symbols "on"

	filter "configurations:Release"
		defines "TF_RELEASE"
		optimize "on"	

	filter "configurations:Dist"	
		defines "TF_DIST"	
		optimize "on"
