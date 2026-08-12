#include "Block.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteRendererComponent.h"


namespace Hiwoong
{
	Block::Block(const Vector2& position, const Color& color) : GameObject(position)
	{
		AddComponent<SpriteRendererComponent>("@", color, 1);
	}
}
