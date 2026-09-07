#pragma once

#include "Math/Vector3.h"

namespace Hiwoong
{
	class DoomMap;


	//각 타일별 문자에 따라, 해당 오브젝트를 생성하는,
	//전략패턴을 사용.
	class ITileBuilder
	{
	public: 
		virtual ~ITileBuilder() = default;

		//순수 가상 함수.
		virtual void Build(
			DoomMap& map,
			const Vector3& worldPosition
		) = 0;
	};


}