#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

struct Card
{
    std::string Name;
    std::string City;
    int Age;
    double Time;
};

struct AnalysisResult
{
    std::size_t LyonnaisCount;                             // a)
    double LyonnaisPercentage;
    std::size_t LyonnaisCountUnder30;                      // b)
    bool ToulousanBeginWithA;                              // c)
    int MinAge;                                            // d)
    int MaxAge;
    std::string MinAgeName;
    std::string MaxAgeName;
    double AgeMean;                                        // e)
    double AgeSd;
    bool PBetterThanM;                                     // c)
    const char* ToulousansDataFileName = "toulousans.txt"; // g)
    double EmpiricalCov;                                   // h)
    std::set<std::string> Cities;                          // i)
};

bool LoadDatasFromFile(std::vector<Card>& datas, const char* fileName);
void PrintAnalysis(const AnalysisResult& analisysResult);

void Analysis(const std::vector<Card>& datas, AnalysisResult& analisysResult);
void AlgorithmAnalysis(const std::vector<Card>& datas, AnalysisResult& analisysResult);

void TimeSortDatas(std::vector<Card>& datas);
void SmallestStep(const std::vector<Card>& datas);
void AgeFilter(const std::vector<Card>& datas);

void SortDatas(std::vector<Card>& datas);
void AgesHistogram(const std::vector<Card>& datas);

int main()
{
    std::vector<Card> datas;

    // ---------- 1.3.1 ----------
    if (!LoadDatasFromFile(datas, "smalldata.txt"))
    {
        return 1;
    }

    {
        AnalysisResult analisysResult;
        Analysis(datas, analisysResult);
        PrintAnalysis(analisysResult);
    }

    // ---------- 1.3.2 ----------
    {
        AnalysisResult analisysResult;
        AlgorithmAnalysis(datas, analisysResult);
        PrintAnalysis(analisysResult);
    }
    TimeSortDatas(datas);

    // ---------- 1.3.3 ----------
    SmallestStep(datas);
    AgeFilter(datas);
    SortDatas(datas);
    AgesHistogram(datas);

    return 0;
}

std::ostream& operator<<(std::ostream& os, const Card& c)
{
    return os << c.Name << " " << c.City << " " << c.Age << " " << c.Time;
}

bool LoadDatasFromFile(std::vector<Card>& datas, const char* fileName)
{
    std::ifstream dataFile(fileName);
    if (!dataFile.is_open())
    {
        std::cerr << "Can not open " << fileName << " !\n";
        return false;
    }

    Card c;
    while (dataFile >> c.Name >> c.City >> c.Age >> c.Time)
    {
        datas.push_back(c);
    }

    if (!dataFile.eof())
    {
        std::cerr << "Can not extract data, wrong format !\n";
        return false;
    }
    return true;
}

void PrintAnalysis(const AnalysisResult& ar)
{
    std::cout << "=========== Analisys results ===========\n\n"
              << "a)  Personnes habitant à Lyon : " << ar.LyonnaisCount << "\n"
              << "    Pourcentage de lyonnais   : " << ar.LyonnaisPercentage << "\n"
              << "b)  Personnes habitant à Lyon <30: " << ar.LyonnaisCountUnder30 << "\n"
              << "c)  Toulousain dont le prénom commence par 'A' ? : " << std::boolalpha << ar.ToulousanBeginWithA << "\n"
              << "d)  Le plus agé : " << ar.MaxAge << " " << ar.MaxAgeName << "\n"
              << "    Le plus jeune : " << ar.MinAge << " " << ar.MinAgeName << "\n"
              << "e)  Age moyen : " << ar.AgeMean << "\n"
              << "    Écart-type : " << ar.AgeSd << "\n"
              << "f)  Parisiens plus rapides en moyenne que les Marseillais ? : " << std::boolalpha << ar.PBetterThanM << "\n"
              << "g)  Le fichier " << ar.ToulousansDataFileName << " a été crée\n"
              << "h)  Covariance empirique entre âge et temps : " << ar.EmpiricalCov << "\n"
              << "i)  Liste des villes représentées :\n";
    for (auto& city : ar.Cities) {
        std::cout << "      " << city << "\n";
    }
    std::cout << "\n======================================\n\n";
}

