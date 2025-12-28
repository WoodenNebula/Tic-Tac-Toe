project "Engine"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "off"

    -- location "%{wks.location} " .. "/" .. "{prj.name}"

    targetdir("%{wks.location}/build/bin/" .. "%{wks.outputdir}" .. "/%{prj.name}")
    objdir("%{wks.location}/build/obj/" .. "%{wks.outputdir}" .. "/%{prj.name}")

    
    dependson { "glad", "GLFW" }

    defines { "GLFW_INCLUDE_NONE" }

    includedirs {
        "Source",
        "dependencies/glad/include",
        "dependencies/GLFW/include",
        "dependencies/stb_image",
        "dependencies/glm",
    }

    
    files { 
        "Source/**.h", 
        "Source/**.cpp", 
        "dependencies/glm/**.hpp",
        "dependencies/glm/**.inl",

        "dependencies/stb_image/*.cpp",
        "dependencies/stb_image/*.h",
    }
    
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