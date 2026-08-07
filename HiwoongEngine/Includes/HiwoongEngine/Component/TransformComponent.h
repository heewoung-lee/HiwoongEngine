#pragma once

#include "Component.h"
#include "Math/Vector2.h"

namespace Hiwoong
{
	// Managing GameObject Position
	class Hiwoong_API TransformComponent : Component
	{
		TYPE_DECALRATIONS(TransformComponent, Component)

	public:
		TransformComponent(const Vector2& localPosition = Vector2::Zero);
		virtual ~TransformComponent() = default;

		// Save previous World Position.
		void SavePreviousWorldPosition();

		//Getter/Setter
		inline Vector2 GetLocalPosition() const{return localPosition;}
		inline void SetLocalPosition(const Vector2& newPosition){localPosition = newPosition;}

		Vector2 GetWorldPosition() const;
		void SetWorldPosition(const Vector2& newPosition);

		inline Vector2 GetPreviousWorldPosition() const { return previousWorldPosition; }

	protected:
		// Base on Parent Transform 
		Vector2 localPosition;

		// Previous before Position
		Vector2 previousWorldPosition;

		int sortingOrder = -1;

	};
}

