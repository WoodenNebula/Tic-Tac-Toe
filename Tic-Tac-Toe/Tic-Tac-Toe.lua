project "Tic-Tac-Toe"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir("%{wks.location}/../build/bin/" .. "%{wks.outputdir}" .. "/%{prj.name}")
    objdir("%{wks.location}/../build/obj/" .. "%{wks.outputdir}" .. "/%{prj.name}")

    prebuildmessage ("---- Building Dependencies-GLFW ----")

    prebuildcommands {
        "cmake -S %{wks.location}/dependencies/GLFW/ -B %{wks.location}/../build/GLFW/ && make -C %{wks.location}/../build/GLFW/"
    }
-- postbuildmessage ("Building Dependencies (GLFW) Complete")

    includedirs {
        "src",
        "dependencies/glad/include",
        "dependencies/GLFW/include"
    }

    libdirs {
        "../build/GLFW/src"
    }

    files { "src/**.h", "src/**.cpp" }

    filter "system:linux"
        targetname("Tic-Tac-Toe.out")

        links {
            "GL",
            "X11",
            "glad",
            "glfw3"
        }
    filter {}
