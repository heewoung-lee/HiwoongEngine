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
		~Input() = default;

		//Create Check KeyInput Method
		
		// Check GetKeydown
		bool GetKeyDown(int keycode) const;
		// Check GetKeyup
		bool GetKeyUP(int keycode) const;
		// Check Getkey
		bool GetKey(int keycode) const;


		//Access singleton
		static Input& Get();

	private:

		//Check the Input in this frame
		void ProcessInput();

		// Save the previous key state
		void SavePreviousInput();


	private:
		//keyboard count
		int keyboardCnt = 256;
		//create Array Keystate 256
		KeyState keystates[256] = {};
		//static instance for singleton
		static Input* instance;

	};

}

