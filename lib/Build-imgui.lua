project "ImGui"
	kind "StaticLib"
	language "C++"
  staticruntime "off"

  targetdir ("../binaries/" .. OutputDir .. "/%{prj.name}")
  objdir ("../binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

  includedirs
  {
    "imgui"
  }

	files
	{
		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_internal.h",
		"imgui/imgui_tables.cpp",
		"imgui/imgui_widgets.cpp",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",
		"imgui/imgui_demo.cpp",
		"imgui/backends/imgui_impl_opengl3.cpp",
		"imgui/backends/imgui_impl_opengl3.h",
		"imgui/backends/imgui_impl_glfw.h",
		"imgui/backends/imgui_impl_glfw.cpp",

	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
    links "GL"

	filter "configurations:debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:release"
		runtime "Release"
		optimize "on"

    filter "configurations:dist"
		runtime "Release"
		optimize "on"
        symbols "off"
