#pragma once

#include "Core/Core.h"

namespace Hiwoong
{
	class Hiwoong_API Input
	{
		//declaraion friend Engine Class to accept PrecessInput method
		friend class Engine;
		
		//create struct
		//iskeydown in this frame
		// waskeydown in previous frame

		struct KeyState
		{
			bool isKeydown = false;
			bool wasKeydown = false;
		};

	public:
		//default declaration construct and destrecut
		Input();
		~Input();

		//Create Check KeyInput Method
		
		// Check GetKeydown
		bool GetKeyDown(int keycode) const;
		// Check GetKeyup
		bool GetKeyUP(int keycode) const;
		// Check Getkey
		bool GetKey(int keycode) const;

		float GetMouseDeltaX() const { return mouseDeltaX; }

		//Access singleton
		static Input& Get();

	private:

		//Check the Input in this frame
		void ProcessInput();

		// Save the previous key state
		void SavePreviousInput();

		void ProcessKeyboardInput();
		void ProcessMouseInput();

	private:
		//keyboard count
		int keyboardCnt = 256;
		//create Array Keystate 256
		KeyState keystates[256] = {};
		//static instance for singleton
		static Input* instance;


		//이동량은 현재 x위치 - 이전x위치.
		float mouseDeltaX = 0.0f;


		//이전 위치.
		int previousMouseX = 0;
		//이전위치를 한번이라도 읽었는지.
		bool hasPreviousMousePosition = false;


		//마우스의 원래 설정을 저장하고 드래그 선택 기능을 끄기
		void InitializeConsoleInput();
		//종료할 때 마우스의 원래 설정으로 복원하기
		void RestoreConsoleInput();

		// 변경하기전 콘솔입력 설정
		unsigned long originalConsoleMode = 0;

		// 원래 설정을 읽어 저장했는지 여부
		bool hasOriginalConsoleMode = false;



	};

}

