#pragma once

#include <iostream>
#include <random>
#include <vector>


class Markov2States
{
public:
    Markov2States(int x0 = 1, double a0 = 0.5, double b0 = 0.5)
         : m_State(x0), m_UA(a0), m_UB(b0)
    {}

    template <typename RNG>
    int operator()(RNG& g)
    {
        if (m_State == 1)
        {
            m_State = m_UA(g) ? 2 : 1;
        }
        else
        {
            m_State = m_UB(g) ? 1 : 2;
        }
        return m_State;
    }
private:
    int m_State;  // 1 or 2
    std::bernoulli_distribution m_UA;
    std::bernoulli_distribution m_UB;
};

class Stat2States
{
public:
    Stat2States()
         : m_Visit1(0), m_Visit2(0), m_Total(0)
    {}

    void operator+=(int s)
    {
        s == 1 ? ++m_Visit1 : ++m_Visit2;
    }

    void operator/=(double n)
    {
        m_Total += n;
    }

    double Visit1() const { return static_cast<double>(m_Visit1) / m_Total; }
    double Visit2() const { return static_cast<double>(m_Visit2) / m_Total; }

    friend std::ostream& operator<<(std::ostream& out, const Stat2States& s);
private:
    std::size_t m_Visit1;
    std::size_t m_Visit2;
    std::size_t m_Total;
};

std::ostream& operator<<(std::ostream& out, const Stat2States& s)
{
    return out << s.Visit1() << ' ' << s.Visit2();
}

template<std::size_t N>
class Ising1D
{
public:
    using State = std::vector<std::int8_t>;
public:
    Ising1D(double beta, double h)
         : m_U(0, N), m_State(N, 1), m_Beta(beta), m_H(h)
    {}

    template <typename RNG>
    State& operator()(RNG& g)
    {
        std::size_t k = m_U(g);
        int8_t xk = m_State[k];
        int8_t xk1 = 0, xk2 = 0;

        if (k > 0)
        {
            xk1 = m_State[k - 1];
        }
        if (k < N - 1)
        {
            xk2 = m_State[k + 1];
        }
        double p = std::min(1.0, std::exp(-2 * m_Beta * xk * (xk1 + xk2) - 2 * m_H * xk));
        std::bernoulli_distribution b(p);
        if (b(g))
        {
            m_State[k] *= -1;
        }
        return m_State;
    }
private:
    std::uniform_int_distribution<int> m_U;
    State m_State;
    double m_Beta;
    double m_H;
};
