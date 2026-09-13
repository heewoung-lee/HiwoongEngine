#pragma once

#include "Core/Core.h"
#include "IRenderOutput.h"
#include <vector>

namespace Hiwoong
{
    class Hiwoong_API WindowRenderOutput : public IRenderOutput
    {
    public:
        explicit WindowRenderOutput(const Vector2& characterSize);
        ~WindowRenderOutput() override;

        void Resize(const Vector2& screenSize) override;
        void Present(const CHAR_INFO* cells) override;
        Vector2 GetCharacterSize() const override;
        bool ProcessEvents() override;

        HWND GetWindowHandle() const
        {
            return windowHandle;
        }


    private:
        //이거 나중에 하다가 컬러에서 따로 처리할꺼 지금은 테스트용도.
        static COLORREF ToWindowColor(WORD color);

        void CreateRenderFont();
        HFONT fontHandle = nullptr;

    private:
        void CreateRenderWindow();//창 생성 과정을 묶는 메서드.

        //얘는 그림을 만들고
        void DrawFrameContent(HDC dc, const RECT& area);

        //얘는 완성된 그림을 표시
        void DrawFrame(HDC dc);


        //창의 그리기·닫기 등의 요청을 받는 함수.
        static LRESULT CALLBACK WindowProc(
            HWND window,
            UINT message,
            WPARAM wParam,
            LPARAM lParam
        );

        void ReleaseBackBuffer();


        //문자용 작업 공간을 해제하는 함수
        void ReleaseGlyphBuffer();
        //필요한 크기의 백 버퍼를 만들고, 같은 크기면 재사용하는 함수
        bool CreateBackBuffer(HDC referenceDC, int width, int height);
        //문자 한 칸 크기의 작업 공간을 만드는 함수
        bool CreateGlyphBuffer(HDC referenceDC);

        //문자 그림이 캐시에 있으면 가져오고, 없으면 만들어 저장하는 함수
        const std::vector<DWORD>& GetGlyphPixels(const CHAR_INFO& cell);

        //저장한 문자 그림들을 전체 화면에 복사하는 함수
        void ComposeFramePixels(int width, int height);


        //만든 창을 식별하는 값.
        HWND windowHandle = nullptr;

        //문자 크기
        Vector2 characterSize;

        Vector2 screenSize = Vector2::Zero;

        std::vector<CHAR_INFO> frameCells;

        //메모리에서 그림을 그릴 작업 공간.
        HDC backBufferDC = nullptr;
        //실제 픽셀을 저장할 이미지.
        HBITMAP backBufferBitmap = nullptr;
        //정리할 때 원래 연결을 복구하기 위한 값.
        HGDIOBJ previousBitmap = nullptr;

        //버퍼 크기. 같은 크기면 다시 만들지 않고 재사용
        int backBufferWidth = 0;
        int backBufferHeight = 0;


        // 문자와 색상 조합별로 완성된 픽셀 그림을 보관한다.
        std::vector<std::vector<DWORD>> glyphCache;

        // 문자 그림들을 복사해서 완성할 전체 화면의 픽셀 배열.
        std::vector<DWORD> framePixels;

        //문자 한 개를 그릴 작업 공간.
        HDC glyphDC = nullptr;
        //문자 한 칸 크기의 이미지.
        HBITMAP glyphBitmap = nullptr;
        //해제할 때 원래 연결을 복구할 값.
        HGDIOBJ previousGlyphBitmap = nullptr;
        //그 이미지의 실제 픽셀을 읽는 포인터.
        DWORD* glyphBits = nullptr;
    };
}