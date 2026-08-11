#include "ScreenBuffer.h"
#include <cassert>

namespace Hiwoong
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize) : screenSize(screenSize)
	{
		////create ConsoleBuffer;

		// _In_ DWORD dwDesiredAccess,
		//	_In_ DWORD dwShareMode,
		//	_In_opt_ CONST SECURITY_ATTRIBUTES* lpSecurityAttributes,
		//	_In_ DWORD dwFlags,
		//	_Reserved_ LPVOID lpScreenBufferData

		screenBuffer = CreateConsoleScreenBuffer(
			GENERIC_READ| GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr
		);

		// Check Created screenBuffer right
		assert(screenBuffer != INVALID_HANDLE_VALUE);

		// initialize window size
		SMALL_RECT temprect = {};
		temprect.Top = 0;
		temprect.Left = 0;
		temprect.Right = 0;
		temprect.Bottom = 0;
		
		BOOL result = SetConsoleWindowInfo(screenBuffer, TRUE, &temprect);


		//Check rectSize
		assert(result == TRUE);

		// Setting screenBuffer size and Checking exception
		COORD coord = {};
		coord.X = static_cast<short>(screenSize.x);
		coord.Y = static_cast<short>(screenSize.y);

		result = SetConsoleScreenBufferSize(screenBuffer, coord);

		assert(result == TRUE);

		// initialize window size
		SMALL_RECT windowRect = {};
		windowRect.Top = 0;
		windowRect.Left = 0;
		windowRect.Right = static_cast<short>(screenSize.x - 1);
		windowRect.Bottom = static_cast<short>(screenSize.y - 1);

		result = SetConsoleWindowInfo(screenBuffer, TRUE, &windowRect);
		
		assert(result == TRUE);

		//Setting Cursor disable
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(screenBuffer, &info);

		info.bVisible = FALSE;
		SetConsoleCursorInfo(screenBuffer, &info);
	}
	ScreenBuffer::~ScreenBuffer()
	{
		if (screenBuffer)
		{
			CloseHandle(screenBuffer);
		}
	}
	void ScreenBuffer::Clear() const
	{
		//delete Console
		//fill empty charactor into whole screen

		// start position to fill character
		COORD coord = {};
		coord.X = 0;
		coord.Y = 0;

		// Setting letter count in screen
		DWORD writtenCount = 0;

		BOOL result = FillConsoleOutputCharacterA(
			screenBuffer,
			' ',
			screenSize.x * screenSize.y,
			coord,
			&writtenCount
		);
		assert(result == TRUE);
	}
	void ScreenBuffer::Draw(const CHAR_INFO* const charInfo) const
	{
		// Setting sent charAraay into screen

		// buffer size
		COORD bufferSize = {};
		bufferSize.X = static_cast<short>(screenSize.x);
		bufferSize.Y = static_cast<short>(screenSize.y);
		
		COORD bufferPosition = {};
		bufferPosition.X = 0;
		bufferPosition.Y = 0;

		//area setting
		SMALL_RECT writeRegion = {};
		writeRegion.Top = 0;
		writeRegion.Left = 0;
		writeRegion.Right = static_cast<short>(screenSize.x);
		writeRegion.Bottom = static_cast<short>(screenSize.y);

		BOOL result = WriteConsoleOutputA(
			screenBuffer,
			charInfo,
			bufferSize,
			bufferPosition,
			&writeRegion
		);
		assert(result == TRUE);
	}
}
