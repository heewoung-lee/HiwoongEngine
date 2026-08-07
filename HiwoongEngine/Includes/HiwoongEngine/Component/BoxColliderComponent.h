#pragma once

#include "Component/Component.h"

namespace Hiwoong
{
	//Manging Collision Range
	class Hiwoong_API BoxColliderComponent : public Component
	{
		TYPE_DECALRATIONS(BoxColliderComponent,Component)

	public:
		BoxColliderComponent(int width = 0);
		virtual ~BoxColliderComponent() = default;

		inline int GetWidth() const { return width; }
		inline void SetWidth(int newWidth) { width = newWidth; }

	protected:

		//width to check collision range
		int width = 0;

	};
}

