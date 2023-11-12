

project "engine"
    kind "StaticLib"
    location "engine"
    language "C++"
	cdialect "C99"
    cppdialect "C++latest"
	targetdir "_bin/%{cfg.buildcfg}"

    includedirs {"engine/include", "raylib-master/src"}
    files {"engine/**.h","engine/*.hpp", "engine/**.cpp"}
	link_raylib()
		
