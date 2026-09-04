#include "Wall.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector2.h"
#include "Math/Color.h"


namespace Hiwoong
{
	Wall::Wall(const Vector2& position) : GameObject(Vector3(position.x,position.y,0))
	{
		AddComponent<SpriteRendererComponent>("#", Color::Gray,1);
	}
}

