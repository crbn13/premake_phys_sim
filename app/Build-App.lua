project "app"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "binaries/%{cfg.buildcfg}"
   staticruntime "off"
   toolset "clang"

   files { "source/**.h", "source/**.*pp" }

   includedirs
   {
      "source",

	  -- Include Core
	  "../core/source"
   }

   links
   {
      "core"
   }

   targetdir ("../binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
