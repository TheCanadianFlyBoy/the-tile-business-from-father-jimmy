------------------------------------------------ Solution
workspace "Pathfinding TestBed"
    configurations  { "Debug", "Release" }
    location        "build"
    startproject    "Game"

    filter "system:windows"
        platforms       { "x64" }
        characterset    "MBCS"

------------------------------------------------ Game Project
project "Pathfinder"
    location    "build"
    kind        "ConsoleApp"
    language    "C++"

    includedirs {
        "Source",
    }

    files {
        "Source/**.cpp",
        "Source/**.h",
        "premake5.lua",
        "GenerateBuildFiles.bat",
    }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
