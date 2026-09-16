#pragma once

#include "Component/Component.h"
#include "Math/Color.h"
#include <string>
#include <vector>
namespace Hiwoong
{
	class Hiwoong_API SpriteRendererComponent : public Component
	{
		TYPE_DECALRATIONS(SpriteRendererComponent,Component)
		
	public:
		SpriteRendererComponent(
			const std::vector<std::string>& image,
			Color color = Color::White,
			int sortingOrder = 0
		);

		virtual ~SpriteRendererComponent() = default;

		virtual void Draw() override;

		inline const std::vector<std::string>& GetImage() const{return image;}
		inline void SetImage(const std::vector<std::string>& newImage){image = newImage;}

		int GetMaxWidth() const;

		inline Color GetColor() const { return color; }
		inline void SetColor(Color newColor) { color = newColor; }

		inline int GetSortingOrder() const { return sortingOrder; }
		inline void SetSortingOrder(int newSortingOrder) { sortingOrder = newSortingOrder;}


	protected:
		// string to show Console
		std::vector<std::string> image;
		
		//Color
		Color color = Color::White;

		// sorting order
		int sortingOrder = 0;
	};
}

