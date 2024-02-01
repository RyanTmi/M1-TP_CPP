#pragma once

#include <vector>
#include <iostream>

class Histogram
{
public:
    Histogram(double lowerBound, double upperBound, std::size_t boxesCount)
        : m_LowerBound(lowerBound),
          m_UpperBound(upperBound),
          m_Delta((upperBound - lowerBound) / boxesCount),
          m_Boxes(boxesCount, 0),
          m_OutCount(0)
    {}
public:
    bool operator+=(double x);
    double LowerBound() const { return m_LowerBound; }
    double UpperBound() const { return m_LowerBound; }
    double BoxesCount() const { return m_Boxes.size(); }
    double OutCount() const { return m_OutCount; }
    void Reset();

    friend std::ostream& operator<<(std::ostream& out, const Histogram& h);
private:
    double m_LowerBound;
    double m_UpperBound;
    double m_Delta;
    std::vector<double> m_Boxes;
    std::size_t m_OutCount;
};
