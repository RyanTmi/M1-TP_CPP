#include "Analysis.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>

std::ostream& operator<<(std::ostream& os, const Fiche& f)
{
    os << f.Name << " " << f.City << " " << f.Age << " " << f.Time;
    return os;
}

bool FetchDatas(std::vector<Fiche>& datas)
{
    std::ifstream dataFile("smalldata.txt");
    if (!dataFile.is_open()) {
        std::cerr << "Can not open smalldata.txt !" << std::endl;
        return false;
    }

    Fiche f {};
    while (dataFile >> f.Name >> f.City >> f.Age >> f.Time) {
        datas.push_back(f);
    }

    if (!dataFile.eof()) {
        std::cerr << "Can not extract data, wrong format !" << std::endl;
        return false;
    }
    return true;
}

void Analysis(const std::vector<Fiche>& datas, AnalysisResult& ar)
{
    ar = {};
    ar.MinAge = datas.front().Age;
    ar.MaxAge = datas.front().Age;

    double parisiansTimeMean = 0.0f;
    int parisiansCount = 0;
    double marseillaisTimeMean = 0.0f;
    int marseillaisCount = 0;
    double timeMean = 0.0f;
    std::ofstream toulousansData(ar.ToulousansDataFileName);

    for (const auto& f : datas) {
        if (f.City == "Lyon") {
            ar.LyonnaisCount++;
            ar.LyonnaisCountUnder30 += (f.Age < 30);
        }

        if (f.City == "Toulouse") {
            if (f.Name[0] == 'A') {
                ar.ToulousanBeginWithA = true;
            }

            int birthDate = 2018 - f.Age;
            toulousansData << f.Name << "\t" << birthDate << "\t" << f.Time << std::endl;
        }

        if (ar.MinAge > f.Age) {
            ar.MinAge = f.Age;
            ar.MinAgeName = f.Name;
        }
        if (ar.MaxAge < f.Age) {
            ar.MaxAge = f.Age;
            ar.MaxAgeName = f.Name;
        }

        ar.AgeMean += f.Age;
        timeMean += f.Time;
        if (f.City == "Paris") {
            parisiansTimeMean += f.Time;
            parisiansCount++;
        }
        if (f.City == "Marseille") {
            marseillaisTimeMean += f.Time;
            marseillaisCount++;
        }

        ar.Cities.insert(f.City);
    }

    ar.LyonnaisPercentage = 100 * ar.LyonnaisCount / static_cast<double>(datas.size());
    ar.AgeMean /= datas.size();
    timeMean /= datas.size();
    parisiansTimeMean /= parisiansCount;
    marseillaisTimeMean /= marseillaisCount;
    ar.PBetterThanM = parisiansTimeMean < marseillaisTimeMean;

    for (auto& f : datas) {
        double diff = f.Age - ar.AgeMean;
        ar.AgeSd += diff * diff;

        ar.EmpiricalCov += (f.Age - ar.AgeMean) * (f.Time - timeMean);
    }
    ar.EmpiricalCov /= datas.size();
    ar.AgeSd = std::sqrt(ar.AgeSd / datas.size());
}

void PrintAnalysis(const AnalysisResult& ar)
{
    std::cout << "--------------- Analisys results ---------------\n" << std::endl;
    std::cout << "a)  Personnes habitant à Lyon : " << ar.LyonnaisCount << std::endl;
    std::cout << "    Pourcentage de lyonnais   : " << ar.LyonnaisPercentage << std::endl;
    std::cout << "b)  Personnes habitant à Lyon <30: " << ar.LyonnaisCountUnder30 << std::endl;
    std::cout << "c)  Toulousain dont le prénom commence par 'A' ? : " << std::boolalpha << ar.ToulousanBeginWithA << std::endl;
    std::cout << "d)  Le plus agé : " << ar.MaxAge << " " << ar.MaxAgeName << std::endl;
    std::cout << "    Le plus jeune : " << ar.MinAge << " " << ar.MinAgeName << std::endl;
    std::cout << "e)  Age moyen : " << ar.AgeMean << std::endl;
    std::cout << "    Écart-type : " << ar.AgeSd << std::endl;
    std::cout << "f)  Parisiens plus rapides en moyenne que les Marseillais ? : " << std::boolalpha << ar.PBetterThanM << std::endl;
    std::cout << "h)  Covariance empirique entre âge et temps : " << ar.EmpiricalCov << std::endl;
    std::cout << "i)  Liste des villes représentées :" << std::endl;
    for (auto& city : ar.Cities) {
        std::cout << "      " << city << std::endl;
    }
    std::cout << "\n-----------------------------------------------\n" << std::endl;
}

