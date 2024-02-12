#pragma once

#include <random>

class RandomWalk
{
public:
    RandomWalk(int startValue, double p)
        : m_CurrentTime(0), m_CurrentValue(startValue), m_StartValue(startValue), m_Distribution(p)
    {
    }

    int Value() const { return m_CurrentValue; }
    unsigned long Time() const { return m_CurrentTime; }
    virtual void Reset()
    {
        m_CurrentTime = 0;
        m_CurrentValue = m_StartValue;
    }

    template <typename RNG>
    void Update(RNG& g)
    {
        m_CurrentTime++;
        m_CurrentValue += (2 * m_Distribution(g) - 1);
    }
protected:
    unsigned long m_CurrentTime;
    int m_CurrentValue;

    int m_StartValue;
    std::bernoulli_distribution m_Distribution;
};

class RandomWalkWithMin : public RandomWalk
{
public:
    RandomWalkWithMin(int startValue, double p)
        : RandomWalk(startValue, p), m_Minimum(startValue)
    {
    }

    int Minimum() const { return m_Minimum; }

    void Reset() override
    {
        m_CurrentTime = 0;
        m_CurrentValue = m_StartValue;
        m_Minimum = m_StartValue;
    }

    template <typename RNG>
    void Update(RNG& g)
    {
        int direction = 2 * m_Distribution(g) - 1;
        m_CurrentValue += direction;
        if (direction < 0)
        {
            m_Minimum = m_CurrentValue;
        }
        m_CurrentTime++;
    }
protected:
    int m_Minimum;
};