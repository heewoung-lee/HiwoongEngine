#include <Windows.h>

LRESULT CALLBACK WindowProc(
    HWND window,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    // 창 내부에서 커서 모양을 정하라는 요청
    if (message == WM_SETCURSOR &&
        LOWORD(lParam) == HTCLIENT)
    {
        SetCursor(nullptr); // 커서 그림을 없앤다.
        return TRUE;        // 우리가 처리했으므로 기본 처리를 막는다.
    }

    // 창 안을 클릭하면 커서를 가둔다.
    if (message == WM_LBUTTONDOWN)
    {
        RECT area{};
        POINT origin{};

        if (!GetClientRect(window, &area) ||
            !ClientToScreen(window, &origin))
        {
            return 0;
        }

        // 창 내부 좌표를 모니터 전체 기준 좌표로 바꾼다.
        OffsetRect(&area, origin.x, origin.y);

        if (!ClipCursor(&area))
        {
            MessageBoxW(window, L"Mouse lock failed.",
                L"Cursor Test", MB_OK);
        }

        return 0;
    }

    // ESC를 누르거나 다른 창으로 전환하면 해제한다.
    if ((message == WM_KEYDOWN && wParam == VK_ESCAPE) ||
        message == WM_KILLFOCUS)
    {
        ClipCursor(nullptr);
        return 0;
    }

    // 창을 닫으면 프로그램 종료를 요청한다.
    if (message == WM_DESTROY)
    {
        ClipCursor(nullptr);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(window, message, wParam, lParam);
}

int main()
{
    const HINSTANCE instance = GetModuleHandleW(nullptr);
    const wchar_t* className = L"CursorTestWindow";

    // 창의 기본 설정과 요청 처리 함수를 등록한다.
    WNDCLASSW windowClass{};
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hbrBackground =
        reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    if (!RegisterClassW(&windowClass))
        return 1;

    // 등록한 설정으로 창을 만든다.
    HWND window = CreateWindowExW(
        0,
        className,
        L"Cursor Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        640, 480,
        nullptr, nullptr,
        instance, nullptr
    );

    if (window == nullptr)
        return 1;

    ShowWindow(window, SW_SHOW);

    // 창의 요청을 받아 WindowProc으로 전달한다.
    MSG message{};

    while (true)
    {
        const BOOL result = GetMessageW(&message, nullptr, 0, 0);

        if (result == -1)
            return 1;

        if (result == 0)
            break;

        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return 0;
}