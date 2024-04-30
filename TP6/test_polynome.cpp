#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#include "polynome.hpp"

int main()
{
    basic_polynome<double> q;
    std::cout << "Degre de q : " << q.degree() << '\n';

    std::vector<int> v1 {6, 3, 0, 1, 5};
    basic_polynome<int> p1(v1);

    std::vector<int> v2 {1, 0, 1};
    basic_polynome<int> p2(v2);

    std::complex<double> a(2.0, 1.0);
    std::complex<double> b(0.0, 1.0);

    std::vector<std::complex<double>> vc {a, b};
    basic_polynome<std::complex<double>> pv(vc);

    std::complex<int> one = 1;
    std::cout << "Is one one : " << is_one(one) << '\n';

    std::cout << "Is zero a : " << is_zero(a) << '\n';
    std::cout << "p1 : " << p1 << '\n';
    std::cout << "p2 : " << p2 << '\n';

    basic_polynome<int> sum = p1 + p2;
    basic_polynome<int> diff = p1 - p2;
    basic_polynome<int> prod = p1 * p2;
    std::cout << "Somme : " << sum << '\n';
    std::cout << "Difference : " << diff << '\n';
    std::cout << "Produit : " << prod << '\n';
    std::cout << "Division: " << p1 / p2 << '\n';
    std::cout << "Reste: " << p1 % p2 << '\n';

    return 0;
}
