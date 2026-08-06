#pragma once

#include "Core/Core.h"

namespace Hiwoong
{
	class Hiwoong_API Vector2
	{
		

	private:

		int x = 0;
		int y = 0;


		static Vector2 Zero;
		static Vector2 One;
		static Vector2 Right;
		static Vector2 Up;

	public:
		Vector2(int x = 0, int y = 0);
		~Vector2();

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const Vector2& other) const;
		Vector2 operator*(float value) const;
		Vector2 operator/(const Vector2& other) const;

		Vector2& operator= (const Vector2& other);


		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;
	};

}
