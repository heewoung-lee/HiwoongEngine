#pragma once

#include "Component/SpriteRendererComponent.h"
#include "Render/RenderView.h"
#include "Component/TransformComponent.h"
#include "Render/IRenderable3D.h"
#include "Render/Mesh.h"
#include <memory>


namespace Hiwoong
{
	class Hiwoong_API SpriteRenderer3DComponent : public SpriteRendererComponent, public IRenderable3D
	{
		TYPE_DECALRATIONS(SpriteRenderer3DComponent, SpriteRendererComponent)

	public:
		//부모 생성자 재사용
		using SpriteRendererComponent::SpriteRendererComponent;
		void Start() override;
		void Draw() override;

		const Mesh& GetMesh() const override;

		Matrix4x4 GetModelMatrix(
			const RenderView& renderView
		) const override;

		Color GetRenderColor() const override;

		bool TryGetCharactor(
			float u,
			float v,
			float brightness,
			char& outCharacter
		) const override;

		void SetVisible(bool visible)
		{
			isVisible = visible;
		}

		bool IsVisible() const
		{
			return isVisible;
		}

	private:
		std::shared_ptr<TransformComponent> transform;
		Mesh mesh;
		bool isVisible = true;
	};

}
