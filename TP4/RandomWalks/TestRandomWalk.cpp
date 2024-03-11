#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

#include "RandomWalk.hpp"

int main()
{
    const size_t T = 10000;

    std::mt19937 g(time(0));
    RandomWalk S(0, 0.5);
    RandomWalkWithMin M(0, 0.5);

    std::ofstream sFile("RW.dat");
    std::ofstream mFile("RWmin.dat");
    for (size_t i = 0; i < T; ++i)
    {
        sFile << i << " " << S.Value() << "\n";
        mFile << i << " " << M.Value() << "\n";
        S.Update(g);
        M.Update(g);
    }

    return 0;
}
