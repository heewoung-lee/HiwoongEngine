#pragma once

#include "Component/Component.h"
#include "Math/Color.h"
#include <string>
namespace Hiwoong
{
	class Hiwoong_API SpriteRendererComponent : Component
	{
		TYPE_DECALRATIONS(SpriteRendererComponent,Component)
		
	public:
		SpriteRendererComponent(
			const std::string& image = "",
			Color color = Color::White,
			int sortingOrder = 0
		);
		virtual ~SpriteRendererComponent() = default;

		virtual void Draw() override;

		//Getter/Setter
		inline const std::string& GetImage() const { return image; }
		inline void SetImage(const std::string& newImage) { image = newImage; }
		inline int GetWidth() const { return static_cast<int>(image.size()); }

		inline Color GetColor() const { return color; }
		inline void SetColor(Color newColor) { color = newColor; }

		inline int GetSortingOrder() const { return sortingOrder; }
		inline void SetSortingOrder(int newSortingOrder) { sortingOrder = newSortingOrder;}


	protected:
		// string to show Console
		std::string image;
		
		//Color
		Color color = Color::White;

		// sorting order
		int sortingOrder = 0;
	};
}

