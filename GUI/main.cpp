// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
#define _CRT_SECURE_NO_WARNINGS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <Windows.h>
#include <commdlg.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

typedef int(*p_gen_chain_word)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_gen_chain_char)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int get_open_file_name(char *file_name)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = 0;
    ofn.lpstrDefExt = 0;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = TEXT("所有文件(*.*)\0*.*\0Plain Text(*.txt)\0*.txt\0\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = 0;
    ofn.lpstrTitle = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    return GetOpenFileName(&ofn);
}

int get_save_file_name(char *file_name)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = 0;
    strcpy(file_name, "solution.txt");
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = TEXT("所有文件(*.*)\0*.*\0Plain Text(*.txt)\0*.txt\0\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = 0;
    ofn.lpstrTitle = 0;
    ofn.lpstrDefExt = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    return GetSaveFileName(&ofn);
}

#define BUFSIZE 10000000
#define MAX_WORD_NUM 100000
#define MAX_WORD_LONG 300
char buf[BUFSIZE];
char head[10];
char tail[10];
char words[MAX_WORD_NUM][MAX_WORD_LONG];
char *pwords[MAX_WORD_NUM];
char *result[MAX_WORD_NUM];

int main(void)
{
    HINSTANCE CoreDLL = LoadLibrary("Core.dll");
    if (CoreDLL == NULL) {
        cout << "File 'Core.dll' not found." << endl;
        exit(1);
    }

    p_gen_chain_word gen_chain_word = (p_gen_chain_word)GetProcAddress(CoreDLL, "gen_chain_word");
    p_gen_chain_char gen_chain_char = (p_gen_chain_char)GetProcAddress(CoreDLL, "gen_chain_char");
    if (gen_chain_word == NULL || gen_chain_char == NULL) {
        cout << "Invalid file 'Core.dll'." << endl;
        exit(1);
    }

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    io.Fonts->AddFontFromFileTTF("X:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    ifstream inf;
    ofstream outf;
    bool show_demo_window = false, par_right = false;
    bool par_w = false, par_c = false, par_h = false, par_t = false, par_r = false;
    bool show_result = false;
    bool show_another_window = false;
    bool input_error = false;
    //const char *p_answer;
    string answer;
    char open_file_name[1000];
    char save_file_name[1000];

    memset(buf, '\0', sizeof(buf));
    memset(open_file_name, '\0', sizeof(open_file_name));
    memset(save_file_name, '\0', sizeof(open_file_name));
    memset(head, '\0', sizeof(head));
    memset(tail, '\0', sizeof(tail));
    memset(words, '\0', sizeof(words));
    memset(result, '\0', sizeof(result));
    memset(buf, '\0', sizeof(buf));
    memset(head, '\0', sizeof(head));
    memset(tail, '\0', sizeof(tail));
    const char label[] = "input words";
    const char label_head[] = "head";
    const char label_tail[] = "tail";
    const char label_path[] = "path";
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            par_right = true;
            ImGui::Begin("Word_chain");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Please input words or select file:");
            if (ImGui::Button("Select File")) {
                show_result = false;
                input_error = false;
                if (get_open_file_name(open_file_name)) {
                    inf.open(open_file_name);
                    if (inf) {
                        int cnt = 0;
                        string s;
                        memset(buf, '\0', sizeof(buf));
                        while (getline(inf, s)) {
                            int size = (int)s.length();
                            for (int i = 0; i < size; i++) {
                                buf[cnt++] = s[i];
                                if (cnt > BUFSIZE - 10) {
                                    strcpy(open_file_name, "Error: the file is too large");
                                    goto label2;
                                }
                            }
                            buf[cnt++] = '\n';
                            buf[cnt] = '\0';
                        }
                    }
                label2:
                    inf.close();
                }
            }
            ImGui::SameLine();
            ImGui::Text("%s", open_file_name);
            if (ImGui::InputTextMultiline(label, buf, BUFSIZE - 1, ImVec2(200, 200), 0, NULL, NULL)) {
                show_result = false;
            }

            ImGui::Text("Please select parameters:");               // Display some text (you can use a format strings too)
            if (ImGui::Checkbox("-w", &par_w)) {
                show_result = false;
            }
            if (ImGui::Checkbox("-c", &par_c)) {
                show_result = false;
            }
            if (!par_w && !par_c) {
                ImGui::Text("The execution mode has not been selected.");
                par_right = false;
            }
            if (par_w && par_c) {
                ImGui::Text("Execution mode conflict.");
                par_right = false;
            }

            if (ImGui::Checkbox("-r", &par_r)) {
                show_result = false;
            }

            if (ImGui::Checkbox("-h", &par_h)) {
                show_result = false;
            }
            ImGui::SameLine();
            if (ImGui::InputText(label_head, head, 2, 0, NULL, NULL)) {
                show_result = false;
            }
            if (par_h && (head[1] != '\0' || !(head[0] >= 'a' && head[0] <= 'z' || head[0] >= 'A' && head[0] <= 'Z'))) {
                ImGui::SameLine();
                ImGui::Text("wrong head %s.", head);
                par_right = false;
            }

            if (ImGui::Checkbox("-t", &par_t)) {
                show_result = false;
            }
            ImGui::SameLine();
            if (ImGui::InputText(label_tail, tail, 2, 0, NULL, NULL)) {
                show_result = false;
            }
            if (par_t && (tail[1] != '\0' || !(tail[0] >= 'a' && tail[0] <= 'z' || tail[0] >= 'A' && tail[0] <= 'Z'))) {
                ImGui::Text("wrong tail %s.", tail);
                par_right = false;
            }


            if (par_right && ImGui::Button("Run")) {
                int length, ifword;
                int wordnum = 0;
                char h, t;
                answer.clear();
                int size = (int)strlen(buf);
                for (int i = 0; i < size; i++) {
                    length = 0;
                    ifword = 0;
                    while (i < size && isalpha(buf[i])) {
                        words[wordnum][length++] = (char)tolower(buf[i]);
                        if (length > MAX_WORD_LONG - 2) {
                            answer = string("Error: the word is too long.");
                            goto label1;
                        }
                        ifword = 1;
                        i++;
                    }
                    if (ifword) {
                        words[wordnum][length] = '\0';
                        pwords[wordnum] = words[wordnum];
                        wordnum++;
                        if (wordnum > MAX_WORD_NUM - 2) {
                            answer = string("Error: the word list is too long.");
                            goto label2;
                        }
                    }
                }

                h = par_h ? head[0] : 0;
                t = par_t ? tail[0] : 0;
                int cnt = 0;
                if (par_w) {
                    try {
                        cnt = gen_chain_word(pwords, wordnum, result, h, t, par_r);
                        for (int i = 0; i < cnt; i++) {
                            answer += result[i];
                            answer += "\n";
                        }
                    }
                    catch (const char *error_message) {
                        answer = string(error_message);
                    }

                }
                else if (par_c) {
                    try {
                        cnt = gen_chain_char(pwords, wordnum, result, h, t, par_r);
                        for (int i = 0; i < cnt; i++) {
                            answer += result[i];
                            answer += "\n";
                        }
                    }
                    catch (const char *error_message) {
                        answer = string(error_message);
                    }
                }
            label1:
                show_result = true;
            }
            if (par_right && show_result) {
                ImGui::Text("Result:\n%s", answer.c_str());
                if (ImGui::Button("Export")) {
                    if (get_save_file_name(save_file_name)) {
                        outf.open(save_file_name);
                        if (outf) {
                            outf << answer;
                        }
                        outf.close();
                    }
                }
            }
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    FreeLibrary(CoreDLL);
    return 0;
}
