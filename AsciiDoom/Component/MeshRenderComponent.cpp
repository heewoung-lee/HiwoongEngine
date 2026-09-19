#include "Component/SpriteRenderer3DComponent.h"
#include "MeshRenderComponent.h"
#include "GameObject/GameObject.h"
#include "Render/IRenderable3D.h"
#include <algorithm>
#include <vector>
#include <cassert>


namespace Hiwoong
{
	MeshRenderComponent::MeshRenderComponent(
		const Mesh& mesh,
		Color color)
		: mesh(mesh), color(color)
	{
	}

	void MeshRenderComponent::Start()
	{
		super::Start();

		transform = GetComponent<TransformComponent>();
		assert(transform != nullptr);
	}

	const Mesh& MeshRenderComponent::GetMesh() const
	{
		return mesh;
	}

	Matrix4x4 MeshRenderComponent::GetModelMatrix(
		const RenderView& renderView) const
	{
		assert(transform != nullptr);

		return Matrix4x4::Translation(transform->GetWorldPosition());
	}

	Color MeshRenderComponent::GetRenderColor() const
	{
		return color;
	}

	bool MeshRenderComponent::TryGetCharactor(
		float u,
		float v,
		float brightness,
		char& outCharacter) const
	{
		static const std::vector<char> shadeCharacters =
		{
			' ', '.', ':', '*', '#', '@'
		};

		brightness = std::clamp(brightness, 0.0f, 1.0f);

		const std::size_t index = static_cast<std::size_t>(
			brightness * static_cast<float>(shadeCharacters.size() - 1)
			);

		outCharacter = shadeCharacters[index];
		return true;
	}
}
