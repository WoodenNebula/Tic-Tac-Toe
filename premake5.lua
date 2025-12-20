workspace "Tic-Tac-Toe"
    configurations { "Debug", "Release" }
    architecture "x64"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"
    filter{}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
    filter{}

    filter "system:windows"
        startproject "Tic-Tac-Toe"
        defines { "WINDOWS" }
        systemversion "latest"

        filter "action:vs2026"
            buildoptions { "/Zc:preprocessor" }
        filter{}
    filter{}

    filter "system:linux"
        defines { "LINUX" }
    filter{}


    outputdir = "%{cfg.architecture}-%{cfg.system}-%{cfg.buildcfg}"

    include "Engine/Engine.lua"
    include "Tic-Tac-Toe/Tic-Tac-Toe.lua"


newaction {
    trigger = "clean",
    description = "Remove all binaries and intermediate binaries, and vs files.",
    execute = function()
        print("==== Cleaning Up ====")
        -- os.chdir("Tic-Tac-Toe")

        os.rmdir("build")
        os.rmdir("./.vs")

        os.remove("**.sln")
        os.remove("**.slnx")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.**")
        os.remove("**Makefile")
        os.remove("**.make")
        print("==== Success ====\n")
    end
}
