project "Engine"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "off"

    -- location "%{wks.location} " .. "/" .. "{prj.name}"

    targetdir("%{wks.location}/build/bin/" .. "%{wks.outputdir}" .. "/%{prj.name}")
    objdir("%{wks.location}/build/obj/" .. "%{wks.outputdir}" .. "/%{prj.name}")

    
    dependson { "glad", "GLFW" }

    includedirs {
        "Source",
        "dependencies/glad/include",
        "dependencies/GLFW/include"
    }

    
    files { "Source/**.h", "Source/**.cpp" }
    
    filter "system:windows"
    -- targetname("Engine.dll")
    
        links {
            "opengl32",
            "glad",
            "GLFW"
        }
    filter {}
    
    filter "system:linux"
        -- targetname("Tic-Tac-Toe.out")
    
        libdirs {
            "%{wks.location}/build/bin/%{wks.outputdir}/GLFW/"
        }

        links {
            "GL",
            "X11",
            "glad",
            "GLFW"
        }
    filter {}

    include "dependencies/glad/glad.lua"
    include "dependencies/GLFW/glfw.lua"