#include "Gun.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"
#include "Math/Vector2.h"
#include "Scene/DoomScene.h"
#include <cassert>

namespace Hiwoong
{
	Gun::Gun()
	{
		AddComponent<SpriteRendererComponent>(
			R"(             _
            |#|
          __|#|__
         [=======]
         |-------|
         |:::::::|
         |-------|
        [|=======|]
       [#|_______|#]
      [##[=======]##]
     [###|#######|###]
    [##|:|=======|:|##]
   [###|:|#######|:|###]
  [####|:|=======|:|####]
 [#####|:|#######|:|#####])",
			Color::White,
			100
		);
	}

	void Gun::Start()
	{
		//씬불러와서 게임플레이 화면의 크기 가져오기.
		//타입 변환때문에 다이나믹 포인터 씀.
		const auto scene =
			std::dynamic_pointer_cast<DoomScene>(GetOwner());
		assert(scene != nullptr);

		const Vector2 size = scene->GetGameSize();

		GetComponent<TransformComponent>()->SetLocalPosition(
			Vector3(size.x / 2 - 13, size.y - 15, 0)
		);
	}

}
