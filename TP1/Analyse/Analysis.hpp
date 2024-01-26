#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <set>

struct Fiche
{
    std::string Name;
    std::string City;
    int Age;
    double Time;

    friend std::ostream& operator<<(std::ostream& os, const Fiche& f);
};

struct AnalysisResult
{
    // a)
    std::size_t LyonnaisCount;
    double LyonnaisPercentage;

    // b)
    std::size_t LyonnaisCountUnder30;

    // c)
    bool ToulousanBeginWithA;

    // d)
    int MinAge;
    int MaxAge;
    std::string MinAgeName;
    std::string MaxAgeName;

    // e)
    double AgeMean;
    double AgeSd;

    // f)
    bool PBetterThanM;

    // g)
    const char* ToulousansDataFileName = "toulousans.txt";

    // h)
    double EmpiricalCov;

    // i)
    std::set<std::string> Cities;
};

bool FetchDatas(std::vector<Fiche>& datas);

void Analysis(const std::vector<Fiche>& datas, AnalysisResult& analisysResult);
void PrintAnalysis(const AnalysisResult& analisysResult);

void BetterAnalysis(const std::vector<Fiche>& datas, AnalysisResult& analisysResult);

void TimeSortDatas(const std::vector<Fiche>& datas);

void SortDatas(const std::vector<Fiche>& datas);
void AgesHistogramme(const std::vector<Fiche>& datas);

void SmallestStep(const std::vector<Fiche>& datas);
void YoungFilter(const std::vector<Fiche>& datas);
