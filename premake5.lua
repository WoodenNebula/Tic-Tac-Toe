workspace "Tic-Tac-Toe"
   configurations { "Debug", "Release" }
   location "build"

project "Tic-Tac-Toe"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "src/**.h", "src/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"