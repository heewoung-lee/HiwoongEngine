#pragma once

#include "Math/Vector2.h"
#include "GameObject/GameObject.h"
#include <array>

namespace Hiwoong
{
	class TetrisModule : public GameObject
	{
	public:
		TetrisModule(const Vector2& position): GameObject(position)
		{

		}


		virtual ~TetrisModule() = default;

		virtual const std::array<Vector2, 4>& GetBlockPosition() const = 0;

		virtual Color GetColor() = 0;

		void Start() override;
		void Update(double deltaTime) override;
	protected:
		double dropInterval = 1.0;
		double elapsedDropTime = 0.0;
	};

}