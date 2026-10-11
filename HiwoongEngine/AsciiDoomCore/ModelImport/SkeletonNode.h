#pragma once

#include <string>

namespace Hiwoong
{
	/// <summary>
	/// 각각의 관절의 부모가 누구인지 지정하기 위한 노드
	/// 이 구조를 통해 팔 -> 어깨 -> 몸통 -> 목 -> 머리 등 구별할 수 있음.
	/// 구별한 구조를 통해 일부 파츠가 움직이면 가중치에따라 움직임을 계산할 수 있음.
	/// </summary>
	struct SkeletonNode
	{
		std::string name;
		// 저장된 노드 목록에서 부모의 번호. 부모가 없으면 -1.
		int parentIdx = -1;

		// 부모를 기준으로 한 각 관절의 기본 위치.
		float localPositionX = 0.0f;
		float localPositionY = 0.0f;
		float localPositionZ = 0.0f;

		// 각 관절의 기본 회전
		float localRotationX = 0.0f;
		float localRotationY = 0.0f;
		float localRotationZ = 0.0f;
		float localRotationW = 1.0f;

		// 부모를 기준으로 한 관절의 기본 크기
		float localScaleX = 1.0f;
		float localScaleY = 1.0f;
		float localScaleZ = 1.0f;

	};
}