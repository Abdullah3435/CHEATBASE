
#include "cheats/Aimbot.h"
#include "iostream"

bool InitAimbot();
bool GLOBAL_UPDATE();
bool UpdateRenderer();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    OutputDebugString(L"GLOBAL ENTRY\n");
    InitAimbot();
    GLOBAL_UPDATE();

    return 0;
} 

bool GLOBAL_UPDATE()
{
    while (true)
    {
        if (GetAsyncKeyState(VK_LSHIFT))
        {
            OutputDebugString(L"Frame called\n");
            Aimbot::frame();
        }
    }
    return true;
}

bool InitAimbot()
{
    Aimbot::target_process = memory::GetProcID(L"cs2.exe");
    Aimbot::base = memory::GetModuleBaseAddress(Aimbot::target_process, L"client.dll");
    
    return true;

}

bool UpdateRenderer() {
    return true;
    /*HWND hwnd = window::InitWindow(hInstance);
    if (!hwnd) return -1;

    if (!renderer::init(hwnd))
    {
        renderer::destroy();
        return -1;
    }

    while (!GetAsyncKeyState(VK_F9) && renderer::running)
    {
        renderer::frame();
    }

    renderer::destroy();
    UnregisterClass(L"overlay", hInstance);

    return 0;*/
}
