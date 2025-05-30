// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs,
// OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include <KHR/khrplatform.h>
#include <cstddef>
#include <future>
#include <iostream>

#include "core/Core.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <GL/gl.h>
#include <stdio.h>
#include <string>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of
// testing and compatibility with old VS compilers. To link with VS2010-era libraries, VS2015+
// requires linking with legacy_stdio_definitions.lib, which we do using this pragma. Your own
// project should not be affected, as you are likely to link with a newer binary of GLFW that is
// adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window
        = glfwCreateWindow(1920, 1080, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // lots of fps maby
    // glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple
    // fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the
    // font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those
    // errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture
    // when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below
    // will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher
    // quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to
    // write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the
    // "fonts/" folder. See Makefile.emscripten for details.
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    // nullptr, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    crbn::Uniform_Sphere_Sim_2d sim;

    sim.setTimeModifier(20);

    int particles = 10;
    int unset_particles = 10;
    float radius = 3;
    sim.setParticleCount(particles);

    // Set the dimensions of the particle box
    sim._rectangle_dims[0] = 10;
    sim._rectangle_dims[1] = 10;
    sim._rectangle_dims[2] = 1000;
    sim._rectangle_dims[3] = 1000;

    sim._bounce_losses = 0.9;

    bool pause = false;
    bool other_async = false;
    // std::vector<crbn::pos> draw_history;
    std::vector<float> draw_history;
    bool do_draw_history;

    auto randomise = [&]()
    {
        for (int i = 0; i < particles; i++)
        {
            crbn::particle_2d tmp;
            tmp.vel_y = rand() % 400;
            tmp.vel_x = rand() % 200;
            tmp.radius = radius;
            tmp.xpos = rand() % sim._rectangle_dims[2] + sim._rectangle_dims[0];
            tmp.ypos = rand() % sim._rectangle_dims[3] + sim._rectangle_dims[1];
            sim.setParticle(tmp, i);
        }
    };
    randomise();

    sim.runAsync(0);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a
    // fopen() of the imgui.ini file. You may manually call LoadIniSettingsFromMemory() to load
    // settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui
        // wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main
        // application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main
        // application, or clear/overwrite your copy of the keyboard data. Generally you may always
        // pass all inputs to dear imgui, and hide them from your application based on those two
        // flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        crbn::coord_type* coordbuf = sim.getCoordBuf();
        auto* imgui_drawlist = ImGui::GetBackgroundDrawList();

        crbn::pos tempPos;
        tempPos.x = coordbuf[0];
        tempPos.y = coordbuf[1 + 0];
        imgui_drawlist->AddCircleFilled(
            { float(tempPos.x), io.DisplaySize.y - float(tempPos.y) },
            radius,
            ImGui::GetColorU32({ 255, 0, 0, 255 }));
        for (int i = 2; i < particles * 2; i += 2)
        {
            tempPos.x = coordbuf[i];
            tempPos.y = coordbuf[1 + i];
            imgui_drawlist->AddCircleFilled(
                { float(tempPos.x), io.DisplaySize.y - float(tempPos.y) },
                radius,
                ImGui::GetColorU32({ 255, 255, 255, 255 }));
        }

        imgui_drawlist->AddQuad(
            { (float)sim._rectangle_dims[0], io.DisplaySize.y - (float)sim._rectangle_dims[1] },
            { (float)sim._rectangle_dims[0], io.DisplaySize.y - (float)sim._rectangle_dims[3] },
            { (float)sim._rectangle_dims[2], io.DisplaySize.y - (float)sim._rectangle_dims[3] },
            { (float)sim._rectangle_dims[2], io.DisplaySize.y - (float)sim._rectangle_dims[1] },
            ImGui::GetColorU32({ 255, 255, 255, 255 }));

        {
            ImGui::Begin("WINDOW");

            static float f = 0.0f;
            static int counter = 0;

            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);

            static float tmpdbl;

            if (ImGui::Checkbox("Draw Tracer", &do_draw_history))
            {
            }
            if (do_draw_history)
            {
                draw_history.push_back(coordbuf[0]);
                draw_history.push_back(coordbuf[1]);

                size_t drawsize = draw_history.size() / 2;

                ImGui::SliderInt("no", (int*)&drawsize, 0, 10000);
                if (drawsize > 4)
                {
                    auto* tempvecdat = draw_history.data();

                    ImVec2 u, v;
                    v.x = 0;
                    v.y = 0;

                    for (size_t i = 0; i < drawsize * 2; i +=2)
                    {
                        u = v;
                        v.x = tempvecdat[i];
                        v.y = io.DisplaySize.y - tempvecdat[i + 1];
                        // imgui_drawlist->AddLine(const ImVec2 &p1, const ImVec2 &p2, ImU32 col)
                        imgui_drawlist->AddLine(u, v, ImGui::GetColorU32({ 255, 0, 0, 255 }));
                        imgui_drawlist->PathLineTo(u);
                    }
                }
            }
            else
            {
                draw_history.clear();
            }

            // imgui_drawlist->AddPolyline((ImVec2*)draw_history.data(),
            // draw_history.size(),ImGui::ColorConvertFloat4ToU32({255,0,0,255}),0, 1 );
            // ImGui::SameLine(); ImGui::Text(" Draw History size = " );

            ImGui::SliderFloat("TimeModifier", &tmpdbl, 0, 20);
            sim.setTimeModifier(tmpdbl);
            if (tmpdbl == 0)
                pause = true;
            else
            {
                pause = false;
            }

            ImGui::SliderInt4("BOXBOX", (int*)&sim._rectangle_dims, 0, 3000);

            if (ImGui::Button("Randomise Speeds"))
                randomise();
            ImGui::SliderInt("Number of particles", &unset_particles, 1, 1000);
            if (ImGui::Button("Apply Particle change"))
            {
                sim.setParticleCount(unset_particles);
                sim.runAsync(0);
                particles = unset_particles;
                randomise();
            }
            ImGui::SliderFloat("Radius", &radius, 1, 100);
            ImGui::SliderFloat("gravity", (float*)&sim.gravity, -0, -100);
            ImGui::SliderInt("Chunk X", &sim._chunks.first, 1, 50);
            ImGui::SliderInt("Chunk Y", &sim._chunks.second, 1, 50);
            ImGui::Checkbox("ASYNC", &sim.async);
            ImGui::Checkbox("Other Async", &other_async);

            ImGui::SliderInt("Threads", (int*)&sim.threads, 1, 50);
            ImGui::End();
        }

        if (other_async)
        {
            if (!pause)
                auto future
                    = std::async(&crbn::Uniform_Sphere_Sim_2d::runAsync, &sim, io.DeltaTime);
        }
        else
        {
            if (!pause)
                sim.runAsync(io.DeltaTime);
        };
        // std::cout << io.DeltaTime << std::endl;

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(
            clear_color.x * clear_color.w,
            clear_color.y * clear_color.w,
            clear_color.z * clear_color.w,
            clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
