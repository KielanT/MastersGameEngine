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
    IncludeDir["SDL"] = "Engine/external/SDL/include"
    IncludeDir["entt"] = "Engine/external/entt/include"
    IncludeDir["stbImage"] = "Engine/external/stb_image"
    IncludeDir["yaml_cpp"] = "Engine/external/yaml-cpp/include"
    IncludeDir["PhysX"] = "Engine/external/physx/physx/include"

    LibDir = {}
    LibDir["DirectXTK"] = "Engine/external/DirectXTK/%{cfg.buildcfg}"
    LibDir["assimp"] = "Engine/external/assimp/lib/%{cfg.buildcfg}"
    LibDir["SDL"] = "Engine/external/SDL/VisualC/x64/%{cfg.buildcfg}"
    LibDir["yaml_cpp"] = "Engine/external/yaml-cpp/lib/%{cfg.buildcfg}"
    LibDir["PhysX"] = "Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}"

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
        "%{prj.name}/external/imgui/backends/imgui_impl_sdl.h",
		"%{prj.name}/external/imgui/backends/imgui_impl_sdl.cpp",
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
        "%{IncludeDir.SDL}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.stbImage}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.PhysX}",
    }

    libdirs
    {
        "%{LibDir.DirectXTK}",
        "%{LibDir.assimp}",
        "%{LibDir.SDL}",
        "%{LibDir.yaml_cpp}",
        "%{LibDir.PhysX}",
    }

    links
    {
        "d3d11.lib",
		"DirectXTK.lib",
		"d3dcompiler.lib",
        "assimp-vc143-mt.lib",
        "ImGui",
        "yaml-cpp.lib",
        "SDL2.lib",

        --> PhysX 5.1
        "PhysX_64.lib",
        "PhysXCommon_64.lib",
        "PhysXCooking_64.lib",
        "PhysXFoundation_64.lib",
        "PhysXPvdSDK_static_64.lib",
        "PhysXExtensions_static_64.lib",
    }

    files("Engine/source/Engine/Renderer/Shaders/HLSL/*.hlsl")
	files("Engine/source/Engine/Renderer/Shaders/HLSL/*.hlsli")
	shadermodel("5.0")
	
    local shader_dir = "../Engine/Shaders/"

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
        defines "NDEBUG"
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
        "%{IncludeDir.DirectXTK}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackends}",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.PhysX}",
    }
    libdirs
    {
        "%{LibDir.DirectXTK}",
        "%{LibDir.assimp}",
        "%{LibDir.PhysX}",
    }

    links
    {
        "Engine",
        "ImGui",
        "d3d11.lib",
        "assimp-vc143-mt.lib",
        
        --> PhysX 5.1
        "PhysX_64.lib",
        "PhysXCommon_64.lib",
        "PhysXCooking_64.lib",
        "PhysXFoundation_64.lib",
        "PhysXPvdSDK_static_64.lib",
        "PhysXExtensions_static_64.lib",
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

    defines
    {
        "E_PLATFORM_WINDOWS",
        "E_EDITOR"
    }

    postbuildcommands 
    {
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysXFoundation_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysX_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysXCommon_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysXGpu_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/Shaders/*.cso ../Editor/Shaders",
    }

    filter "configurations:Debug"
        defines "E_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "E_RELEASE"
        defines "NDEBUG"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        defines "E_DISTRIBUTION"
        defines "NDEBUG"
        runtime "Release"
        optimize "On"

project "Game"
    location "Game"
    kind "WindowedApp"
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
        "%{IncludeDir.DirectXTK}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuiBackends}",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.PhysX}",
    }
    libdirs
    {
        "%{LibDir.DirectXTK}",
        "%{LibDir.assimp}",
        "%{LibDir.PhysX}",
    }
    
    links
    {
        "Engine",
        "ImGui",
        "d3d11.lib",
        "assimp-vc143-mt.lib",
        
        --> PhysX 5.1
        "PhysX_64.lib",
        "PhysXCommon_64.lib",
        "PhysXCooking_64.lib",
        "PhysXFoundation_64.lib",
        "PhysXPvdSDK_static_64.lib",
        "PhysXExtensions_static_64.lib",
    }
    
    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"
    
    defines
    {
        "E_PLATFORM_WINDOWS"
    }

    postbuildcommands 
    {
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysXFoundation_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysX_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysXCommon_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/external/physx/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}/PhysXGpu_64.dll %{cfg.targetdir}",
        "{COPY} ../Engine/Shaders/*.cso ../Game/Shaders",
    }
    
   

    filter "configurations:Debug"
        defines "E_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "E_RELEASE"
        defines "NDEBUG"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Distribution"
        defines "E_DISTRIBUTION"
        defines "NDEBUG"
        runtime "Release"
        optimize "On"