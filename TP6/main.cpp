#include <complex>
#include <iostream>
#include <vector>

#include "polynome.hpp"

int main()
{
    polynome<double> q;
    std::cout << "Degre de q : " << q.degree() << std::endl;

    std::vector<int> v1 {6, 3, 0, 1, 5};
    polynome<int> p1(v1);
    
    std::vector<int> v2 {1, 0, 1};
    polynome<int> p2(v2);

    std::complex<double> a(2.0, 1.0);
    std::complex<double> b(0.0, 1.0);

    std::vector<std::complex<double>> vc {a, b};
    polynome<std::complex<double>> pv(vc);

    std::complex<int> one = 1;
    std::cout << "Is one one : " << is_one(one) << std::endl;

    std::cout << "Is zero a : " << is_zero(a) << std::endl;
    std::cout << "p1 : " << p1 << std::endl;
    std::cout << "p2 : " << p2 << std::endl;

    polynome<int> sum = p1 + p2;
    polynome<int> diff = p1 - p2;
    // polynome<int> prod = p1 * p2;
    std::cout << "Somme : " << sum << std::endl;
    std::cout << "Différence : " << diff << std::endl;
    // std::cout << "Produit : " << prod << std::endl;

    return 0;
}
