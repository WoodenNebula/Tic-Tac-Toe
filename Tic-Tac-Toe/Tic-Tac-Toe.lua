project "Tic-Tac-Toe"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir("%{wks.location}/build/bin/" .. "%{wks.outputdir}" .. "/%{prj.name}")
    objdir("%{wks.location}/build/obj/" .. "%{wks.outputdir}" .. "/%{prj.name}")

    
    dependson { "glad", "GLFW" }

    includedirs {
        "src",
        "dependencies/glad/include",
        "dependencies/GLFW/include"
    }

    
    files { "src/**.h", "src/**.cpp" }
    
    filter "system:windows"
    targetname("Tic-Tac-Toe.exe")
    
    links {
        "opengl32",
        "glad",
        "GLFW"
    }
    filter {}
    
    filter "system:linux"
        targetname("Tic-Tac-Toe.out")
    
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
