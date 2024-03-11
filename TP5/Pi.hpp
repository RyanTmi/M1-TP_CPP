#pragma once

// #include <cmath>
#include <random>

class SquareInDisk
{
public:
    SquareInDisk()
         : m_U(0, 1)
    {}

    template <typename RNG>
    double operator()(RNG& G)
    {
        return (std::pow(m_U(G), 2) + std::pow(m_U(G), 2)) <= 1;
    }
private:
    std::uniform_real_distribution<double> m_U;
};
