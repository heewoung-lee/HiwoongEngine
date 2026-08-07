#include "SpriteRendererComponent.h"

namespace Hiwoong
{
	SpriteRendererComponent::SpriteRendererComponent(
		const std::string& image, 
		Color color, 
		int sortingOrder) : image(image),color(color),sortingOrder(sortingOrder)
	{

	}
	void SpriteRendererComponent::Draw()
	{
		super::Draw();

		//TODO: Access Transform Getter 
	}
}