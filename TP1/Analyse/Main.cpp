#include <vector>

#include "Analysis.hpp"

int main()
{
    std::vector<Fiche> datas {};
    if (!FetchDatas(datas))
        return 1;

    AnalysisResult analisysResult {};
    Analysis(datas, analisysResult);
    PrintAnalysis(analisysResult);

    BetterAnalysis(datas, analisysResult);
    PrintAnalysis(analisysResult);

    TimeSortDatas(datas);

    SortDatas(datas);
    AgesHistogramme(datas);

    SmallestStep(datas);
    YoungFilter(datas);

    return 0;
}