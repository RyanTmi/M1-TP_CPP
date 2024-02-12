#include <chrono>
#include <fstream>
#include <iostream>

#include "Permutation.hpp"

int main()
{
    Permutation u;
    SparsePermutation u_sp;
    {
        auto fichier_u = std::ifstream("file_u.dat");
        auto fichier_sparse_u = std::ifstream("file_sparse_u.dat");
        fichier_u >> u;
        fichier_sparse_u >> u_sp;
    }

    std::cout << u << "\n";
    std::cout << u_sp << "\n";

    {
        std::cout << "----- Avec Permutation: \n";
        auto t1 = std::chrono::system_clock::now();
        auto lu = u.Cycles();
        auto t2 = std::chrono::system_clock::now();
        std::chrono::duration<double> diff21 = t2 - t1;
        std::cout << "Temps mis pour extraire les cycles: " << diff21.count() << "s." << "\n";
        std::cout << "Les cycles de u sont:\n" ;
        for(const auto & c: lu) std::cout << c << " longueur: " << c.Order() << "\n";
        std::cout << "Ordre de u: " << u.Order() << "\n";
        
        auto t3 = std::chrono::system_clock::now();
        auto u4 = u*u*u*u;
        auto t4= std::chrono::system_clock::now();
        std::chrono::duration<double> diff43 = t4-t3;
        std::cout << "Temps mis pour calculer la puissance 5: " << diff43.count() << "s." << "\n";
        std::cout << "Ordre de u^5: " << u4.Order() << "\n";
        auto lu4 = u4.Cycles();
        for(const auto & c: lu4) std::cout << c << " longueur: " << c.Order()<< "\n";
    }
    std::cout << "\n\n\n\n";
    {
        std::cout << "----- Avec Sparse_Permutation: \n";
        auto t1 = std::chrono::system_clock::now();
        auto lu_sp = u_sp.Cycles();
        auto t2 = std::chrono::system_clock::now();
        std::chrono::duration<double> diff21 = t2-t1;
        std::cout << "Temps mis pour extraire les cycles: " << diff21.count() << "s." << "\n";
        std::cout << "Les cycles de u sont:\n" ;
        for(const auto & c: lu_sp) std::cout << c << " longueur: " << c.Order()<< "\n";
        std::cout << "Ordre de u: " << u_sp.Order() << "\n";
        
        auto t3 = std::chrono::system_clock::now();
        auto u4_sp = u_sp * u_sp * u_sp * u_sp;
        auto t4 = std::chrono::system_clock::now();
        std::chrono::duration<double> diff43 = t4-t3;
        std::cout << "Temps mis pour calculer la puissance 5: " << diff43.count() << "s." << "\n";
        std::cout << "Ordre de u^5: " << u4_sp.Order() << "\n";
        auto lu4 = u4_sp.Cycles();
        for(const auto & c: lu4) std::cout << c << " longueur: " << c.Order() <<"\n";
    }
    return 0;
}
