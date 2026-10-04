#pragma once

#include "Component/Component.h"
#include "Math/Vector3.h"

#include <vector>
#include <memory>

namespace Hiwoong
{
    class IPathFinder;
    class INavigationMap;

    class NavAIComponent : public Component
    {
        TYPE_DECALRATIONS(NavAIComponent, Component)

    public:
        void Start() override;

        // 시작 위치에서 목적지까지의 월드 좌표 경로를 반환한다.
        std::vector<Vector3> FindPath(
            const Vector3& startPosition,
            const Vector3& targetPosition
        ) const;

    private:
        //길찾기에 필요한 알고리즘과 맵을 보관
        void InitReferences();

        std::shared_ptr<const IPathFinder> pathFinder;
        std::weak_ptr<const INavigationMap> navigationMap;
    };
}