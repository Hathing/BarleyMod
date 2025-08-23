#include <random>

inline int RandomInt(int min, int max)
{
    thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}
