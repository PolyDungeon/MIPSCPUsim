// Dear ImGui: standalone example application for Win32 + OpenGL 3

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// This is provided for completeness, however it is strongly recommended you use OpenGL with SDL or GLFW.

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "CPUsects.h"
#include "MemoryRAM.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/GL.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>
// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;
static int counterSpeed = 5;
static int counter = 0;
static char textBuffer[128] = "";
static char pathString[128] = "";
static bool pathSubmitted = false;
static bool callFunct = false;
CPU::CPUsects cpuObj;
bool fileLoad = true;
std::vector<std::string> lines; // Vector to store lines of data
//static string 
// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
void renderBoxes();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"CPU", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"CPUGUI", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();
    
    bool timerStarted = false;
    // Start time point
    auto startTime = std::chrono::steady_clock::now();


    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    

    // Our state
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;


        // Check if the condition for starting the timer is met
        if (!timerStarted && pathSubmitted) {
            startTime = std::chrono::steady_clock::now();
            timerStarted = true;
        }

        if (timerStarted)
        {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
            //// If one second has passed, update count and read one line of data
            if (elapsedTime.count() >= 1) {
                counter++;
                startTime += std::chrono::seconds(1); // Increment start time by 1 second
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        renderBoxes();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, g_Width, g_Height);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}
static ImVec2 pos[11];// = { ImVec2((g_Width / 4), g_Height / 1.5), ImVec2(g_Width / 26, g_Height / 4), ImVec2(g_Width / 3, g_Height / 4), ImVec2(g_Width / 2, g_Height / 4), ImVec2(g_Width / 1.1, g_Height / 4), ImVec2(g_Width / .8, g_Height / 4), ImVec2(g_Width / 4.45, g_Height / 4), ImVec2(g_Width / 1.89, g_Height / 4) };
void UpdatePos()
{
    pos[0] = ImVec2((g_Width / 100), g_Height / 1.25);
    pos[1] = ImVec2(g_Width / 26, g_Height / 2.5);
    pos[2] = ImVec2(g_Width / 4.25, g_Height / 2.5);
    pos[3] = ImVec2(g_Width / 2.35, g_Height / 2.5);
    pos[4] = ImVec2(g_Width / 1.63, g_Height / 2.5);
    pos[5] = ImVec2(g_Width / 1.25 , g_Height / 2.5);
    pos[6] = ImVec2(g_Width / 5.675, g_Height / 4);
    pos[7] = ImVec2(g_Width / 2.72, g_Height / 4);
    pos[8] = ImVec2(g_Width / 1.8, g_Height / 4);
    pos[9] = ImVec2(g_Width / 1.345, g_Height / 4);
    pos[10] = ImVec2((g_Width / 4), g_Height / 10);
}

string checkType()
{
    switch (cpuObj.type) {
    case 16: // add
        return "+";
    case 34: // sub
        return "-";
    case 36:// and
        return "&";
    case 37:// or
        return "|";
    case 42:// slt
        return "<";
    case 35: // lw
        return "lw";
    case 43: // sw
        return "sw";
    case 4: // BEQ
        return "=";
    case 2: // Jump
        return "Jump";
    case 8: // ADDI
        return"+";
    }
}

string checkInsType()
{
    switch (cpuObj.type) {
    case 16: // add
        return "add";
    case 34: // sub
        return "sub";
    case 36:// and
        return "and";
    case 37:// or
        return "or";
    case 42:// slt
        return "slt";
    case 35: // lw
        return "lw";
    case 43: // sw
        return "sw";
    case 4: // BEQ
        return "beq";
    case 2: // Jump
        return "Jump";
    case 8: // ADDI
        return"addi";
    case -1:
        return "";
    default:
        return "";
    }
}

