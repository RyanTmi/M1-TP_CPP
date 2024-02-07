#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

#include <Eigen/Dense>
#include <Eigen/Sparse>

using MatrixDouble       = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
using SparseMatrixDouble = Eigen::SparseMatrix<double>;

MatrixDouble SlowPower(const MatrixDouble& m, std::size_t n);
MatrixDouble FastPower(const MatrixDouble& m, std::size_t n);
SparseMatrixDouble SparseFastPower(const SparseMatrixDouble& m, std::size_t n);

template <typename MatrixType>
void ProfilePow(const char* funcName, bool showResult, std::function<MatrixType()> func);

int main()
{
    constexpr std::size_t pow = 10000;

    MatrixDouble A {
        {0.4f , 0.6f , 0.0f},
        {0.75f, 0.25f, 0.0f},
        {0.0f , 0.0f , 1.0f}
    };
    MatrixDouble B(30, 30);
    SparseMatrixDouble sB(B.rows(), B.cols());

    std::ifstream matrixDatas("matrice.dat");
    for (int i = 0; i < 30; ++i)
    {
        for (int j = 0; j < 30; ++j)
        {
            matrixDatas >> B(i, j);
            if (B(i, j) != 0)
            {
                sB.coeffRef(i, j) = B(i, j);
            }
        }
    }
    matrixDatas.close();

    std::cout << "----------------------------\n"
              << "A :\n" << A << "\n"
              << "----------------------------\n"
              << "B :\n" << B << "\n"
              << "----------------------------\n\n";

    std::cout << "Power : " << pow << "\n\n";
    ProfilePow<MatrixDouble>("SlowPower A", false, [&A]() {
        return SlowPower(A, pow);
    });
    ProfilePow<MatrixDouble>("SlowPower B", false, [&B]() {
        return SlowPower(B, pow);
    });
    ProfilePow<MatrixDouble>("FastPower A", false, [&A]() {
        return FastPower(A, pow);
    });
    ProfilePow<MatrixDouble>("FastPower B", false, [&B]() {
        return FastPower(B, pow);
    });
    ProfilePow<SparseMatrixDouble>("SparseFastPower B", false, [&sB]() {
        return SparseFastPower(sB, pow);
    });

    return 0;
}


MatrixDouble SlowPower(const MatrixDouble& m, std::size_t n)
{
    switch (n)
    {
        case 0:
            return MatrixDouble::Identity(m.rows(), m.cols());
        case 1:
            return m;
        default: 
            return m * SlowPower(m, n - 1);
    }
}

MatrixDouble FastPower(const MatrixDouble& m, std::size_t n)
{
    switch (n)
    {
        case 0:
            return MatrixDouble::Identity(m.rows(), m.cols());
        case 1:
            return m;
        default:
            break;
    }
    MatrixDouble halfPow = FastPower(m, n / 2);
    switch (n % 2)
    {
        case 0:
            return halfPow * halfPow;
        default:
            return m * halfPow * halfPow;
    }
}

SparseMatrixDouble SparseFastPower(const SparseMatrixDouble& m, std::size_t n)
{
    switch (n)
    {
        case 0: {
            SparseMatrixDouble id;
            id.setIdentity();
            return id;
        }
        case 1:
            return m;
        default:
            break;
    }
    SparseMatrixDouble halfPow = SparseFastPower(m, n / 2);
    switch (n % 2)
    {
        case 0:
            return halfPow * halfPow;
        default:
            return m * halfPow * halfPow;
    }
}

template <typename MatrixType>
void ProfilePow(const char* funcName, bool showResult, std::function<MatrixType()> func)
{
    auto start = std::chrono::system_clock::now();
    MatrixType m = func();
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "============================\n";
    if (showResult)
    {
        std::cout << m << "\n----------------------------\n";
    }
    std::cout << funcName << ": " << diff.count() * 1000 << "ms\n"
              << "============================\n\n";
}
