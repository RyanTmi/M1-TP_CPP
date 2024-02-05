#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <functional>

using MatrixDouble = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
using SparseMatrixDouble = Eigen::SparseMatrix<double>;

MatrixDouble SlowPower(const MatrixDouble& m, std::size_t n);
MatrixDouble FastPower(const MatrixDouble& m, std::size_t n);
SparseMatrixDouble SparseFastPower(const SparseMatrixDouble& m, std::size_t n);

template <typename M>
void ProfilePow(const char* funcName, bool showResult, std::function<M()> func);

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
    {
        std::ifstream matrixDatas("matrice.dat");
        for (int i = 0; i < 30; ++i) {
            for (int j = 0; j < 30; ++j) {
                matrixDatas >> B(i, j);
                if (B(i, j) != 0) {
                    sB.coeffRef(i, j) = B(i, j);
                }
            }
        }
    }

    std::cout << "----------------------------\n";
    std::cout << "A :\n" << A << std::endl;
    std::cout << "----------------------------\n";
    std::cout << "B :\n" << B << std::endl;
    std::cout << "----------------------------\n";

    ProfilePow<MatrixDouble>("SlowPower", false, [&A]() {
        return SlowPower(A, pow);
    });
    ProfilePow<MatrixDouble>("SlowPower", false, [&B]() {
        return SlowPower(B, pow);
    });
    ProfilePow<MatrixDouble>("FastPower", false, [&A]() {
        return FastPower(A, pow);
    });
    ProfilePow<MatrixDouble>("FastPower", false, [&B]() {
        return FastPower(B, pow);
    });
    ProfilePow<SparseMatrixDouble>("SparseFastPower", false, [&sB]() {
        return SparseFastPower(sB, pow);
    });

    return 0;
}


MatrixDouble SlowPower(const MatrixDouble& m, std::size_t n)
{
    switch (n) {
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
    switch (n) {
        case 0:
            return MatrixDouble::Identity(m.rows(), m.cols());
        case 1:
            return m;
        default:
            break;
    }
    MatrixDouble halfPow = FastPower(m, n / 2);
    switch (n % 2) {
        case 0:
            return halfPow * halfPow;
        default:
            return m * halfPow * halfPow;
    }
}

SparseMatrixDouble SparseFastPower(const SparseMatrixDouble& m, std::size_t n)
{
    switch (n) {
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
    switch (n % 2) {
        case 0:
            return halfPow * halfPow;
        default:
            return m * halfPow * halfPow;
    }
}

template <typename M>
void ProfilePow(const char* funcName, bool showResult, std::function<M()> func)
{
    auto start = std::chrono::system_clock::now();
    M m = func();
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "============================\n";
    if (showResult) {
        std::cout << m << "\n";
    }
    std::cout << "----------------------------\n"
              << funcName << ": " << diff.count() * 1000 << "ms\n"
              << "============================\n\n";
}
