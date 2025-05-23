-- premake5.lua
workspace "phys_sim"
   architecture "x64"
   configurations { "debug", "release", "dist" }
   startproject "app"
   require "premake-ecc/ecc"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.buildcfg}"

group "deps"
  include "lib/Build-imgui.lua"
  include "lib/Build-glfw.lua"


group "core"
	include "core/Build-Core.lua"

group "app"
  include "app/Build-App.lua"
