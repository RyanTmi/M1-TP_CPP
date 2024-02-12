#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>

#include "Permutation.hpp"

int main()
{
    // ******* Premiere partie: bases de la bibliothèque

    // Déclaration de deux permutations de deux manières différentes:
    Permutation b(6); // Identité

    std::vector<std::size_t> v {2, 4, 5, 3, 1, 0};
    Permutation a(v);

    // Calcul des itérées d'une permutation
    for (std::size_t i = 0; i <= 6; ++i)
    {
        std::cout << "a^" << i << "\n" << b << "\n";
        b = b * a;
    }

    // Calcul des points fixes d'une permutation
    std::list<std::size_t> fp = a.FixedPoints();
    std::copy(fp.begin(), fp.end(), std::ostream_iterator<std::size_t>(std::cout, " "));
    std::cout << "\n";

    // ******* Deuxieme partie: un peu plus d'algorithmique

    // Lecture de deux permutations dans deux fichiers
    Permutation s;
    std::ifstream fichier_s("file_s.dat");
    fichier_s >> s;
    fichier_s.close();

    Permutation t;
    std::ifstream fichier_t("file_t.dat");
    fichier_t >> t;
    fichier_t.close();
    
    // Calcul de cycles et ordres
    std::list<Cycle> aCycles = a.Cycles();
    std::cout << "Les cycles de a sont: ";
    std::copy(aCycles.begin(), aCycles.end(), std::ostream_iterator<Cycle>(std::cout));
    std::cout << "\nL'ordre de la permutation a est égal à " << a.Order() << "\n";

    // Calcul de u = s * t^{-1} et son ordre
    Permutation u = s * t.Inverse();
    std::size_t o = u.Order();
    std::cout << "L'ordre de la permutation s*t^-1 est égal à " << o << "\n";

    // Extraction des cycles de u
    std::list<Cycle> uCycles = u.Cycles();
    std::cout << "Cette permutation a " << uCycles.size()
              << " cycles, dont le plus grand a pour longueur "
              << std::max_element(uCycles.begin(), uCycles.end())->Order() << "\n";
    // Attention, cela utilise < sur des Cycle !

    // ******* Troisieme partie: génération aléatoire et Monte-Carlo

    std::mt19937 g(time(0));
    unsigned n = 100;
    unsigned nb_echant = 10000;
    unsigned nb_derang = 0;
    for (unsigned i = 0; i < nb_echant; ++i)
    {
        nb_derang += Permutation(n,g).IsDerangement();
    }
    std::cout << "La proportion de dérangements est environ "
              << nb_derang/double(nb_echant) << "\n";

    return 0;
}
