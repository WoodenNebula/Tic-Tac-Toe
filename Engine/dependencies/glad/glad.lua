project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    warnings "off"

    targetdir ("%{wks.location}/build/bin/%{wks.outputdir}/%{prj.name}")
    objdir ("%{wks.location}/build/obj/%{wks.outputdir}/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter { "system:windows", "configurations:Debug-AS" }
        runtime "Debug"
        symbols "on"
        sanitize { "Address" }
        flags { "NoRuntimeChecks", "NoIncrementalLink" }
        systemversion "latest"


    filter "configurations:Release"
        runtime "Release"
        optimize "speed"

    filter "configurations:Dist"
        runtime "Release"
        optimize "speed"
        symbols "off"
