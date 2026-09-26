#pragma once

#include "PathFinder/Interfaces/IPathFinder.h"
#include<memory>


/// <summary>
/// 9.26일 추가 해당 클래스는 게임을 돌리는데 필요한
/// 구현체들을 직접 주입 하는데 사용함.
/// 프로젝트가 비대해짐에 따라.
/// 테스트를 할때에 테스트 진입점 까지 쉽지 않는 상황이 생기다보니 만들었음.
/// 로직은 추상에 의존하도록 한 다음
/// 추상의 구현체들끼리 단위테스트로 빠르게 테스트를 하고, 
/// 테스트를 마친 구현체들끼리 병합해서 사용.
/// 그리고 엔진 시작전 추상에 해당 구현체들을 주입하는 식으로 해서 엔진은 준비된 구현체들만 상호작용 하도록함
/// </summary>
namespace Hiwoong
{
	class DoomInstaller final 
	{
		// 구현체를 생성하고 주입만 할 것이기에 DoomInstaller의 인스턴스는 필요없음. 
		// 즉 DoomInstaller는 의존성 안내 표지판 역할
	public:
		static std::shared_ptr<const IPathFinder> CreatePathFinder();
	};

}

