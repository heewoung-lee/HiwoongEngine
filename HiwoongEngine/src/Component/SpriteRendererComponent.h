#pragma once

#include "Component/Component.h"
#include "Math/Color.h"
#include "Render/SpriteFrame.h"
#include <string>
#include <vector>
namespace Hiwoong
{
	class Hiwoong_API SpriteRendererComponent : public Component
	{
		TYPE_DECALRATIONS(SpriteRendererComponent,Component)
		
	public:
		SpriteRendererComponent(
			const SpriteFrame& frame,
			int sortingOrder = 0
		);

		virtual ~SpriteRendererComponent() = default;

		virtual void Draw() override;

		inline const SpriteFrame& GetImage() const
		{
			return image;
		}

		inline void SetImage(const SpriteFrame& newImage)
		{
			image = newImage;
		}

		int GetMaxWidth() const;

		inline Color GetColor() const { return color; }
		inline void SetWholeColor(Color newColor)
		{
			color = newColor;

			for (std::vector<SpriteCell>& row : image.cells)
			{
				for (SpriteCell& cell : row)
				{
					cell.color = newColor;
				}
			}
		}

		inline int GetSortingOrder() const { return sortingOrder; }
		inline void SetSortingOrder(int newSortingOrder) { sortingOrder = newSortingOrder;}


	protected:
		// string to show Console
		SpriteFrame image;
		
		//Color
		Color color = Color::White;

		// sorting order
		int sortingOrder = 0;
	};
}

