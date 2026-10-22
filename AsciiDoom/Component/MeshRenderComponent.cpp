#include "MeshRenderComponent.h"
#include "GameObject/GameObject.h"
#include "Render/IRenderable3D.h"
#include "Render/ShaderData.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <cassert>
#include <climits>


namespace Hiwoong
{
	MeshRenderComponent::MeshRenderComponent(
		const Mesh& mesh,
		Color color,
		std::shared_ptr<const ModelTexture> texture
	)
		: mesh(mesh),
		color(color),
		texture(texture)
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

		return transform->GetModelMatrix();
	}

	Color MeshRenderComponent::GetRenderColor() const
	{
		return color;
	}

	bool MeshRenderComponent::TryGetCharactor(
		float u,
		float v,
		float brightness,
		char& outCharacter,
		Color& outColor) const
	{
		const std::vector<char>& shadeCharacters = ShaderData::DetailedShadeCharacters;

		brightness = std::clamp(brightness, 0.0f, 1.0f);

		const std::size_t index = static_cast<std::size_t>(
			brightness * static_cast<float>(shadeCharacters.size() - 1)
			);

		outCharacter = shadeCharacters[index];
		outColor = GetTextureColor(u, v);
		return true;
	}
	Color MeshRenderComponent::GetTextureColor(float u, float v) const
	{
		if (texture == nullptr)
			return color;

		assert(texture->width > 0 && texture->height > 0);
		assert(texture->rgbPixels.size() ==
			texture->width * texture->height * 3);

		//반복 설정이 켜져 있으면 1.3 → 0.3, -0.3 → 0.7처럼 이미지 범위 안으로 돌아오게 하는 계산.
		if (texture->info.repeatU)
		{
			u -= std::floor(u);
		}
		else
		{
			u = std::clamp(u, 0.0f, 1.0f);
		}

		if (texture->info.repeatV)
		{
			v -= std::floor(v);
		}
		else
		{
			v = std::clamp(v, 0.0f, 1.0f);
		}

		// PNG의 위아래 방향을 맞춘다.
		v = 1.0f - v;

		const std::size_t column = (std::min)(
			static_cast<std::size_t>(
				u * static_cast<float>(texture->width)),
			texture->width - 1
			);

		const std::size_t row = (std::min)(
			static_cast<std::size_t>(
				v * static_cast<float>(texture->height)),
			texture->height - 1
			);

		const std::size_t byteIndex =
			(row * texture->width + column) * 3;

		return Color{
					texture->rgbPixels[byteIndex],
					texture->rgbPixels[byteIndex + 1],
					texture->rgbPixels[byteIndex + 2]
		};
	}
	void MeshRenderComponent::SetVertexPositions(
		const std::vector<ModelVertex>& vertices
	)
	{
		assert(vertices.size() == mesh.vertices.size());

		if (vertices.size() != mesh.vertices.size())
			return;

		for (std::size_t i = 0; i < vertices.size(); ++i)
		{
			const ModelVertex& vertex = vertices[i];

			mesh.vertices[i].position = Vector3(
				vertex.x,
				vertex.y,
				vertex.z
			);
		}
	}
}
