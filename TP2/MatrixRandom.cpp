#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "Histogram.hpp"

using MatrixDouble = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

Eigen::VectorXcd GenerateRandomSpectrum(std::mt19937_64& g, std::size_t n);
void GenerateGOEMatrix(std::mt19937_64& g, MatrixDouble& m);

int main()
{
    std::mt19937_64 g(time(0));

    std::size_t sampleCount = 200;
    std::size_t matricesSize = 150;

    double lowerBound = -3;
    double upperBound = 3;
    std::size_t boxCount = 20;
    Histogram hist(lowerBound, upperBound, boxCount);

    for (std::size_t i = 0; i < sampleCount; ++i)
    {
        for (auto eigenValue : GenerateRandomSpectrum(g, matricesSize))
        {
            hist += eigenValue.real() / (2 * std::sqrt(matricesSize));
        }
    }

    std::cout << hist << "\n";
    std::ofstream eigenDatas("eigenvalues.dat");
    eigenDatas << hist;

    return 0;
}

Eigen::VectorXcd GenerateRandomSpectrum(std::mt19937_64& g, std::size_t n)
{
    MatrixDouble goe(n, n);
    GenerateGOEMatrix(g, goe);
    Eigen::EigenSolver<MatrixDouble> solver(goe);
    return solver.eigenvalues();
}

void GenerateGOEMatrix(std::mt19937_64& g, MatrixDouble& m)
{
    std::normal_distribution<double> n1(0, 1);
    std::normal_distribution<double> n2(0, 2);

    for (int i = 0; i < m.rows(); ++i) {
        m(i, i) = n1(g);
        for (int j = i + 1; j < m.cols(); ++j) {
            m(i, j) = n2(g);
            m(j, i) = m(i, j);
        }
    }
}