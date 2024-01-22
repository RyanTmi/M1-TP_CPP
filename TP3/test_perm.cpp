// ------ CONTENU DU FICHIER test_perm.cpp -------
#include "my_permutation.hpp"
#include <iostream>
#include <fstream> 
#include <algorithm> 
#include <iterator>
#include <random> // pour le générateur aléatoire std::mt19937
int main () {
// *******Premiere partie: bases de la bibliothèque
// Déclaration de deux permutations de deux manières différentes:
    Permutation b(6); //identite
    std::vector<int> v{2,4,5,3,1,0};//syntaxe C++11 avec { }
    Permutation a(v);
// Calcul des itérées d'une permutation
    for(int i=0; i<=6; ++i) {
      std::cout << "a^" << i << "\n" << b << "\n";
      b = b*a;
    }
// Calcul des points fixes d'une permutation
    std::list<int> fp = a.fixed_points();
    std::copy( fp.begin(), fp.end(), 
               std::ostream_iterator<int>(std::cout," ") );
    std::cout << "\n";

// ******* Deuxieme partie: un peu plus d'algorithmique
// lecture de deux permutations dans deux fichiers
    std::ifstream fichier_s("./file_s.dat");
    std::ifstream fichier_t("./file_t.dat");
    Permutation s,t;
    fichier_s >> s;
    fichier_t >> t;
    fichier_s.close();
    fichier_t.close();
    
 // Calcul de u=s * t^{-1} et son ordre
    Permutation u=s*t.inverse();
    std::cout << "L'ordre de la permutation s*t^-1 est égal à "
        << u.order() << "\n";
        
 // Extraction des cycles de u
    std::list<Cycle> l = u.cycles();
    std::cout << "Cette permutation a " << l.size() <<
        "cycles, dont le plus grand a pour longueur " <<
        std::max_element(l.begin(), l.end())->order() << "\n";
        //attention, cela utilise < sur des Cycle !
        
// ******* Troisieme partie: génération aléatoire et Monte-Carlo
    std::mt19937 g(time(nullptr));
    unsigned n=100;
    unsigned nb_echant = 10000;
    unsigned nb_derang = 0;
    for(unsigned i = 0; i < nb_echant; ++i) {
        nb_derang += Permutation(n,g).is_derangement();
    }
    std::cout << "La proportion de dérangements est environ "
        << nb_derang/double(nb_echant) << "\n";

    return 0;
}
