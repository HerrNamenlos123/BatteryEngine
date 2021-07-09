
-- Retrieve the project name
newoption { trigger = "projectname", description = "Name of the generated project" }
local projectName = _OPTIONS["projectname"]
if projectName == nil then print("The project name was not specified! --projectname=YourApplication") end

-- Main Solution
workspace (projectName)
    configurations { "ALL" }

    platforms { "x64" }
    defaultplatform "x64"
    startproject "INSTALL_ALL"

-- Utility project for building and installing everything
project "INSTALL_ALL"
    kind "Utility"
    basedir "build"

    postbuildcommands { 
        "SETX BATTERY_ENGINE_INCLUDE_DIRECTORY $(ProjectDir)../include/;$(ProjectDir)../modules/imgui/backends/;$(ProjectDir)../modules/imgui/;$(ProjectDir)../modules/;$(ProjectDir)../modules/implot/;$(ProjectDir)../modules/spdlog/include/;$(ProjectDir)../modules/clip/;$(ProjectDir)../packages/Allegro.5.2.7/build/native/include",

        "SETX BATTERY_ENGINE_DEBUG_LINK_FILES BatteryEngine-d.lib;allegro_monolith-static.lib;freetype.lib;jpeg.lib;libpng16.lib;webp.lib;zlib.lib;opengl32.lib;winmm.lib;shlwapi",   -- .lib will be appended to the end by the client premake script
        "SETX BATTERY_ENGINE_RELEASE_LINK_FILES BatteryEngine.lib;allegro_monolith-static.lib;freetype.lib;jpeg.lib;libpng16.lib;webp.lib;zlib.lib;opengl32.lib;winmm.lib;shlwapi",   -- .lib will be appended to the end by the client premake script
        "SETX BATTERY_ENGINE_DEBUG_LINK_DIRS $(ProjectDir)../bin/;$(ProjectDir)../packages/Allegro.5.2.7/build/native/v142/x64/lib/;$(ProjectDir)../packages/AllegroDeps.1.12.0/build/native/v142/x64/deps/lib",
        "SETX BATTERY_ENGINE_RELEASE_LINK_DIRS $(ProjectDir)../bin/;$(ProjectDir)../packages/Allegro.5.2.7/build/native/v142/x64/lib/;$(ProjectDir)../packages/AllegroDeps.1.12.0/build/native/v142/x64/deps/lib"
    }
    
    dependson { projectName .. "-Debug", projectName .. "-Release" }


-- Debug version of the framework
project (projectName .. "-Debug")
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    location "build/Debug"
    targetname (projectName .. "-d")

    defines { "DEBUG", "_DEBUG", "ALLEGRO_STATICLINK" }
    runtime "Debug"
    symbols "On"
    system "Windows"
    architecture "x86_64"
    
    includedirs ({ 
        _SCRIPT_DIR .. "/include", 
        _SCRIPT_DIR .. "/modules", 
        _SCRIPT_DIR .. "/modules/imgui",
        _SCRIPT_DIR .. "/modules/imgui/backends",
        _SCRIPT_DIR .. "/modules/implot",
        _SCRIPT_DIR .. "/modules/spdlog/include",
        _SCRIPT_DIR .. "/modules/clip",
        _SCRIPT_DIR .. "/packages/Allegro.5.2.7/build/native/include"
    })
    
    files ({ _SCRIPT_DIR .. "/include/**", _SCRIPT_DIR .. "/src/**" })
    files ({ _SCRIPT_DIR .. "/modules/clip/clip.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/clip_win.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/image.cpp" })
    targetdir (_SCRIPT_DIR .. "/bin")

    pchheader "Battery/pch.h"
    pchsource "src/pch.cpp"
    filter { "files:include/glm/detail/glm.cpp or files:modules/**.cpp" }
        flags { 'NoPCH' }
    filter {}

    filter { "files:modules/clip/clip_win.cpp" }    -- Ignore signed/unsigned warning from 'clip' library 
        disablewarnings { "4018", "4267" }
    filter {}

    -- Organize the files in the Visual Studio project view
    --makeVPaths(_SCRIPT_DIR .. "/include")
    --makeVPaths(_SCRIPT_DIR .. "/src")


-- Release version of the framework
project (projectName .. "-Release")
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    location "build/Release"
    targetname (projectName)

    defines { "NDEBUG", "ALLEGRO_STATICLINK" }
    runtime "Release"
    optimize "On"
    system "Windows"
    architecture "x86_64"
    
    includedirs ({ 
        _SCRIPT_DIR .. "/include", 
        _SCRIPT_DIR .. "/modules", 
        _SCRIPT_DIR .. "/modules/imgui",
        _SCRIPT_DIR .. "/modules/imgui/backends",
        _SCRIPT_DIR .. "/modules/implot",
        _SCRIPT_DIR .. "/modules/spdlog/include",
        _SCRIPT_DIR .. "/modules/clip",
        _SCRIPT_DIR .. "/packages/Allegro.5.2.7/build/native/include"
    })
    
    files ({ _SCRIPT_DIR .. "/include/**", _SCRIPT_DIR .. "/src/**" })
    files ({ _SCRIPT_DIR .. "/modules/clip/clip.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/clip_win.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/image.cpp" })
    targetdir (_SCRIPT_DIR .. "/bin")

    pchheader "Battery/pch.h"
    pchsource "src/pch.cpp"
    filter { "files:include/glm/detail/glm.cpp or files:modules/**.cpp" }
        flags { 'NoPCH' }
    filter {}

    filter { "files:modules/clip/clip_win.cpp" }    -- Ignore signed/unsigned warning from 'clip' library 
        disablewarnings { "4018", "4267" }
    filter {}

    -- Organize the files in the Visual Studio project view
    --makeVPaths(_SCRIPT_DIR .. "/include")
    --makeVPaths(_SCRIPT_DIR .. "/src")
