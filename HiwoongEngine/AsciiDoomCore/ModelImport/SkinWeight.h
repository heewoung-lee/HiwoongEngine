#pragma once

namespace Hiwoong
{
	struct SkinWeight
	{
		//SkeletonNode 목록에서 영향을 주는 관절 번호.
		int nodeIdx = -1;

		// 이 관절의 영향 비율
		float weight = 0.0f;
	};


}