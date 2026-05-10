project "Tic-Tac-Toe"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/build/obj/" .. outputdir .. "/%{prj.name}")

    defines { "GLFW_INCLUDE_NONE" }

    dependson { "Engine" }

    includedirs {
        "src",
        "../Engine/Source/Public",
        "../Engine/dependencies/glad/include",
        "../Engine/dependencies/GLFW/include",
        "../Engine/dependencies/glm",
        "../Engine/dependencies/stb_image",

    }


    files { "src/**.h", "src/**.cpp" }

    files { "res/**" }

    links
    {
        "Engine",
        "glad",
        "glfw"
    }

    filter "system:windows"
        systemversion "latest"
        targetname("Tic-Tac-Toe")
    filter {}

    filter "system:linux"
        targetname("Tic-Tac-Toe.out")
    filter {}
