#include "tripch.h"
#include "Random.h"

namespace TriEngine {
    std::mt19937 Random::s_Engine;
    std::uniform_int_distribution<uint32_t> Random::s_UintDist;


    void Random::Init()
    {
        s_Engine.seed(std::random_device()());
    }

    float Random::Float()
    {
        return (float)s_UintDist(s_Engine) / (float)std::numeric_limits<uint32_t>::max();
    }
}
