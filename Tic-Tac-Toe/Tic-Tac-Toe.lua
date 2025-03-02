project "Tic-Tac-Toe"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir("%{wks.location}/build/bin/" .. "%{wks.outputdir}" .. "/%{prj.name}")
    objdir("%{wks.location}/build/obj/" .. "%{wks.outputdir}" .. "/%{prj.name}")

    includedirs {
        "src",
    }

    libdirs {

    }

    files { "src/**.h", "src/**.cpp" }

    filter "system:linux"
        targetname("Tic-Tac-Toe.out")

        links {
            "GL",
            "X11",
            "glut",
            -- "glu"
        }
    filter {}
