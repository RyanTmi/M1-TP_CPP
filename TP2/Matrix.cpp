#include <Eigen/Dense>

#include <iostream>
#include <chrono>
#include <functional>

using MatrixDouble = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

MatrixDouble SlowPower(const MatrixDouble& m, std::size_t n);
MatrixDouble FastPower(const MatrixDouble& m, std::size_t n);

void ProfilePow(const char* funcName, std::function<MatrixDouble()> func);

int main()
{
    MatrixDouble A {
        {0.4f , 0.6f , 0.0f},
        {0.75f, 0.25f, 0.0f},
        {0.0f , 0.0f , 1.0f}
    };
    constexpr std::size_t pow = 1000;

    std::cout << "----------------------------\n";
    std::cout << "A :\n" << A << std::endl;
    std::cout << "----------------------------\n";

    ProfilePow("SlowPower", [&A, pow]() {
        return SlowPower(A, pow);
    });
    ProfilePow("FastPower", [&A, pow]() {
        return FastPower(A, pow);
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

void ProfilePow(const char* funcName, std::function<MatrixDouble()> func)
{
    auto start = std::chrono::system_clock::now();
    MatrixDouble m = func();
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "============================\n"
              << m << "\n"
              << "----------------------------\n"
              << funcName << ": " << diff.count() * 1000 << "ms\n"
              << "============================\n\n";
}