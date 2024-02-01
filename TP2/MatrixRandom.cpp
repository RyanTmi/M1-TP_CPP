#include <iostream>
#include <random>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "Histogram.hpp"

using MatrixDouble = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;


Eigen::VectorXcd GenerateRandomSpectrum(std::mt19937& g, std::size_t n);
MatrixDouble GenerateGOEMatrix(std::mt19937& g, std::size_t n);

int main()
{
    std::random_device device {};
    std::mt19937_64 g(device);


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
    std::normal_distribution n1 {0, 1};
    std::normal_distribution n2 {0, 2};

    MatrixDouble goe(n, n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (i == j) {
                goe(i, j) = n1(g);
            } else {
                goe(i, j) = n2(g);
            }
        }
    }

    return goe;
}