#pragma once

#include "Component.h"
#include "Math/Vector2.h"
#include <memory>

namespace Hiwoong
{
	// Managing GameObject Position
	class Hiwoong_API TransformComponent : public Component
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

		inline std::shared_ptr<TransformComponent> GetParent() const { return parent.lock(); }
		inline void SetParent(std::weak_ptr<TransformComponent> newParent) { parent = newParent; }

	protected:
		// Base on Parent Transform 
		Vector2 localPosition;

		// Previous before Position
		Vector2 previousWorldPosition;

		//Get ParentTransform from Scene Graph
		std::weak_ptr<TransformComponent> parent;

	};
}

