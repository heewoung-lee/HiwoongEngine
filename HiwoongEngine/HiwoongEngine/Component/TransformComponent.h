#pragma once

#include "Component.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include <memory>

namespace Hiwoong
{
	// Managing GameObject Position
	class Hiwoong_API TransformComponent : public Component
	{
		TYPE_DECALRATIONS(TransformComponent, Component)

	public:
		TransformComponent(
			Vector3 position = Vector3::Zero,
			Vector3 rotation = Vector3::Zero,
			Vector3 scale = Vector3(1, 1, 1));
		virtual ~TransformComponent() = default;


		// Save previous World Position.
		void SavePreviousWorldPosition();

		//Getter/Setter
		inline Vector3 GetLocalPosition() const{return localPosition;}
		inline const Vector3& GetRotation() const { return rotation; }
		inline const Vector3& GetScale() const { return scale; }

		inline Vector3 GetPreviousWorldPosition() const { return previousWorldPosition; }
		inline std::shared_ptr<TransformComponent> GetParent() const { return parent.lock(); }
		inline void SetParent(std::weak_ptr<TransformComponent> newParent) { parent = newParent; }

		inline void SetLocalPosition(const Vector3& newPosition) { localPosition = newPosition; }
		inline void SetRotation(const Vector3& newRotation) { rotation = newRotation; }
		inline void SetScale(const Vector3& newScale) { scale = newScale; }

		Vector3 GetWorldPosition() const;
		void SetWorldPosition(const Vector3& newPosition);


		Matrix4x4 GetModelMatrix() const;

	private:

		// Previous before Position
		Vector3 previousWorldPosition;

		//Get ParentTransform from Scene Graph
		std::weak_ptr<TransformComponent> parent;

		// Base on Parent Transform 
		Vector3 localPosition;
		Vector3 rotation;
		Vector3 scale;

	};

}

