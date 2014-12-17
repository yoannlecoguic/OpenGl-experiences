
dofile("../glsdk/links.lua")

local usedlibs = {"freeglut"}

-- A solution contains projects, and defines the available configurations
solution "Test"
configurations { "Debug", "Release" }

-- A project defines one build target
	project "Test"
		kind "ConsoleApp"
		language "C++"
		files { "headers/**.h", "main.cpp", "c_scripts/**.c", "cpp_scripts/**.cpp"}

	links {"GL", "GLEW", "X11"}
	UseLibs(usedlibs)

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
