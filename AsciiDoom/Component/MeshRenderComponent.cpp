#include "MeshRenderComponent.h"
#include "GameObject/GameObject.h"
#include "Render/IRenderable3D.h"
#include "Render/ShaderData.h"
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

		u = std::clamp(u, 0.0f, 1.0f);

		// PNG는 위에서부터 저장되므로 세로 방향을 맞춘다.
		v = std::clamp(1.0f - v, 0.0f, 1.0f);

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

		const int red = texture->rgbPixels[byteIndex];
		const int green = texture->rgbPixels[byteIndex + 1];
		const int blue = texture->rgbPixels[byteIndex + 2];

		int bestDistance = INT_MAX;
		Color bestColor = Color::Black;

		for (int i = 0; i < 16; ++i)
		{
			const COLORREF candidate = ShaderData::ColorPalette[i];

			const int redDifference = red - GetRValue(candidate);
			const int greenDifference = green - GetGValue(candidate);
			const int blueDifference = blue - GetBValue(candidate);

			const int distance =
				redDifference * redDifference +
				greenDifference * greenDifference +
				blueDifference * blueDifference;

			if (distance < bestDistance)
			{
				bestDistance = distance;
				bestColor = static_cast<Color>(i);
			}
		}

		return bestColor;
	}
}
