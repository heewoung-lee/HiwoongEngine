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



	//Input construct
	
	//getkeydown

	//getkeyup

	//getkey

	//acceces singleton
	Input& Input::Get()
	{
		assert(instance != nullptr);
		return *instance;
	}


	void Input::ProcessInput()
	{
		for (int ix = 0; ix < keyboardCnt; ++ix)
		{
			keystates[ix].isKeydown = (GetAsyncKeyState(ix) & 0x8000) != 0;
		}
	}

	void Input::SavePreviousInput()
	{
		for (KeyState& state : keystates)
		{
			state.wasKeydown = state.isKeydown;
		}
	}
	
}