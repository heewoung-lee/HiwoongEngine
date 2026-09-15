#include "BackGround.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
namespace Hiwoong
{
	BackGround::BackGround(const Vector2& position) : GameObject(Vector3(position.x,position.y,0))
	{
		AddComponent<SpriteRendererComponent>(".",Color::White,0);
	}
}