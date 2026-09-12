#include "Input.h"
#include <cassert>
#include <Windows.h>

namespace Hiwoong
{
	//singleton initialize 
	Input* Input::instance = nullptr;

	Input::Input()
	{
		assert(instance == nullptr);
		instance = this;
		InitializeConsoleInput();
	}
	
	Input::~Input()
	{
		ClipCursor(nullptr);
		RestoreConsoleInput();
		instance = nullptr;
	}

	//Create Check KeyInput Method

	// Check GetKeydown
	bool Input::GetKeyDown(int keycode) const
	{
		return keystates[keycode].isKeydown == true && keystates[keycode].wasKeydown == false;
	}
	// Check GetKeyup
	bool Input::GetKeyUP(int keycode) const
	{
		return keystates[keycode].isKeydown == false && keystates[keycode].wasKeydown == true;
	}
	// Check Getkey
	bool Input::GetKey(int keycode) const
	{
		return keystates[keycode].isKeydown;
	}

	//getkey

	//acceces singleton
	Input& Input::Get()
	{
		assert(instance != nullptr);
		return *instance;
	}

	void Input::SetMouseLocked(bool locked)
	{
		isMouseLocked = locked;
		mouseDeltaX = 0.0f;
		hasPreviousMousePosition = false;

		//가두기 해제
		if (!isMouseLocked)
		{
			ClipCursor(nullptr);
		}
	}

	//PrecessInput
	void Input::ProcessInput()
	{
		ProcessMouseInput();
		ProcessKeyboardInput();
	}

	//Save currentPressed button state to Previous Button
	void Input::SavePreviousInput()
	{
		for (KeyState& state : keystates)
		{
			state.wasKeydown = state.isKeydown;
		}
	}

	void Input::ProcessKeyboardInput()
	{
		for (int ix = 0; ix < keyboardCnt; ++ix)
		{
			keystates[ix].isKeydown = (GetAsyncKeyState(ix) & 0x8000) != 0;
		}
	}

	void Input::ProcessMouseInput()
	{
		mouseDeltaX = 0.0f;

		// 잠금이 꺼져 있으면 가두기와 중앙 복귀를 건너뛴다.
		if (isMouseLocked == false)
			return;

		//게임 콘솔 창을 가져옴
		const HWND gameWindow = GetConsoleWindow();

		//현재 사용중인 창을 가져옴
		if (gameWindow == nullptr || GetForegroundWindow() != gameWindow)
		{
			hasPreviousMousePosition = false;
			ClipCursor(nullptr);
			return;
		}

		RECT clientRect{};
		POINT origin{};

		//GetClientRect는 제목 표시줄을 제외한 창 내부 크기
		//ClientToSceen은 그 내부 왼쪽위가 모니터 어디에 있는지.
		if (GetClientRect(gameWindow, &clientRect) == false
			|| ClientToScreen(gameWindow, &origin) == false)
		{
			hasPreviousMousePosition = false;
			ClipCursor(nullptr);
			return;
		}

		//둘을 합치면 커서를 가둘 범위
		RECT cursorArea{
			origin.x,
			origin.y,
			origin.x + clientRect.right,
			origin.y + clientRect.bottom
		};



		//마우스 위치를 읽는데 성공하면 위치 갱신
		POINT mousePosition{};
		if (GetCursorPos(&mousePosition) == false)
		{
			hasPreviousMousePosition = false;
			ClipCursor(nullptr);
			return;
		}

		if (ClipCursor(&cursorArea) == false)
		{
			hasPreviousMousePosition = false;
			ClipCursor(nullptr);
			return;
		}

		if (hasPreviousMousePosition == true)
		{
			mouseDeltaX = static_cast<float>(
				mousePosition.x - previousMouseX
				);
		}




		const int centerX =
			cursorArea.left + (cursorArea.right - cursorArea.left) / 2;

		const int centerY =
			cursorArea.top + (cursorArea.bottom - cursorArea.top) / 2;


		//마우스를 중앙값으로 옮긴다.
		if (SetCursorPos(centerX, centerY) == false)
		{
			mouseDeltaX = 0.0f;
			hasPreviousMousePosition = false;
			ClipCursor(nullptr);
			return;
		}

		previousMouseX = centerX;
		hasPreviousMousePosition = true;
	}
	//흐름은 원래 설정 저장 → 
	// 드래그 선택 옵션 끄기 → 
	// 변경된 설정 적용
	void Input::InitializeConsoleInput()
	{
		
		const HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

		if (inputHandle == nullptr ||
			inputHandle == INVALID_HANDLE_VALUE)
			return;

		if (!GetConsoleMode(inputHandle, &originalConsoleMode))
			return;

		hasOriginalConsoleMode = true;

		DWORD mode = originalConsoleMode;
		mode |= ENABLE_EXTENDED_FLAGS;
		mode &= ~ENABLE_QUICK_EDIT_MODE;

		if (!SetConsoleMode(inputHandle, mode))
		{
			OutputDebugStringA("Failed to disable console Quick Edit.\n");
		}
	}

	//설정 되돌리기 함수.
	//끝나면 이전 마우스 설정으로 되돌아감
	void Input::RestoreConsoleInput()
	{
		if (!hasOriginalConsoleMode)
			return;

		const HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

		if (!SetConsoleMode(inputHandle, originalConsoleMode))
		{
			OutputDebugStringA("Failed to restore console input mode.\n");
			return;
		}

		hasOriginalConsoleMode = false;
	}
	
}