void Analysis(const std::vector<Card>& datas, AnalysisResult& ar)
{
    const double cardsCount = datas.size();

    ar.MinAge = datas.front().Age;
    ar.MaxAge = datas.front().Age;

    double variance = 0.0f;

    int parisiansCount = 0;
    double parisiansTimeMean = 0.0f;
    int marseillaisCount = 0;
    double marseillaisTimeMean = 0.0f;

    double covariance = 0.0f;
    int toulousainsCount = 0;
    double toulousainsTimeMean = 0.0f;
    double toulousainsAgeMean = 0.0f;

    std::ofstream toulousansData(ar.ToulousansDataFileName);

    for (const Card& c : datas)
    {
        if (c.City == "Lyon")
        {
            // Question a)
            ar.LyonnaisCount++;
            // Question b)
            ar.LyonnaisCountUnder30 += (c.Age < 30);
        }

        if (c.City == "Toulouse")
        {
            // Question c)
            if (c.Name[0] == 'A')
            {
                ar.ToulousanBeginWithA = true;
            }
            // Question g)
            toulousansData << c.Name << "\t" << 2018 - c.Age << "\t" << c.Time << "\n";

            // Question h)
            toulousainsTimeMean += c.Time;
            toulousainsAgeMean += c.Age;
            toulousainsCount++;
            covariance += c.Age * c.Time;
        }

        // Question d)
        if (ar.MinAge > c.Age) {
            ar.MinAge = c.Age;
            ar.MinAgeName = c.Name;
        }
        if (ar.MaxAge < c.Age) {
            ar.MaxAge = c.Age;
            ar.MaxAgeName = c.Name;
        }

        // Question e)
        ar.AgeMean += c.Age;
        variance += c.Age * c.Age;

        // Question f)
        if (c.City == "Paris") {
            parisiansTimeMean += c.Time;
            parisiansCount++;
        }
        if (c.City == "Marseille") {
            marseillaisTimeMean += c.Time;
            marseillaisCount++;
        }

        // Question i)
        ar.Cities.insert(c.City);
    }
    toulousansData.close();

    // Question a)
    ar.LyonnaisPercentage = 100 * ar.LyonnaisCount / cardsCount;

    // Question e)
    ar.AgeMean /= cardsCount;
    variance /= cardsCount;
    variance -= ar.AgeMean * ar.AgeMean;
    ar.AgeSd = std::sqrt(variance);

    // Question f)
    parisiansTimeMean /= parisiansCount;
    marseillaisTimeMean /= marseillaisCount;
    ar.PBetterThanM = parisiansTimeMean < marseillaisTimeMean;

    // Question h)
    toulousainsTimeMean /= toulousainsCount;
    toulousainsAgeMean /= toulousainsCount;
    covariance /= toulousainsCount;
    covariance -= toulousainsAgeMean * toulousainsTimeMean;
    ar.EmpiricalCov = covariance;
}

void AlgorithmAnalysis(const std::vector<Card>& datas, AnalysisResult& ar)
{
    const double cardCount = datas.size();

    // Question a)
    ar.LyonnaisCount = std::count_if(datas.begin(), datas.end(), [](const Card& c) {
        return c.City == "Lyon";
    });
    ar.LyonnaisPercentage = 100 * ar.LyonnaisCount / cardCount;

    // Question b)
    ar.LyonnaisCountUnder30 = std::count_if(datas.begin(), datas.end(), [](const Card& c) {
        return c.City == "Lyon" && c.Age < 30;
    });

    // Question c)
    ar.ToulousanBeginWithA = std::any_of(datas.begin(), datas.end(), [](const Card& c) {
        return c.City == "Toulouse" && c.Name[0] == 'A';
    });

    // Question d)
    auto minMax = std::minmax_element(datas.begin(), datas.end(), [](const Card& c1, const Card& c2) {
        return c1.Age < c2.Age;
    });
    ar.MinAge = minMax.first->Age;
    ar.MinAgeName = minMax.first->Name;
    ar.MaxAge = minMax.second->Age;
    ar.MaxAgeName = minMax.second->Name;

    // Question e)
    ar.AgeMean = std::accumulate(datas.begin(), datas.end(), 0, [](int x, const Card& c) {
        return x + c.Age;
    }) / cardCount;

    double variance = std::accumulate(datas.begin(), datas.end(), 0, [](int x, const Card& c) {
        return x + c.Age * c.Age;
    }) / cardCount;
    variance -= ar.AgeMean * ar.AgeMean;
    ar.AgeSd = std::sqrt(variance);

    // Question f)
    double parisiansTimeMean = std::accumulate(datas.begin(), datas.end(), 0, [](int x, const Card& c) {
        return x + (c.City == "Paris" ? c.Time : 0);
    }) / static_cast<double>(std::count_if(datas.begin(), datas.end(), [](Card c) {
        return c.City == "Paris";
    }));
    double marseillaisTimeMean = std::accumulate(datas.begin(), datas.end(), 0, [](int x, const Card& c) {
        return x + (c.City == "Marseille" ? c.Time : 0);
    }) / static_cast<double>(std::count_if(datas.begin(), datas.end(), [](Card c) {
        return c.City == "Marseille";
    }));
    ar.PBetterThanM = parisiansTimeMean < marseillaisTimeMean;

    // Question g)
    std::ofstream toulousansData(ar.ToulousansDataFileName);
    std::for_each(datas.begin(), datas.end(), [&toulousansData](const Card& c) {
        if (c.City == "Toulouse") {
            toulousansData << c.Name << "\t" << 2018 - c.Age << "\t" << c.Time << std::endl;
        }
    });
    toulousansData.close();

    // Question h)
    double toulousainsCount = std::count_if(datas.begin(), datas.end(), [](const Card& c){
        return c.City == "Toulouse";
    });
    double ageMean = std::accumulate(datas.begin(), datas.end(), 0.0, [](double x, const Card& c) {
        return x + (c.City == "Toulouse" ? c.Age : 0);
    }) / toulousainsCount;
    double timeMean = std::accumulate(datas.begin(), datas.end(), 0.0, [](double x, const Card& c) {
        return x + (c.City == "Toulouse" ? c.Time : 0);
    }) / toulousainsCount;

    ar.EmpiricalCov = std::accumulate(datas.begin(), datas.end(), 0.0, [](double x, Card c) {
        return x + (c.City == "Toulouse" ? c.Age * c.Time : 0);
    }) / toulousainsCount;
    ar.EmpiricalCov -= ageMean * timeMean;

    // Question i)
    std::for_each(datas.begin(), datas.end(), [&ar](const Card& c) {
        ar.Cities.insert(c.City);
    });
}