void BetterAnalysis(const std::vector<Fiche>& datas, AnalysisResult& ar)
{
    ar = {};

    ar.LyonnaisCount = std::count_if(datas.begin(), datas.end(), [](Fiche f) {
        return f.City == "Lyon";
    });
    ar.LyonnaisPercentage = 100 * ar.LyonnaisCount / static_cast<double>(datas.size());
    ar.LyonnaisCountUnder30 = std::count_if(datas.begin(), datas.end(), [](Fiche f) {
        return f.City == "Lyon" && f.Age < 30;
    });
    ar.ToulousanBeginWithA = std::any_of(datas.begin(), datas.end(), [](Fiche f) {
        return f.City == "Toulouse" && f.Name[0] == 'A';
    });
    auto AgeCompare = [](Fiche f1, Fiche f2) {
        return f1.Age < f2.Age;
    };
    const Fiche& minFiche = *std::min_element(datas.begin(), datas.end(), AgeCompare);
    ar.MinAge = minFiche.Age;
    ar.MinAgeName = minFiche.Name;

    const Fiche& maxFiche = *std::max_element(datas.begin(), datas.end(), AgeCompare);
    ar.MaxAge = maxFiche.Age;
    ar.MaxAgeName = maxFiche.Name;

    ar.AgeMean = std::accumulate(datas.begin(), datas.end(), 0, [](int a, Fiche f) {
        return a + f.Age;
    }) / static_cast<double>(datas.size());

    double ageMean = ar.AgeMean;
    ar.AgeSd = std::sqrt(std::accumulate(datas.begin(), datas.end(), 0, [ageMean](int a, Fiche f) {
        return a + std::pow(f.Age - ageMean, 2);
    }) / static_cast<double>(datas.size()));

    double parisiansTimeMean = std::accumulate(datas.begin(), datas.end(), 0, [](int a, Fiche f) {
        return a + (f.City == "Paris" ? f.Time : 0);
    }) / static_cast<double>(std::count_if(datas.begin(), datas.end(), [](Fiche f) {
        return f.City == "Paris";
    }));
    double marseillaisTimeMean = std::accumulate(datas.begin(), datas.end(), 0, [](int a, Fiche f) {
        return a + (f.City == "Marseille" ? f.Time : 0);
    }) / static_cast<double>(std::count_if(datas.begin(), datas.end(), [](Fiche f) {
        return f.City == "Marseille";
    }));
    ar.PBetterThanM = parisiansTimeMean < marseillaisTimeMean;

    std::ofstream toulousansData(ar.ToulousansDataFileName);
    std::for_each(datas.begin(), datas.end(), [&toulousansData](Fiche f) {
        if (f.City == "Toulouse") {
            toulousansData << f.Name << "\t" << 2018 - f.Age << "\t" << f.Time << std::endl;
        }
    });

    double timeMean = std::accumulate(datas.begin(), datas.end(), 0.0f, [](double a, Fiche f) {
        return a + f.Time;
    }) / datas.size();
    ar.EmpiricalCov = std::accumulate(datas.begin(), datas.end(), 0.0f, [ageMean, timeMean](double a, Fiche f) {
        return a + (f.Age - ageMean) * (f.Time - timeMean);
    }) / datas.size();

    std::for_each(datas.begin(), datas.end(), [&ar](Fiche f) {
        ar.Cities.insert(f.City);
    });
}

void TimeSortDatas(const std::vector<Fiche>& datas)
{
    std::vector<Fiche> sortedDatas = datas;
    std::sort(sortedDatas.begin(), sortedDatas.end(), [](Fiche& f1, Fiche& f2) {
        return f1.Time < f2.Time;
    });

    constexpr int dataLimit = 100;
    std::ofstream sortDatasFile("data_tri.txt");
    for (int i = dataLimit - 1; i >= 0; --i) {
        sortDatasFile << sortedDatas[i] << std::endl;
    }
}

void SortDatas(const std::vector<Fiche>& datas)
{
    std::vector<Fiche> sortedDatas = datas;
    std::sort(sortedDatas.begin(), sortedDatas.end(), [](Fiche& f1, Fiche& f2) {
        if (f1.Name != f2.Name) {
            return f1.Name < f2.Name;
        }
        if (f1.City != f2.City) {
            return f1.City < f2.City;
        }
        if (f1.Age != f2.Age) {
            return f1.Age > f2.Age;
        }
        return f1.Time < f2.Time;
    });

    std::ofstream sortDatasFile("ordre.dat");
    for (const auto& f : sortedDatas) {
            sortDatasFile << f << std::endl;
    }
}

void AgesHistogramme(const std::vector<Fiche>& datas)
{
    std::map<int, int> agesHist;
    for (const auto& f : datas) {
        agesHist[f.Age]++;
    }

    std::for_each(agesHist.begin(), agesHist.end(), [](std::pair<int, int> e) {
        std::cout << e.first << " : " << e.second << std::endl;
    });
    std::cout << "Classe d'âge la plus nombreuse : " << std::get<0>(*std::max_element(agesHist.begin(), agesHist.end(), [](std::pair<int, int> e1, std::pair<int, int> e2) {
        return e1.second < e2.second;
    })) << std::endl;
}

void SmallestStep(const std::vector<Fiche>& datas)
{
    std::vector<double> times(datas.size());
    std::transform(datas.begin(), datas.end(), std::back_inserter(times), [](const Fiche& f) { return f.Time; });

    std::sort(times.begin(), times.end());
    std::adjacent_difference(times.begin(), times.end(), times.begin());

    double minStep = *std::min_element(times.begin(), times.end());
    std::cout << "Min : " << minStep << std::endl;
}

void YoungFilter(const std::vector<Fiche>& datas)
{
    std::vector<Fiche> jeunes;
    std::vector<Fiche> moinsJeunes;

    std::partition_copy(
        datas.begin(), datas.end(),
        std::back_inserter(jeunes), std::back_inserter(moinsJeunes),
        [](const Fiche& f) { return f.Age < 40; }
    );

    // for (const auto& f : jeunes) {
    //     std::cout << f << std::endl;
    // }
    // for (const auto& f : moinsJeunes) {
    //     std::cout << f << std::endl;
    // }
}