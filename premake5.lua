workspace "MastersGameEngine" 
    architecture "x64"
    startproject "Editor"

    configurations
    {
        "Debug", -- Used for debugging
        "Release", -- Strips out the default debugging tools but leaves in the console log developed for any engine errors
        "Distribution" -- Main Mode for distributing games, stips all debugging tools
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {} 
    IncludeDir["DirectXTK"] = "Engine/external/DirectXTK"
    IncludeDir["glm"] = "Engine/external/glm"

    LibDir = {}
    LibDir["DirectXTK"] = "Engine/external/DirectXTK/%{cfg.buildcfg}"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "epch.h"
	pchsource "Engine/source/epch.cpp"

    files
    {
        "%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/source",
        "%{IncludeDir.DirectXTK}",
        "%{IncludeDir.glm}",
    }

    libdirs
    {
        "%{LibDir.DirectXTK}",
    }

    links
    {
        "d3d11.lib",
		"DirectXTK.lib",
		"d3dcompiler.lib",
    }

    -- Shader Code here

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

        defines
        {
            "E_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "E_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "E_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        defines "I_DISTRIBUTION"
        runtime "Release"
        optimize "On"

project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
	}

    includedirs
    {
        "Engine/source",
        "%{IncludeDir.glm}",
    }

    links
    {
        "Engine"
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

    defines
    {
        "E_PLATFORM_WINDOWS"
    }

    filter "configurations:Debug"
        defines "E_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "E_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        defines "E_DISTRIBUTION"
        runtime "Release"
        optimize "On"
