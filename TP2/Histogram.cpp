#include "Histogram.hpp"

bool Histogram::operator+=(double x)
{
    for (std::size_t k = 0; k < m_Boxes.size(); ++k)
    {
        if (m_LowerBound + k * m_Delta <= x && x < m_LowerBound + (k + 1) * m_Delta)
        {
            m_Boxes[k]++;
            return true;
        }
    }
    m_OutCount++;
    return false;
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
