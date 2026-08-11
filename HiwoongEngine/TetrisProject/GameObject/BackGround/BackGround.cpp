#include "BackGround.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
namespace Hiwoong
{
	BackGround::BackGround(const Vector2& position) : GameObject(position)
	{
		AddComponent<SpriteRendererComponent>(".",Color::White,0);
	}
}