#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <string>

void WriteStats(const std::map<std::string, std::uint32_t>& frequencies);
void Numerics(const std::map<std::string, std::uint32_t>& frequencies);

int main()
{
    std::ifstream declarationFile("declaration.txt");
    std::map<std::string, std::uint32_t> frequencies;

    std::string word;
    while (declarationFile >> word)
    {
        ++frequencies[word];
    }
    declarationFile.close();

    WriteStats(frequencies);
    Numerics(frequencies);

    return 0;
}

void WriteStats(const std::map<std::string, std::uint32_t>& frequencies)
{
    std::ofstream statsFile("stats.dat");
    for (const auto& p : frequencies)
    {
        statsFile << p.first << " : " << p.second << '\n';
    }
    statsFile.close();

    std::ofstream statsRevFile("rev_stats.dat");
    for (auto it = frequencies.rbegin(); it != frequencies.rend(); ++it)
    {
        statsRevFile << it->first << " : " << it->second << '\n';
    }
    statsRevFile.close();
}

void Numerics(const std::map<std::string, std::uint32_t>& f)
{
    using pair = std::pair<std::string, std::uint32_t>;

    std::cout << "Nombre de mots différents :     " << f.size() << '\n';
    std::cout << "Nombre de mots différents > 7 : "
        << std::count_if(f.begin(), f.end(), [](const pair& p) { return p.first.size() >= 7; }) << '\n';

    auto maxIt = std::max_element(f.begin(), f.end(), [](const pair& p1, const pair& p2) { return p1.second < p2.second; });
    std::cout << "Le plus frequent :              \"" << maxIt->first << "\" : " << maxIt->second << '\n';

    std::uint64_t wordCount = std::accumulate(f.begin(), f.end(), 0UL, [](std::uint64_t i, const pair& p) { return i + p.first.size() * p.second; });
    std::cout << "Nombre de lettres :             " << wordCount << '\n';


    auto isVowel = [](char c) { 
        c = std::tolower(c);
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y';
    };
    auto vowelCount = [&isVowel](const std::string& s) { return std::count_if(s.begin(), s.end(), isVowel); };
    std::cout << "Mots avec 2 voyelles :          "
         << std::count_if(f.begin(), f.end(), [&vowelCount](const pair& p) { return vowelCount(p.first) == 2; }) << '\n';

    auto eCount = [](const std::string& s) { return std::count_if(s.begin(), s.end(), [](char c) { return c == 'e'; }); };
    maxIt = std::max_element(f.begin(), f.end(), [&eCount](const pair& p1, const pair& p2) { return eCount(p1.first) < eCount(p2.first); });
    std::cout << "Contient le plus de 'e' :       \"" << maxIt->first << "\" : " << maxIt->second << '\n';
}