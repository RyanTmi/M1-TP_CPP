#pragma once

#include <algorithm>
#include <iostream>
#include <vector>


template <typename Statistic, typename RandomVariable, typename Measurement, typename RNG>
void MonteCarlo(Statistic& res, RandomVariable& X, const Measurement& f, RNG& G, std::uint64_t n)
{
    for (std::uint64_t i = 0; i < n; ++i)
    {
        res += f(X(G));
    }
    res /= static_cast<double>(n);
}

class DoubleMeanVar
{
public:
    DoubleMeanVar(double x = 0.0)
         : m_M(x), m_V(0.0)
    {}

    double Mean() const { return m_M; }
    double Var() const { return m_V - m_M * m_M; }

    DoubleMeanVar& operator+=(double x)
    {
        m_M += x;
        m_V += x * x;
        return *this;
    }

    DoubleMeanVar& operator/=(double n)
    {
        m_M /= n;
        m_V /= n;
        return *this;
    }
private:
    double m_M;
    double m_V;
};

class Histogram
{
public:
    Histogram(double lowerBound, double upperBound, std::size_t n)
         : m_Boxes(n, 0.0), m_BoxCount(n),
           m_LowerBound(lowerBound), m_UpperBound(upperBound),
           m_BoxWidth((upperBound - lowerBound) / n)
    {}

    void operator+=(double x)
    {
        if (x >= m_LowerBound && x <= m_UpperBound)
        {
            m_Boxes[std::floor((x - m_LowerBound) / m_BoxWidth)]++;
        }
    }

    void operator/=(double n)
    {
        std::transform(m_Boxes.begin(), m_Boxes.end(), m_Boxes.begin(), [n](double i) {
            return i / n;
        });
    }

    friend std::ostream& operator<<(std::ostream& out, const Histogram& h);
private:
    std::vector<double> m_Boxes;
    std::size_t m_BoxCount;
    double m_LowerBound;
    double m_UpperBound;
    double m_BoxWidth;
};

std::ostream& operator<<(std::ostream& out, const Histogram& h)
{
    for (std::size_t i = 0; i < h.m_BoxCount; ++i)
    {
        out << h.m_LowerBound + i * h.m_BoxWidth << ' ' << h.m_Boxes[i] << '\n';
    }
    return out;
}