void TimeSortDatas(std::vector<Card>& datas)
{
    std::sort(datas.begin(), datas.end(), [](const Card& c1, const Card& c2) {
        return c1.Time < c2.Time;
    });

    constexpr int dataLimit = 100;
    std::ofstream sortDatasFile("data_tri.txt");
    for (int i = dataLimit - 1; i >= 0; --i)
    {
        sortDatasFile << datas[i] << "\n";
    }
    std::cout << "Le fichier data_tri.txt a été crée\n";
}

void SmallestStep(const std::vector<Card>& datas)
{
    std::vector<double> times(datas.size());
    std::transform(datas.begin(), datas.end(), std::back_inserter(times), [](const Card& c) { return c.Time; });

    std::sort(times.begin(), times.end());
    std::adjacent_difference(times.begin(), times.end(), times.begin());

    double minStep = *std::min_element(times.begin() + 1, times.end());
    std::cout << "Le plus petit écart entre les temps de courses est : " << minStep << " secondes\n";
}

void AgeFilter(const std::vector<Card>& datas)
{
    std::vector<Card> youngest;
    std::vector<Card> oldest;

    std::partition_copy(
        datas.begin(), datas.end(),
        std::back_inserter(youngest), std::back_inserter(oldest),
        [](const Card& c) { return c.Age < 40; }
    );

    /*
    for (const Card& c : youngest)
    {
        std::cout << c << std::endl;
    }
    for (const Card& c : oldest)
    {
        std::cout << c << std::endl;
    }
    */
}

void SortDatas(std::vector<Card>& datas)
{
    std::sort(datas.begin(), datas.end(), [](const Card& c1, const Card& c2) {
        if (c1.Name != c2.Name) {
            return c1.Name < c2.Name;
        }
        if (c1.City != c2.City) {
            return c1.City < c2.City;
        }
        if (c1.Age != c2.Age) {
            return c1.Age > c2.Age;
        }
        return c1.Time < c2.Time;
    });

    std::ofstream sortDatasFile("ordre.dat");
    for (const auto& c : datas) {
            sortDatasFile << c << "\n";
    }
    std::cout << "Le fichier ordre.dat a été crée\n";
}

void AgesHistogram(const std::vector<Card>& datas)
{
    std::map<int, int> agesHistogram;
    for (const Card& c : datas) {
        agesHistogram[c.Age]++;
    }

    std::cout << "Histogramme des âges:\n\n";
    std::for_each(agesHistogram.begin(), agesHistogram.end(), [](std::pair<int, int> x) {
        std::cout << x.first << " : " << x.second << "\n";
    });
    auto max = std::max_element(agesHistogram.begin(), agesHistogram.end(), [](std::pair<int, int> x1, std::pair<int, int> x2) {
        return x1.second < x2.second;
    })->first;
    std::cout << "Classe d'âge la plus nombreuse : " << max << "\n";
}
