#include "Gun.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"
#include "Math/Vector2.h"

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

		//총을 2D화면에 고정해서 붙여버림.
		const Vector2 size = Renderer::Get().GetScreenSize();
		GetComponent<TransformComponent>()->SetLocalPosition(
			Vector3(size.x / 2 -13, size.y - 15, 0)
		);
	}

}
