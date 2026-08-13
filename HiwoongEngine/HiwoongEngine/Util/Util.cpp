#include "Util/Util.h"
#include <random>

namespace Util
{
    std::mt19937& GetRandomEngine()
    {
        static std::mt19937 engine;
        return engine;
    }

    void SetRandomSeed()
    {
        std::random_device randomDevice;
        GetRandomEngine().seed(randomDevice());
    }

    int RandomRange(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(GetRandomEngine());
    }

    float RandomRange(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(GetRandomEngine());
    }
}