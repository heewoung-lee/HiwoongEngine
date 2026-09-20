#pragma once

#include "Animation/SpriteAnimationClip.h"

namespace Hiwoong
{
    struct MonsterAnimationSet
    {
        SpriteAnimationClip idle;
        SpriteAnimationClip attack;
        SpriteAnimationClip run;
        SpriteAnimationClip dead;
    };
}