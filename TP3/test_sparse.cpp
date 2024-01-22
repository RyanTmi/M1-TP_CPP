#include "my_permutation.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
	Permutation u;
	Sparse_permutation u_sp;
	{
		auto fichier_u = ifstream("file_u.dat");
		auto fichier_sparse_u = ifstream("file_sparse_u.dat");
		fichier_u >> u;
		fichier_sparse_u >> u_sp;
	}
	
	{
		cout << "----- Avec Permutation: \n";
		auto t1 = std::chrono::system_clock::now();
		auto lu = u.cycles();
		auto t2 = std::chrono::system_clock::now();
		std::chrono::duration<double> diff21 = t2-t1;
		std::cout << "Temps mis pour extraire les cycles: " << diff21.count() << "s." << "\n";
		cout << "Les cycles de u sont:\n" ;
		for(const auto & c: lu) cout << c << " longueur: " << c.order() << "\n";
		cout << "Ordre de u: " << u.order() << "\n";
		
		auto t3 = std::chrono::system_clock::now();
		auto u4 = u*u*u*u;
		auto t4= std::chrono::system_clock::now();
		std::chrono::duration<double> diff43 = t4-t3;
		std::cout << "Temps mis pour calculer la puissance 5: " << diff43.count() << "s." << "\n";
		cout << "Ordre de u^5: " << u4.order() << "\n";
		auto lu4 = u4.cycles();
		for(const auto & c: lu4) cout << c << " longueur: " << c.order()<< "\n";
	}
	cout << "\n\n\n\n";
	{
		cout << "----- Avec Sparse_Permutation: \n";
		auto t1 = std::chrono::system_clock::now();
		auto lu_sp = u_sp.cycles();
		auto t2 = std::chrono::system_clock::now();
		std::chrono::duration<double> diff21 = t2-t1;
		std::cout << "Temps mis pour extraire les cycles: " << diff21.count() << "s." << "\n";
		cout << "Les cycles de u sont:\n" ;
		for(const auto & c: lu_sp) cout << c << " longueur: " << c.order()<< "\n";
		cout << "Ordre de u: " << u_sp.order() << "\n";
		
		auto t3 = std::chrono::system_clock::now();
		auto u4_sp = u_sp*u_sp*u_sp*u_sp;
		auto t4= std::chrono::system_clock::now();
		std::chrono::duration<double> diff43 = t4-t3;
		std::cout << "Temps mis pour calculer la puissance 5: " << diff43.count() << "s." << "\n";
		cout << "Ordre de u^5: " << u4_sp.order() << "\n";
		auto lu4 = u4_sp.cycles();
		for(const auto & c: lu4) cout << c << " longueur: " << c.order() <<"\n";
	}
	
	
	
	
	
	return 0;
}
