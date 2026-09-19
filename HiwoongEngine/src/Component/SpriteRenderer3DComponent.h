#pragma once

#include "Component/SpriteRendererComponent.h"
#include "Render/RenderView.h"
#include "Component/TransformComponent.h"
#include <memory>


namespace Hiwoong
{
	class Hiwoong_API SpriteRenderer3DComponent : public SpriteRendererComponent
	{
		TYPE_DECALRATIONS(SpriteRenderer3DComponent, SpriteRendererComponent)

	public:
		//부모 생성자 재사용
		using SpriteRendererComponent::SpriteRendererComponent;
		void Start() override;
		void Draw() override;
		void Render(const RenderView& renderView);


	private:
		std::shared_ptr<TransformComponent> transform;
	};

}
