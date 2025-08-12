#pragma once

#include <cstdint>

namespace Smith
{
    //Random Number Generator class based on the Xorshift128 algorithm.
    class Random
    {
    public:
        static void Seed(uint32_t seed)
        {
            if (seed == 0)
                seed = 1;
            stateA = seed;
            stateB = seed ^ 0x9E3779B9;
            stateC = (seed << 13) ^ (seed >> 9);
            stateD = (seed << 5) ^ (seed >> 3);

            if ((stateA | stateB | stateC | stateD) == 0)
                stateD = 1;
        }

        static uint32_t Next()
        {
            uint32_t t = stateA ^ (stateA << 11);
            stateA = stateB;
            stateB = stateC;
            stateC = stateD;
            stateD = stateD & (stateD >> 19) ^ (t ^ (t >> 8));
            return stateD;
        }

        static int Int()
        {
            return Next();
        }

        static float Float()
        {
            return (Next() >> 8) * (1.0f / (1 << 24));
        }

        static int Range(int min, int max)
        {
            int range = max - min + 1;
            uint64_t threshold = -range % range;
            while (true)
            {
                uint32_t result = Int();
                if (result >= threshold)
                    return min + (result % range);
            }
        }

        static float Range(float min, float max)
        {
            return min + (max - min) * Float();
        }

        static bool Bool(float probability = 0.5f)
        {
            return Float() < probability;
        }

    private:
        static inline uint32_t stateA;
        static inline uint32_t stateB;
        static inline uint32_t stateC;
        static inline uint32_t stateD;
    };
} // namespace Smith
