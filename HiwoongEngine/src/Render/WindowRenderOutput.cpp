#include "WindowRenderOutput.h"
#include "Core/Input.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>

namespace Hiwoong
{
    //문자 한 바이트: 256가지
    //글자색 16가지 × 배경색 16가지: 256가지
    WindowRenderOutput::WindowRenderOutput(
        const Vector2& characterSize)
        : characterSize(characterSize),
        glyphCache(256 * 256)
    {
        CreateRenderFont();
        try
        {
            CreateRenderWindow();
        }
        catch (...)
        {
            DeleteObject(fontHandle);
            fontHandle = nullptr;
            throw;
        }
    }

    WindowRenderOutput::~WindowRenderOutput()
    {
        ReleaseGlyphBuffer();
        ReleaseBackBuffer();

        if (windowHandle != nullptr && IsWindow(windowHandle))
        {
            DestroyWindow(windowHandle);
        }

        windowHandle = nullptr;

        if (fontHandle != nullptr)
        {
            DeleteObject(fontHandle);
            fontHandle = nullptr;
        }
    }

    void WindowRenderOutput::Resize(const Vector2& screenSize)
    {
        RECT rect{};
        rect.right = static_cast<LONG>(
            screenSize.x * characterSize.x);
        rect.bottom = static_cast<LONG>(
            screenSize.y * characterSize.y);

        // 그림을 그릴 영역에 제목 표시줄과 테두리 크기를 더한다.
        if (!AdjustWindowRectEx(
            &rect, WS_OVERLAPPEDWINDOW, FALSE, 0))
        {
            throw std::runtime_error("Window size calculation failed.");
        }

        // 창 위치는 유지하고 크기만 변경한다.
        if (!SetWindowPos(
            windowHandle, nullptr,
            0, 0,
            rect.right - rect.left,
            rect.bottom - rect.top,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE))
        {
            throw std::runtime_error("Window resize failed.");
        }

        this->screenSize = screenSize;
    }

    void WindowRenderOutput::Present(const CHAR_INFO* cells)
    {
        if (cells == nullptr ||
            screenSize.x <= 0 || screenSize.y <= 0)
        {
            return;
        }

        const std::size_t cellCount =
            static_cast<std::size_t>(screenSize.x) *
            static_cast<std::size_t>(screenSize.y);

        // 각 칸의 문자와 색상을 복사한다.
        frameCells.assign(cells, cells + cellCount);

        // 창 전체를 다시 그리도록 요청한다.
        InvalidateRect(windowHandle, nullptr, FALSE);
    }

    Vector2 WindowRenderOutput::GetCharacterSize() const
    {
        return characterSize;
    }

