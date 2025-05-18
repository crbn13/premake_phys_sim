-- premake5.lua
workspace "phys_sim"
   architecture "x64"
   configurations { "debug", "delease", "dist" }
   startproject "app"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.buildcfg}"

group "core"
	include "core/Build-Core.lua"
group "app"

include "app/Build-App.lua"
