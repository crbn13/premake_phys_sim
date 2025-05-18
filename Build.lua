-- premake5.lua
workspace "phys_sim"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "{cfg.buildcfg}"

group "Core"
	include "Core/Build-Core.lua"
group "App"

include "App/Build-App.lua"
