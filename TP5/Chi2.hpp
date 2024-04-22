#pragma once

#include <random>


template <typename Real, std::size_t deg>
class Chi2_distribution
{
public:
    Chi2_distribution()
         : m_ND(0, 1)
    {}

    template <typename RNG>
    Real operator()(RNG& g)
    {
        Real result = 0;
        for (std::size_t i = 0; i < deg; ++i)
        {
            Real x = m_ND(g);
            result += x * x;
        }
        return result;
    }
private:
    std::normal_distribution<Real> m_ND;
};
