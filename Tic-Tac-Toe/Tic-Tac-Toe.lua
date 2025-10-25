project "Tic-Tac-Toe"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir("%{wks.location}/build/bin/" .. "%{wks.outputdir}" .. "/%{prj.name}")
    objdir("%{wks.location}/build/obj/" .. "%{wks.outputdir}" .. "/%{prj.name}")

    
    dependson { "Engine" }

    includedirs {
        "src",
        "../Engine/Source",
        "../Engine/dependencies/glad/include",
        "../Engine/dependencies/GLFW/include",
    }

    
    files { "src/**.h", "src/**.cpp" }
    
    links 
    {
        "Engine",
    }

    filter "system:windows"
        targetname("Tic-Tac-Toe.exe")
    filter {}
    
    filter "system:linux"
        targetname("Tic-Tac-Toe.out")
    filter {}
