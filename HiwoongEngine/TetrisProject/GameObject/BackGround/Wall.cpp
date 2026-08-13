#include "Wall.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector2.h"
#include "Math/Color.h"


namespace Hiwoong
{
	Wall::Wall(const Vector2& position) : GameObject(position)
	{
		AddComponent<SpriteRendererComponent>("#", Color::Gray,1);
	}
}

