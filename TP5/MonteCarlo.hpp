#pragma once

#include <cstdint>

template <typename Statistic, typename RandomVariable, typename Measurement, typename RNG>
void MonteCarlo(Statistic& res, RandomVariable& X, const Measurement& f, RNG& G, std::uint64_t n)
{
    for (std::uint64_t i = 0; i < n; ++i)
    {
        res += f(X(G));
    }
    res /= static_cast<double>(n);
}
