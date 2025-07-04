#include "tripch.h"
#include "Random.h"

namespace TriEngine {
    thread_local std::random_device Random::s_Device;
    thread_local std::mt19937 Random::s_Engine(s_Device());

    template<class T>
    T::result_type Random::Generate(T distribution)
    {
        return distribution(s_Engine);
    }

    float Random::Float(float min, float max)
    {
        return Generate(std::uniform_real_distribution<float>(min, max));
    }

    int32_t Random::Int(int32_t min, int32_t max)
    {
        return Generate(std::uniform_int_distribution<int32_t>(min, max));
    }

    uint32_t Random::Uint(uint32_t min, uint32_t max)
    {
        return Generate(std::uniform_int_distribution<uint32_t>(min, max));
    }

    bool Random::Bool()
    {
        return Generate(std::bernoulli_distribution());
    }

}
