
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

    local _includedirs = "$(ProjectDir)../include/;"
    _includedirs = _includedirs .. "$(ProjectDir)../modules/imgui/backends/;"
    _includedirs = _includedirs .. "$(ProjectDir)../modules/imgui/;"
    _includedirs = _includedirs .. "$(ProjectDir)../modules/;"
    _includedirs = _includedirs .. "$(ProjectDir)../modules/implot/;"
    _includedirs = _includedirs .. "$(ProjectDir)../modules/spdlog/include/;"
    _includedirs = _includedirs .. "$(ProjectDir)../modules/serial/include/;"
    _includedirs = _includedirs .. "$(ProjectDir)../modules/clip/;"
    _includedirs = _includedirs .. "$(ProjectDir)../packages/Allegro.5.2.7/build/native/include"

    postbuildcommands { 
        "SETX BATTERY_ENGINE_INCLUDE_DIRECTORY " .. _includedirs,

        "SETX BATTERY_ENGINE_DEBUG_LINK_FILES BatteryEngine-d.lib;allegro_monolith-static.lib;freetype.lib;jpeg.lib;libpng16.lib;webp.lib;zlib.lib;opengl32.lib;winmm.lib;setupapi.lib;shlwapi",   -- .lib will be appended to the end by the client premake script
        "SETX BATTERY_ENGINE_RELEASE_LINK_FILES BatteryEngine.lib;allegro_monolith-static.lib;freetype.lib;jpeg.lib;libpng16.lib;webp.lib;zlib.lib;opengl32.lib;winmm.lib;setupapi.lib;shlwapi",   -- .lib will be appended to the end by the client premake script
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
    targetdir (_SCRIPT_DIR .. "/bin")

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
        _SCRIPT_DIR .. "/modules/serial/include",
        _SCRIPT_DIR .. "/modules/clip",
        _SCRIPT_DIR .. "/packages/Allegro.5.2.7/build/native/include"
    })
    
    -- Main source files
    files ({ _SCRIPT_DIR .. "/include/**", _SCRIPT_DIR .. "/src/**" })

    -- Serial library
    files ({ _SCRIPT_DIR .. "/modules/serial/src/serial.cc" })
    files ({ _SCRIPT_DIR .. "/modules/serial/src/impl/win.cc" })
    files ({ _SCRIPT_DIR .. "/modules/serial/src/impl/list_ports/list_ports_win.cc" })

    -- Clip library
    files ({ _SCRIPT_DIR .. "/modules/clip/clip.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/clip_win.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/image.cpp" })

    -- ImGui/ImPlot library
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_demo.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_draw.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_tables.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_widgets.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/backends/imgui_impl_allegro5.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/implot/implot.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/implot/implot_demo.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/implot/implot_items.cpp" })

    -- Precompiled headers
    pchheader "Battery/pch.h"
    pchsource "src/pch.cpp"
    filter { "files:include/glm/detail/glm.cpp or files:modules/**" }
        flags { 'NoPCH' }
    filter {}

    filter { "files:modules/clip/clip_win.cpp" }    -- Ignore signed/unsigned warning from 'clip' library 
        disablewarnings { "4018", "4267" }
    filter { "files:modules/serial/src/impl/win.cc" }    -- Ignore possible loss of data warning from 'serial' library 
        disablewarnings { "4244" }
    filter { "files:modules/serial/src/serial.cc" }    -- Ignore unreferenced local variable warning from 'serial' library 
        disablewarnings { "4101" }
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
    targetdir (_SCRIPT_DIR .. "/bin")

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
        _SCRIPT_DIR .. "/modules/serial/include",
        _SCRIPT_DIR .. "/modules/clip",
        _SCRIPT_DIR .. "/packages/Allegro.5.2.7/build/native/include"
    })
    
    -- Main source files
    files ({ _SCRIPT_DIR .. "/include/**", _SCRIPT_DIR .. "/src/**" })

    -- Serial library
    files ({ _SCRIPT_DIR .. "/modules/serial/src/serial.cc" })
    files ({ _SCRIPT_DIR .. "/modules/serial/src/impl/win.cc" })
    files ({ _SCRIPT_DIR .. "/modules/serial/src/impl/list_ports/list_ports_win.cc" })

    -- Clip library
    files ({ _SCRIPT_DIR .. "/modules/clip/clip.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/clip_win.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/clip/image.cpp" })

    -- ImGui/ImPlot library
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_demo.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_draw.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_tables.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/imgui_widgets.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/imgui/backends/imgui_impl_allegro5.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/implot/implot.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/implot/implot_demo.cpp" })
    files ({ _SCRIPT_DIR .. "/modules/implot/implot_items.cpp" })

    -- Precompiled headers
    pchheader "Battery/pch.h"
    pchsource "src/pch.cpp"
    filter { "files:include/glm/detail/glm.cpp or files:modules/**" }
        flags { 'NoPCH' }
    filter {}

    filter { "files:modules/clip/clip_win.cpp" }    -- Ignore signed/unsigned warning from 'clip' library 
        disablewarnings { "4018", "4267" }
    filter { "files:modules/serial/src/impl/win.cc" }    -- Ignore possible loss of data warning from 'serial' library 
        disablewarnings { "4244" }
    filter { "files:modules/serial/src/serial.cc" }    -- Ignore unreferenced local variable warning from 'serial' library 
        disablewarnings { "4101" }
    filter {}

    -- Organize the files in the Visual Studio project view
    --makeVPaths(_SCRIPT_DIR .. "/include")
    --makeVPaths(_SCRIPT_DIR .. "/src")