bool oneSubmit = false;
bool twoSubmit = false;
vector<string> ins;
int fileLength = 0;
string ex = "";
int numLines = 0;
int insCount = 0;
void renderBoxes() {

    UpdatePos();
    // Set minimum and maximum constraints for the window size
    ImGui::SetNextWindowSize(ImVec2(g_Width, g_Height + 40));
    ImGui::SetNextWindowPos(ImVec2(0, -40));

    if (pathSubmitted && fileLoad)
    {
        std::ifstream file(textBuffer, std::ios::binary); // Open the file in binary mode
        if (!file.is_open()) {
            std::cerr << "Error opening file\n";
            exit(1);
        }

        // Determine the size of the file
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Allocate memory to store the file contents
        std::vector<char> buffer(fileSize);

        // Read file contents into memory
        file.read(buffer.data(), fileSize);
        file.close();
        string file2 = buffer.data();
        fileLength = file2.length();
        
        
        // Tokenize the buffer into lines and store in lines vector
        std::istringstream ss(buffer.data());
        std::string line = "";

        while (std::getline(ss, line)) {
            
            lines.push_back(line);
            unsigned int intData = cpuObj.binToInt(line);
            cpuObj.meme.writeMemory(0x600 + (32 * numLines), intData);
            numLines++;
        }

        string instructions = cpuObj.intToBin(cpuObj.meme.readMemory(0x600 + 2 * 32), 32);
        fileLoad = false;
    }
    ImGui::Begin("Boxes", nullptr, ImGuiWindowFlags_AlwaysAutoResize);


    for (int i = 0; i <= 10; ++i) {

        ImGui::SetCursorPos(pos[i]);

        if (i == 0)
        {
            ImGui::BeginChild(std::to_string(i).c_str(), ImVec2(), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
            
            if (pathSubmitted)
            {
                // Begin the table
                ImGui::BeginTable("MyTable", 32, ImGuiWindowFlags_AlwaysHorizontalScrollbar, ImVec2((2000), 10), 10);
                // Set column headers
                for (int i = 1; i < 32; ++i) {
                    ImGui::TableSetupColumn(("Column " + std::to_string(i + 1)).c_str());
                }
                ImGui::TableNextRow();

                // Fill in the data for the first row
                for (int i = 0; i < 32; ++i) {
                    ImGui::TableSetColumnIndex(i);
                    ImGui::Text("Reg %d: ", i);
                }
                ImGui::TableNextRow();

                // Fill in the data for the first row
                for (int i = 0; i < 32; ++i) {
                    ImGui::TableSetColumnIndex(i);
                    ImGui::Text("%d", cpuObj.registers[i]);
                }

                // End the table
                ImGui::EndTable();

                ImGui::Text("Time count: %d", counter);
                ImGui::Text("Add Count: %d", cpuObj.addCount);
                ImGui::SameLine();
                ImGui::Text("   Addi Count: %d", cpuObj.addICount);
                ImGui::SameLine();
                ImGui::Text("   Sub Count: %d", cpuObj.subCount);
                ImGui::SameLine();
                ImGui::Text("   Or Count: %d", cpuObj.orCount);
                ImGui::SameLine();
                ImGui::Text("   And Count: %d", cpuObj.andCount);
                ImGui::SameLine();
                ImGui::Text("   Slt Count: %d", cpuObj.sltCount);
                ImGui::Text("Program Counter %d", cpuObj.PC);

            }
            ImGui::EndChild();

        }
        
            if (i > 0 && i < 6)
            {
                ImGui::BeginChild(std::to_string(i).c_str(), ImVec2(g_Width / 8, g_Width / 8), true);
                if (pathSubmitted && !fileLoad)
                {
                    if (i == 1)
                    {
                        ImGui::Text("Instruction Fetch");

                        if (counter >= 0)
                        {
                            ImGui::TextWrapped("ins %d from memory", insCount + 1);
                        }
                        if (counter > (5 / counterSpeed))
                        {

                            ImGui::TextWrapped("Instruction: %s", lines[insCount].c_str());

                            ins = cpuObj.fetch(lines[insCount]);

                        }

                    }
                    if (i == 2)
                    {
                        ImGui::Text("Instruction Decode");
                        int output = 0;
                        output = cpuObj.InstructionDecode(ins);
                        if (counter > (10 / counterSpeed))
                        {
                            if (output == 1)
                            {

                                ImGui::TextWrapped("input 1: %d \ninput 2: %d \ntarget reg: %d", cpuObj.r1, cpuObj.r2, cpuObj.targetr);
                            }
                            else if (output == 2)
                            {
                                ImGui::TextWrapped("input 1: %d \nimm: %d \ntarget reg: %d", cpuObj.r1, cpuObj.imm, cpuObj.targetr);
                            }
                            else if (output == 3)
                            {
                                ImGui::Text("imm: \n%s", cpuObj.imm);
                            }
                            else
                            {
                                ImGui::Text("error");
                                ImGui::Text("r1: %d \nr2: %d \ntarget reg: %d", cpuObj.r1, cpuObj.r2, cpuObj.targetr);
                            }
                        }

                    }
                    if (i == 3)
                    {

                        ImGui::Text("Instruction Execute");

                        if (counter > (15 / counterSpeed))
                        {
                            ImGui::Text("%d : %s", cpuObj.type, checkInsType().c_str());
                            if (twoSubmit == false)
                            {
                                ex = cpuObj.InstructionExectue(ins);
                                twoSubmit = true;
                            }
                            if (ex == "lw")
                            {
                                ImGui::TextWrapped("loading data ...", ins[2].c_str());
                                ImGui::TextWrapped("data: ", cpuObj.targetr);
                            }
                            else if (ex == "sw")
                            {
                                ImGui::TextWrapped("storing Data ...", ins[3].c_str(), ins[2].c_str());
                            }
                            else
                            {
                                //three registers used
                                if (cpuObj.type == 16 || cpuObj.type == 34 || cpuObj.type == 36 || cpuObj.type == 37 || cpuObj.type == 42)
                                {
                                    ImGui::TextWrapped("%d = %d %s %d", cpuObj.registers[cpuObj.targetr], cpuObj.registers[cpuObj.r1], checkType().c_str(), cpuObj.registers[cpuObj.r2]);
                                }
                                else if (cpuObj.type == 8)
                                {
                                    if (cpuObj.r1 == cpuObj.targetr)
                                    {
                                        ImGui::TextWrapped("%d = %d %s %d", cpuObj.registers[cpuObj.targetr], cpuObj.registers[cpuObj.r1] - cpuObj.imm, checkType().c_str(), cpuObj.imm);
                                    }
                                    else
                                    {
                                        ImGui::TextWrapped("%d = %d %s %d", cpuObj.registers[cpuObj.targetr], cpuObj.registers[cpuObj.r1], checkType().c_str(), cpuObj.imm);
                                    }
                                }
                                //2 registers  and one immediate
                                else if (cpuObj.type == 4)
                                {
                                    ImGui::TextWrapped("increase PC by %d if %d %s %d", cpuObj.imm, cpuObj.registers[cpuObj.r1], checkType().c_str(), cpuObj.registers[cpuObj.targetr]);
                                    if (ex == "beq")
                                    {
                                        ImGui::Text("true");
                                    }
                                    else
                                    {
                                        ImGui::Text("false");
                                    }
                                }
                                //no registers only imm
                                else if (cpuObj.type == 43)
                                {
                                    //J
                                    ImGui::TextWrapped("jump to address: %s", cpuObj.imm);
                                }
                            }
                        }
                    }
                    if (i == 4)
                    {
                        ImGui::Text("Instruction Memory");
                        if (counter > (20 / counterSpeed))
                        {
                            if (ex == "lw")
                            {
                                ImGui::TextWrapped("loading value: %d from address: %s", cpuObj.registers[cpuObj.targetr], ins[2].c_str());
                                // Computed address must be a multiple of 4
                            }
                            else if (ex == "sw")
                            {
                                ImGui::TextWrapped("storing value: %d in address: %s", cpuObj.meme.readMemory(cpuObj.binToInt(ins[2])), ins[2].c_str());
                                //Computed address must be a multiple of 4
                            }
                        }
                    }
                    if (i == 5)
                    {
                        
                        ImGui::Text("Write Back");
                        if (counter > (25 / counterSpeed))
                        {
                            ImGui::TextWrapped("Registers updated");
                            ImGui::TextWrapped("ProgramCounter updating");
                        }
                        if (counter > (35 / counterSpeed))
                        {
                            if (ex == "beq")
                            {
                                cpuObj.WriteBack("");
                                twoSubmit = false;
                                insCount + cpuObj.imm;
                            }
                            else if (ex == "j")
                            {
                                cpuObj.WriteBack("");
                                twoSubmit = false;
                                if (cpuObj.imm < numLines)
                                {
                                    insCount = cpuObj.imm;
                                }
                                else //if (cpuObj.imm)
                                {

                                }
                                
                            }
                            else if (ex != "j" && ex != "beq" && insCount < numLines - 1)
                            {
                                
                                cpuObj.WriteBack("");
                                twoSubmit = false;
                                counter = 0;
                                insCount++;
                            }

                            
                        }
                    }
                }

                ImGui::EndChild();
        }
        
        if (i >= 6 && i < 10)
        {
            ImGui::BeginChild(std::to_string(i).c_str(), ImVec2(g_Width / 20, g_Height / 2), true);

            ImGui::EndChild();
        }
        if (i == 10)
        {
            ImGui::BeginChild(std::to_string(i).c_str(), ImVec2((g_Width / 2), g_Height / 7), true);
            
            if (pathSubmitted == false)
            {
                // Text input box
                ImGui::Text("Enter path to file");
                ImGui::InputText("##inputText", textBuffer, IM_ARRAYSIZE(textBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::SameLine();
                
                // Button next to the text input box
                if (ImGui::Button("Submit")) {
                    // Handle button click action here
                    //std::cout << "Submitted: " << textBuffer << std::endl;

                    std::ifstream file2(textBuffer, std::ios::binary); // Open the file in binary mode
                    if (!file2.is_open()) {
                        std::cerr << "Error opening file\n";
                        //ImGui::Text("Error opening file");
                        oneSubmit = true;
                    }
                    else
                    {
                        file2.close();
                        pathSubmitted = true;
                        oneSubmit = false;
                    }

                }
                if (oneSubmit == true)
                {
                    ImGui::Text("Error opening file");
                }
                ImGui::SliderInt("counterSpeed", &counterSpeed, 1, 10);            // Edit 1 float using a slider from 0.0f to 1.0f
            }
            else
            {
                ImGui::Text("Memory");
                for (int k = 0; k < numLines; k++)
                {
                    string instructions = cpuObj.intToBin(cpuObj.meme.readMemory(0x600 + k * 32), 32);
                    ImGui::TextWrapped("Address 0x%x: %s", 0x600 + k * 32,lines[k].c_str());
                    //cout << instructions;
                    //ImGui::Text("%d", k);
                }

                //ImGui::TextWrapped((cpuObj.intToBin(cpuObj.meme.readMemory(0x2 + (numLines * 32), 32 * numLines))));
                //need to update to actually load the data into our memory map line by line. 
                // would look something like 
                // "0x0000: 00000000010000110000100000100000"  - for bits 0 - 32
                // "0x0021: 00000000011001000001000000100010" - for bits 33-64
                // "0x0041: 00000000110001000010100000100100" - for bits 65-96
                // "0x0061: 00000000110001000010100000100100" - for bits 97-128
                //ImGui::Text(lines[0].c_str());
                //ImGui::Text(lines[0].c_str());
            }
            
            ImGui::EndChild();
        }
        
        


        /*for (int j = 0; j < 4; ++j) {
            ImGui::Text("Box %d", j + 1);
            if (j < 3)
                ImGui::Separator();
        }*/

        //draw lines
        //if (i < 4) {
        //    ImVec2 p1 = ImVec2(pos[i + 1].x - 50, pos[i + 1].y - 50); // Corrected this line
        //    ImVec2 p2 = ImVec2(pos[i + 1].x + 125, pos[i + 1].y + 50); // Corrected this line
        //    ImGui::GetWindowDrawList()->AddLine(p1, p2, IM_COL32(255, 255, 255, 255), 10.0f);
        //}

    }
    //UpdatePos();
    ImGui::End();
}



// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}