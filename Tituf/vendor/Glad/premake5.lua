project "Glad"
	kind "StaticLib"	
	language "C"
	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")	
	
	files
	{
		"include/GLAD/glad.h",
		"include/KHR/khrplatform.h",    
		"src/glad.c"
	}			

	includedirs 
	{	  
		"include"
	}

	-- common Windows-specific settings (applies to all configurations)
	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter {"system:windows", "configurations:Release"}
		buildoptions "/MT"
