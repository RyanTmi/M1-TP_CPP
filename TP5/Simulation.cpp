#include <ctime>
#include <iostream>
#include <random>

#include "MonteCarlo.hpp"
#include "Pi.hpp"

int main()
{
    SquareInDisk A;
    double piApprox = 0;
    std::mt19937_64 g(time(0));

    auto f = [](double x) { return 4 * x; };
    MonteCarlo(piApprox, A, f, g, 1000000000);

    std::cout << piApprox << '\n';

    return 0;
}