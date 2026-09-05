#pragma once
#include "Math/Vector2.h"
#include <Windows.h>

namespace Hiwoong
{
	class ScreenBuffer
	{
	public: 
		ScreenBuffer(const Vector2& screenSize);
		~ScreenBuffer();

		// Initialize Screen
		void Clear() const;

		//Draw image to screen with textArray
		void Draw(const CHAR_INFO* const charInfo) const;

		// Getter.
		inline HANDLE GetScreenBuffer() const { return screenBuffer; }

		bool TryGetCharacterSize(Vector2& outSize) const;


	private:
		// Screen Buffer Handle(console Buffer)
		HANDLE screenBuffer = nullptr;

		// screenSize
		Vector2 screenSize;

	};
}

