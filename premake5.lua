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
    IncludeDir["spdlog"] = "Engine/external/spdlog/include"
    IncludeDir["assimp"] = "Engine/external/assimp/include"
    IncludeDir["ImGui"] = "Engine/external/imgui"
    IncludeDir["ImGuiBackends"] = "Engine/external/imgui/backends"

    LibDir = {}
    LibDir["DirectXTK"] = "Engine/external/DirectXTK/%{cfg.buildcfg}"
    LibDir["assimp"] = "Engine/external/assimp/lib/%{cfg.buildcfg}"

    include "Engine/external/imgui"

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
        "%{prj.name}/external/imgui/backends/imgui_impl_win32.h",
		"%{prj.name}/external/imgui/backends/imgui_impl_win32.cpp",
		"%{prj.name}/external/imgui/backends/imgui_impl_dx11.h",
		"%{prj.name}/external/imgui/backends/imgui_impl_dx11.cpp",
    }

    includedirs
    {
        "%{prj.name}/source",
        "%{IncludeDir.DirectXTK}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackends}",
    }

    libdirs
    {
        "%{LibDir.DirectXTK}",
        "%{LibDir.assimp}",
    }

    links
    {
        "d3d11.lib",
		"DirectXTK.lib",
		"d3dcompiler.lib",
        "assimp-vc143-mt.lib",
        "ImGui",
    }

    files("Engine/source/Engine/Renderer/Shaders/HLSL/*.hlsl")
	files("Engine/source/Engine/Renderer/Shaders/HLSL/*.hlsli")
	shadermodel("5.0")
	
	local shader_dir = "../Editor/Shaders/"

	filter("files:**.hlsl")
      flags("ExcludeFromBuild")
      shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")

   filter("files:**_ps.hlsl")
      removeflags("ExcludeFromBuild")
      shadertype("Pixel")

   filter("files:**_vs.hlsl")
      removeflags("ExcludeFromBuild")
      shadertype("Vertex")

	 shaderoptions({"/WX"})

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
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackends}",
    }

    links
    {
        "Engine",
        "ImGui",
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
