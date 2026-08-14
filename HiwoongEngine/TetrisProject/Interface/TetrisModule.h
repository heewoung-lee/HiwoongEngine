#pragma once

#include "Math/Vector2.h"
#include "GameObject/GameObject.h"
#include <functional>
#include <utility>
#include <array>

namespace Hiwoong
{
	class Block;

	class TetrisModule : public GameObject
	{
	public:

		using LockedCallback = std::function<void()>;
		void AddOnLocked(LockedCallback callback);

		TetrisModule(const Vector2& position) : GameObject(position) {}
		virtual ~TetrisModule() = default;
		
		const std::array<Vector2, 4>& GetBlockPosition() const
		{
			return blockPositions;
		} 
		virtual Color GetColor() = 0;

		bool CanMove(const Vector2& direction) const;
		bool TryMove(const Vector2& direction);
		//Rotation Block
		bool TryRotate();

		void Start() override;
		void Update(double deltaTime) override;

	protected:
		double dropInterval = 1.0;
		double elapsedDropTime = 0.0;
		std::array<Vector2, 4> blockPositions;
		bool canRotate = true;

		
		std::array<std::weak_ptr<Block>, 4> blocks;

		//Rotation Pivot 
		Vector2 rotationPivot = Vector2::Zero;

	private:
		bool isLocked = false;
		void Lock();
		std::vector<LockedCallback> onLockedCallbacks;
		void BroadcastOnLocked();
		
	};




}