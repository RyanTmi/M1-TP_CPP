#include "Histogram.hpp"

#include <cmath>

bool Histogram::operator+=(double x)
{
    if (x < m_LowerBound || x > m_UpperBound)
    {
        m_OutCount++;
        return false;
    }
    m_Boxes[std::floor((x - m_LowerBound) / m_Delta)]++;
    return true;
}

void Histogram::Reset()
{
    m_OutCount = 0;
    m_Boxes.clear();
}

std::ostream& operator<<(std::ostream& out, const Histogram& h)
{
    for (std::size_t k = 0; k < h.m_Boxes.size(); ++k)
    {
        out << h.m_LowerBound + k * h.m_Delta + h.m_Delta / 2.0 << "\t" << h.m_Boxes[k] << "\n";
    }
    return out;
}
