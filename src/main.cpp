#include <stdio.h>
#include <Windows.h>

// 消息换行
void KeyCtrlEnter(HWND hwnd) {
    INPUT inputs[1] = {};
    // Ctrl down
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LCONTROL;
    inputs[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    Sleep(100); // 等待GetKeyState可以获取到按键状态
    // Enter down
    SendMessageW(hwnd, WM_KEYDOWN, (WPARAM)VK_RETURN, (LPARAM)0);
    // Enter up
    SendMessageW(hwnd, WM_KEYUP, (WPARAM)VK_RETURN, (LPARAM)0);
    // Ctrl up
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LCONTROL;
    inputs[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

void SendText(HWND hwnd, LPCWSTR text)
{
    size_t le = wcslen(text);
    for (int i = 0; i < le; i++)
    {
        if (text[i] == L'\n')
        {
            KeyCtrlEnter(hwnd);
        }
        else
        {
            SendMessageW(hwnd, WM_CHAR, (WPARAM)text[i], (LPARAM)0);
        }
        Sleep(1);
    }
    SendMessageW(hwnd, WM_CHAR, (WPARAM)L'\0', (LPARAM)0);
    // Sleep(200);
    SendMessageW(hwnd, WM_KEYDOWN, (WPARAM)VK_RETURN, (LPARAM)0);
    SendMessageW(hwnd, WM_KEYUP, (WPARAM)VK_RETURN, (LPARAM)0);
}

void MouseLeftClick()
{
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    // left down
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // left up
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

// 保证窗口置顶, 直接模拟鼠标点击输入框
void SetTextFocus(HWND hwnd)
{
    RECT rect = {0};
    GetWindowRect(hwnd, &rect);
    // printf("%d %d %d %d\n", rect.left, rect.top, rect.right, rect.bottom);
    SetCursorPos(rect.left + 50, rect.bottom - 70);
    MouseLeftClick();
    Sleep(200);
}

// 重新打开窗口, 这会自动获得输入焦点
void SetWXTextFocus(HWND chatWnd)
{
    ShowWindow(chatWnd, SW_HIDE);
    ShowWindow(chatWnd, SW_SHOWDEFAULT);
    SetForegroundWindow(chatWnd);
    // SetWindowPos(chatWnd, HWND_TOP, 600, 200, 0, 0, SWP_NOSENDCHANGING | SWP_NOSIZE);
    Sleep(100);
}

void CallAudio(HWND hwnd)
{
    RECT rect = {0};
    GetWindowRect(hwnd, &rect);
    SetCursorPos(rect.right - 65, rect.bottom - 120);
    MouseLeftClick();
}

void CallVideo(HWND hwnd)
{
    RECT rect = {0};
    GetWindowRect(hwnd, &rect);
    SetCursorPos(rect.right - 36, rect.bottom - 120);
    MouseLeftClick();
}

int usage()
{
    HWND chatWnd = FindWindowA("ChatWnd", NULL);
    SetWXTextFocus(chatWnd);
    if (GetForegroundWindow() == chatWnd)
    {
        SendText(chatWnd, L"在吗?");
        // CallAudio(chatWnd);
        // CallVideo(chatWnd);
    }
    else
    {
        printf("获取输入焦点失败\n");
    }
    return 0;
}

// #define DllExport   __declspec( dllexport )
// 静态链接不需要申明导出符号
#define DllExport

extern "C" DllExport void wx_send(LPCWSTR text)
{
    HWND chatWnd = FindWindowA("ChatWnd", NULL);
    SetWXTextFocus(chatWnd);
    if (GetForegroundWindow() != chatWnd)
    {
        return;
    }
    SendText(chatWnd, text);
}

extern "C" DllExport void wx_audio()
{
    HWND chatWnd = FindWindowA("ChatWnd", NULL);
    SetWXTextFocus(chatWnd);
    if (GetForegroundWindow() != chatWnd)
    {
        return;
    }
    CallAudio(chatWnd);
}

extern "C" DllExport void wx_video()
{
    HWND chatWnd = FindWindowA("ChatWnd", NULL);
    SetWXTextFocus(chatWnd);
    if (GetForegroundWindow() != chatWnd)
    {
        return;
    }
    CallVideo(chatWnd);
}

int _main()
{
    wx_send(LR"(
        你
        好
    )");
    return 0;
}