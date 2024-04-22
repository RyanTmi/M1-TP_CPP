#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

#include "Chi2.hpp"
#include "MCMC.hpp"
#include "MonteCarlo.hpp"
#include "Pi.hpp"

template <typename RNG>
void Pi_MC(std::size_t n, RNG& g);

template <typename RNG>
void I1_MC(std::size_t n, RNG& g);

template <typename RNG>
void I2_MC(std::size_t n, RNG& g);

template <typename RNG>
void Normal_MC(std::size_t n, RNG& g);

template <typename RNG>
void Chi2_MC(std::size_t n, RNG& g);

template <typename RNG>
void MC_MC(std::size_t n, RNG& g);

template <typename RNG>
void Ising_MC(std::size_t n, RNG& g);

int main()
{
    std::size_t n = 100000000;
    std::mt19937_64 g(time(0));

    // Pi_MC(n, g);
    // I1_MC(n, g);
    // I2_MC(n, g);
    // Normal_MC(n, g);
    // Chi2_MC(n, g);
    // MC_MC(n, g);
    Ising_MC(n, g);

    return 0;
}

template <typename RNG>
void Pi_MC(std::size_t n, RNG& g)
{
    SquareInDisk a;
    double piApprox = 0;

    auto f = [](double x) { return 4 * x; };
    MonteCarlo(piApprox, a, f, g, n);
    std::cout << piApprox << '\n';

    DoubleMeanVar mv;
    MonteCarlo(mv, a, f, g, n);
    std::cout << "[" << mv.Mean() - 1.96 * std::sqrt(mv.Var() / n)
              << "; " << mv.Mean() + 1.96 * std::sqrt(mv.Var() / n) << "]\n";
}

template <typename RNG>
void I1_MC(std::size_t n, RNG& g)
{
    std::uniform_real_distribution<double> u(0, 1);
    auto f = [](double x) { return std::log(1 + x * x); };

    double i = 0;
    MonteCarlo(i, u, f, g, n);
    std::cout << "I1 = " << i << '\n';
}

template <typename RNG>
void I2_MC(std::size_t n, RNG& g)
{
    std::exponential_distribution<double> e(1);
    std::uniform_real_distribution<double> u(0, 1);

    auto coupleVar = [&](RNG& g) { return std::make_pair(e(g), u(g)); };
    auto f = [](double x) { return std::log(1 + x * x); };

    double i = 0;
    MonteCarlo(i, coupleVar, f, g, n);
    std::cout << "I2 = " << i << '\n';
}

template <typename RNG>
void Normal_MC(std::size_t n, RNG& g)
{
    Histogram h(-3, 3, 50);
    std::normal_distribution<double> nd(0, 1);
    MonteCarlo(h, nd, [](double x) { return x; }, g, n);

    std::ofstream out("histogram.dat");
    out << h;
}

template <typename RNG>
void Chi2_MC(std::size_t n, RNG& g)
{
    Histogram h1(0, 10, 50);
    Histogram h2(0, 10, 50);
    Chi2_distribution<double, 3> chi3;
    Chi2_distribution<double, 6> chi6;
    auto id = [](double x) { return x; };

    MonteCarlo(h1, chi3, id, g, n);
    std::ofstream out3("chi3.dat");
    out3 << h1;

    MonteCarlo(h2, chi6, id, g, n);
    std::ofstream out6("chi6.dat");
    out6 << h2;
}

template <typename RNG>
void MC_MC(std::size_t n, RNG& g)
{
    double aa = 0.7;
    double bb = 0.2;
    Markov2States mc(1, aa, bb);
    Stat2States s;
    auto id = [](double x) { return x; };

    MonteCarlo(s, mc, id, g, n);
    std::cout << bb / (aa + bb) << ' ' << aa / (aa + bb) << '\n';
    std::cout << s << '\n';
}

template <typename RNG>
void Ising_MC(std::size_t n, RNG& g)
{
    auto f = [](const Ising1D<1000>::State& s) { return s[500 - 1]; };

    for (double j = -10; j < 10; j+=0.7)
    {
        Ising1D<1000> ising(1, 1 / j);
        double x500 = 0;
        MonteCarlo(x500, ising, f, g, n);
        std::cout << "Beta = " << 1 << "| h = " << 1 / j << '\n';
        std::cout << "x500 = " << x500 << '\n';
    }
}
