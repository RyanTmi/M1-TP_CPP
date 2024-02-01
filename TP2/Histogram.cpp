#include "Histogram.hpp"

bool Histogram::operator+=(double x)
{
    (void) x;
    return true;
}

void Histogram::Reset()
{

}

std::ostream& operator<<(std::ostream& out, const Histogram& h)
{
    return out;
}
