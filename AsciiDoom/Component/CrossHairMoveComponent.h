#pragma once

#include "Component/Component.h"

namespace Hiwoong
{
	class CrossHairMoveComponent : public Component
	{
		TYPE_DECALRATIONS(CrossHairMoveComponent,Component)

	public :
		CrossHairMoveComponent(float minY, float maxY);
		~CrossHairMoveComponent() = default;

		//마우스의 열이동 Y축 이동에 대한 업데이트를 맡게될 컴포넌트
		void Update(double deltaTime) override;


	private:
		float sensitivity = 0.1f; //감도

		float minY = 0.0f; //최소 반경
		float maxY = 0.0f; //최대 반경. 0 으로 한 이유는 만약 내가 의도한 동작이 안되면 여길보고 수정하라는뜻으로
	};
}


