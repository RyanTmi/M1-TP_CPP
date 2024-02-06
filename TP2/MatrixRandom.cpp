#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "Histogram.hpp"

using MatrixDouble = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

Eigen::VectorXcd GenerateRandomSpectrum(std::mt19937_64& g, std::size_t n);
MatrixDouble GenerateGOEMatrix(std::mt19937_64& g, std::size_t n);

int main()
{
    std::mt19937_64 g(time(0));

    double lowerBound = -3;
    double upperBound = 3;
    std::size_t boxCount = 20;

    Histogram hist(lowerBound, upperBound, boxCount);

    for (std::size_t i = 0; i < 200; ++i)
    {
        for (auto eigenValue : GenerateRandomSpectrum(g, 150))
        {
            hist += eigenValue.real() / (2 * std::sqrt(150));
        }
    }

    std::cout << hist << "\n";
    {
        std::ofstream eigenDatas("eigenvalues.dat");
        eigenDatas << hist;
    }

    return 0;
}

Eigen::VectorXcd GenerateRandomSpectrum(std::mt19937_64& g, std::size_t n)
{
    MatrixDouble m = GenerateGOEMatrix(g, n);
    Eigen::EigenSolver<MatrixDouble> solver(m);
    return solver.eigenvalues();
}

MatrixDouble GenerateGOEMatrix(std::mt19937_64& g, std::size_t n)
{
    std::normal_distribution<double> n1(0, 1);
    std::normal_distribution<double> n2(0, 2);

    MatrixDouble goe(n, n);
    for (std::size_t i = 0; i < n; ++i) {
        goe(i, i) = n1(g);
        for (std::size_t j = i + 1; j < n; ++j) {
            goe(i, j) = n2(g);
            goe(j, i) = goe(i, j);
        }
    }

    return goe;
}