    bool WindowRenderOutput::ProcessEvents()
    {
        MSG message{};

        while (PeekMessageW(
            &message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                return false;
            }

            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        return true;
    }


    LRESULT CALLBACK WindowRenderOutput::WindowProc(
        HWND window,
        UINT message,
        WPARAM wParam,
        LPARAM lParam)
    {

        if (message == WM_NCCREATE)
        {
            const auto* createInfo =
                reinterpret_cast<const CREATESTRUCTW*>(lParam);

            auto* output = static_cast<WindowRenderOutput*>(
                createInfo->lpCreateParams);

            SetWindowLongPtrW(
                window,
                GWLP_USERDATA,
                reinterpret_cast<LONG_PTR>(output)
            );
        }

        auto* output = reinterpret_cast<WindowRenderOutput*>(
            GetWindowLongPtrW(window, GWLP_USERDATA)
            );

        if (message == WM_ERASEBKGND)
        {
            // 배경도 백 버퍼에 포함해서 그리므로 별도로 지우지 않는다.
            return 1;
        }

        if (message == WM_SETCURSOR &&
            LOWORD(lParam) == HTCLIENT &&
            GetForegroundWindow() == window &&
            Input::IsMouseLockedTo(window))
        {
            SetCursor(nullptr);
            return TRUE;
        }

        if (message == WM_PAINT)
        {
            PAINTSTRUCT paint{};
            HDC dc = BeginPaint(window, &paint);

            if (output != nullptr)
            {
                output->DrawFrame(dc);
            }

            EndPaint(window, &paint);
            return 0;
        }

        if (message == WM_DESTROY)
        {
            // 창이 닫혔음을 메시지 루프에 알린다.
            PostQuitMessage(0);
            return 0;
        }

        // 따로 처리하지 않은 메시지는 Windows에 맡긴다.
        return DefWindowProcW(window, message, wParam, lParam);
    }
    //순서 : 원래 비트맵 복원 → 만든 비트맵 삭제 → 작업 공간 삭제
    void WindowRenderOutput::ReleaseBackBuffer()
    {
        if (backBufferDC != nullptr)
        {
            // 우리가 만든 비트맵을 작업 공간에서 분리한다.
            SelectObject(backBufferDC, previousBitmap);

            DeleteObject(backBufferBitmap);
            DeleteDC(backBufferDC);
        }

        backBufferDC = nullptr;
        backBufferBitmap = nullptr;
        previousBitmap = nullptr;
        backBufferWidth = 0;
        backBufferHeight = 0;
    }

    bool WindowRenderOutput::CreateBackBuffer(
        HDC referenceDC, int width, int height)
    {
        if (width <= 0 || height <= 0) return false;

        // 같은 크기의 버퍼가 있으면 재사용한다.
        if (backBufferDC != nullptr &&
            backBufferWidth == width &&
            backBufferHeight == height)
        {
            return true;
        }

        HDC newDC = CreateCompatibleDC(referenceDC);
        if (newDC == nullptr) return false;

        HBITMAP newBitmap =
            CreateCompatibleBitmap(referenceDC, width, height);

        if (newBitmap == nullptr)
        {
            DeleteDC(newDC);
            return false;
        }

        HGDIOBJ oldBitmap = SelectObject(newDC, newBitmap);
        if (oldBitmap == nullptr || oldBitmap == HGDI_ERROR)
        {
            DeleteObject(newBitmap);
            DeleteDC(newDC);
            return false;
        }

        // 새 버퍼 생성에 성공한 뒤 기존 버퍼를 정리한다.
        ReleaseBackBuffer();

        backBufferDC = newDC;
        backBufferBitmap = newBitmap;
        previousBitmap = oldBitmap;
        backBufferWidth = width;
        backBufferHeight = height;

        return true;
    }

    bool WindowRenderOutput::CreateGlyphBuffer(HDC referenceDC)
    {
        if (glyphDC != nullptr) return true;

        const int width = static_cast<int>(characterSize.x);
        const int height = static_cast<int>(characterSize.y);
        if (width <= 0 || height <= 0) return false;

        HDC newDC = CreateCompatibleDC(referenceDC);
        if (newDC == nullptr) return false;

        BITMAPINFO info{};
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = width;
        info.bmiHeader.biHeight = -height;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;

        void* pixels = nullptr;
        HBITMAP newBitmap = CreateDIBSection(
            referenceDC, &info, DIB_RGB_COLORS,
            &pixels, nullptr, 0
        );

        if (newBitmap == nullptr)
        {
            DeleteDC(newDC);
            return false;
        }

        HGDIOBJ oldBitmap = SelectObject(newDC, newBitmap);
        if (oldBitmap == nullptr || oldBitmap == HGDI_ERROR)
        {
            DeleteObject(newBitmap);
            DeleteDC(newDC);
            return false;
        }

        SelectObject(newDC, fontHandle);
        SetTextAlign(newDC, TA_LEFT | TA_TOP | TA_NOUPDATECP);

        glyphDC = newDC;
        glyphBitmap = newBitmap;
        previousGlyphBitmap = oldBitmap;
        glyphBits = static_cast<DWORD*>(pixels);

        return true;
    }

    const std::vector<DWORD>& WindowRenderOutput::GetGlyphPixels(
        const CHAR_INFO& cell)
    {
        const unsigned char character =
            cell.Char.AsciiChar == '\0'
            ? ' '
            : static_cast<unsigned char>(cell.Char.AsciiChar);

        const WORD colors = cell.Attributes & 0x00FF;

        // 문자와 색상 조합에 해당하는 저장 칸을 찾는다.
        auto& pixels = glyphCache[colors * 256 + character];

        // 이미 만든 그림이면 바로 반환한다.
        if (!pixels.empty()) return pixels;

        const int width = static_cast<int>(characterSize.x);
        const int height = static_cast<int>(characterSize.y);
        const RECT area{ 0, 0, width, height };

        SetTextColor(glyphDC, ToWindowColor(colors));
        SetBkColor(glyphDC, ToWindowColor(colors >> 4));

        if (ExtTextOutA(
            glyphDC, 0, 0,
            ETO_OPAQUE | ETO_CLIPPED,
            &area,
            reinterpret_cast<const char*>(&character),
            1, nullptr))
        {
            // Windows의 그리기를 완료한 뒤 픽셀을 읽는다.
            GdiFlush();

            pixels.assign(
                glyphBits,
                glyphBits + static_cast<std::size_t>(width) * height
            );
        }

        return pixels;
    }

 
    COLORREF WindowRenderOutput::ToWindowColor(WORD color)
    {
        static const COLORREF palette[16] =
        {
            RGB(0,   0,   0),     // 0: 검정
            RGB(0,   0,   128),   // 1: 어두운 파랑
            RGB(0,   128, 0),     // 2: 어두운 초록
            RGB(0,   128, 128),   // 3: 어두운 청록
            RGB(128, 0,   0),     // 4: 어두운 빨강
            RGB(128, 0,   128),   // 5: 어두운 자홍
            RGB(128, 128, 0),     // 6: 어두운 노랑
            RGB(192, 192, 192),   // 7: 회색
            RGB(128, 128, 128),   // 8: 어두운 회색
            RGB(0,   0,   255),   // 9: 파랑
            RGB(0,   255, 0),     // 10: 초록
            RGB(0,   255, 255),   // 11: 청록
            RGB(255, 0,   0),     // 12: 빨강
            RGB(255, 0,   255),   // 13: 자홍
            RGB(255, 255, 0),     // 14: 노랑
            RGB(255, 255, 255)    // 15: 흰색
        };

        return palette[color & 0x0F];
    }

    //지정한 크기의 고정폭 글꼴을 Windows에 요청하는 코드
    void WindowRenderOutput::CreateRenderFont()
    {
        LOGFONTW fontInfo{};
        fontInfo.lfWidth = static_cast<LONG>(characterSize.x);
        fontInfo.lfHeight = static_cast<LONG>(characterSize.y);
        fontInfo.lfWeight = FW_NORMAL;
        fontInfo.lfCharSet = DEFAULT_CHARSET;
        fontInfo.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
        fontInfo.lfQuality = NONANTIALIASED_QUALITY;

        fontHandle = CreateFontIndirectW(&fontInfo);

        if (fontHandle == nullptr)
        {
            throw std::runtime_error("Render font creation failed.");
        }
    }
    void WindowRenderOutput::ReleaseGlyphBuffer()
    {
        if (glyphDC != nullptr)
        {
            SelectObject(glyphDC, previousGlyphBitmap);
            DeleteObject(glyphBitmap);
            DeleteDC(glyphDC);
        }

        glyphDC = nullptr;
        glyphBitmap = nullptr;
        previousGlyphBitmap = nullptr;
        glyphBits = nullptr;
    }
    void WindowRenderOutput::CreateRenderWindow()
    {
        const HINSTANCE instance = GetModuleHandleW(nullptr);
        const wchar_t* className = L"HiwoongRenderWindow";

        WNDCLASSW windowClass{};
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = instance;
        windowClass.lpszClassName = className;
        windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        windowClass.hbrBackground =
            static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

        if (!RegisterClassW(&windowClass) &&
            GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        {
            throw std::runtime_error("Window class registration failed.");
        }

        windowHandle = CreateWindowExW(
            0,
            className,
            L"ASCII Doom",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            640, 480,
            nullptr, nullptr,
            instance, this
        );

        if (windowHandle == nullptr)
        {
            throw std::runtime_error("Render window creation failed.");
        }

        ShowWindow(windowHandle, SW_SHOW);
    }
    void WindowRenderOutput::DrawFrameContent(
        HDC dc, const RECT& area)
    {
        const int width = area.right - area.left;
        const int height = area.bottom - area.top;

        if (width <= 0 || height <= 0) return;
        if (!CreateGlyphBuffer(dc)) return;

        // 저장한 문자 그림으로 전체 화면을 조립한다.
        ComposeFramePixels(width, height);

        BITMAPINFO info{};
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = width;
        info.bmiHeader.biHeight = -height;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;

        // 완성된 픽셀 배열을 백 버퍼에 한 번에 전달한다.
        SetDIBitsToDevice(
            dc,
            0, 0, width, height,
            0, 0, 0, height,
            framePixels.data(),
            &info,
            DIB_RGB_COLORS
        );
    }

    void WindowRenderOutput::ComposeFramePixels(int width, int height)
    {
        if (width <= 0 || height <= 0) return;

        // 전체 화면을 검은색으로 준비한다.
        framePixels.assign(
            static_cast<std::size_t>(width) * height, 0);

        const int columns = static_cast<int>(screenSize.x);
        const int rows = static_cast<int>(screenSize.y);
        const int cellWidth = static_cast<int>(characterSize.x);
        const int cellHeight = static_cast<int>(characterSize.y);

        if (columns <= 0 || rows <= 0 ||
            cellWidth <= 0 || cellHeight <= 0 ||
            glyphDC == nullptr ||
            frameCells.size() !=
            static_cast<std::size_t>(columns) * rows)
        {
            return;
        }

        for (int y = 0; y < rows && y * cellHeight < height; ++y)
        {
            for (int x = 0; x < columns && x * cellWidth < width; ++x)
            {
                const auto& cell =
                    frameCells[static_cast<std::size_t>(y) * columns + x];

                const auto& glyph = GetGlyphPixels(cell);
                if (glyph.empty()) continue;

                // 창 밖으로 넘어가는 부분은 복사하지 않는다.
                const int copyWidth =
                    (std::min)(cellWidth, width - x * cellWidth);
                const int copyHeight =
                    (std::min)(cellHeight, height - y * cellHeight);

                for (int row = 0; row < copyHeight; ++row)
                {
                    DWORD* destination = framePixels.data() +
                        static_cast<std::size_t>(y * cellHeight + row)
                        * width + x * cellWidth;

                    const DWORD* source =
                        glyph.data() + row * cellWidth;

                    std::memcpy(
                        destination, source,
                        copyWidth * sizeof(DWORD));
                }
            }
        }
    }
    void WindowRenderOutput::DrawFrame(HDC dc)
    {
        RECT area{};
        if (!GetClientRect(windowHandle, &area)) return;

        const int width = area.right - area.left;
        const int height = area.bottom - area.top;

        if (!CreateBackBuffer(dc, width, height)) return;

        // 백 버퍼에 그림을 완성한다.
        DrawFrameContent(backBufferDC, area);

        // 완성된 그림을 창에 복사한다.
        BitBlt(
            dc, 0, 0, width, height,
            backBufferDC, 0, 0, SRCCOPY
        );
    }
}