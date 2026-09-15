#include "Block.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteRendererComponent.h"

#include "Math/Vector2.h"
namespace Hiwoong
{
	Block::Block(const Vector2& position, const Color& color) : GameObject(Vector3(position.x, position.y,0))
	{
		AddComponent<SpriteRendererComponent>("@", color, 1);
	}
}
