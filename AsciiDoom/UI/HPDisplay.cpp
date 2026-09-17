#include "HPDisplay.h"
#include "Component/SpriteRendererComponent.h"
#include "UI/BigTextDisplayComponent.h"
#include "UI/HpDisplayComponent.h"
#include "Player/Player.h"
#include <string>


namespace Hiwoong
{
    HPDisplay::HPDisplay(const std::shared_ptr<Player>& player, int scale)
    {
        AddComponent<SpriteRendererComponent>(
            std::vector<std::string>{},
            Color::Green,
            150
        );

        // HPDisplay
        AddComponent<BigTextDisplayComponent>(
            "HP " + std::to_string(player->GetCurrentHp()), scale);//글자를 크게 만들어서 스프라이트 렌더러에게 넘겨줌.

        AddComponent<HpDisplayComponent>(player);//현재 HP를 전달하는역할을 하는 컴포넌트 
    }
}

