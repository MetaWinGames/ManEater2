#include <map>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <array>
#include <variant>

#ifndef MANEATER2_HEADERS_H
#define MANEATER2_HEADERS_H

struct Settings {
    std::map<std::string, std::vector<std::vector<int>>> Reels;
    std::vector<std::vector<int>> paylines;
    std::vector<std::vector<int>> paytable;
    std::vector<int> jackpotValues;
    std::vector<double> jackpotProbabilities;
    int default_bet = 25;
};

struct ExtraSpinResults {
    int TotalPoints = 0;
    int SymbolHit[11][5] = {0};
    int SymbolPoints[11][5] = {0};

};


struct BaseGameResults {
    int TotalPoints = 0;
    int StackedWilds = 0;
    int Scatters = 0;
    int SymbolHit[11][5] = {0};
    int SymbolPoints[11][5] = {0};

    int StackedWildsPosition[3] = {0};
    int StackedWildsPoints[3] = {0};

    long long int JackpotHits[3] = {0};
    long long int JackpotPoints[3] = {0};
    long long int TotalFreeGameRounds = 0;
    long long int TotalFreeGameRetriggerRounds = 0;

    long long int TotalFGWinRounds = 0;
    long long int TotalFGWinRetriggerRounds = 0;

    int BaseStackedTriggers = 0;
    int OneBaseStackedTriggers = 0;
    int TwoBaseStackedTriggers = 0;
    int ThreeBaseStackedTriggers = 0;



    ExtraSpinResults Extraspin;

};

using ValueType = std::variant<int, double,
        std::vector<int>,
        std::vector<double>,
        std::string, std::vector<std::vector<int>>,
        std::vector<std::string>,
        std::vector<std::vector<double>>
>;
BaseGameResults BaseGame(std::vector<std::vector<int>>& game_view,std::map<std::string, ValueType>& configMap);
BaseGameResults FreeGame(std::map<std::string, ValueType>& configMap);


struct Stats {
    std::vector<int > TotalPointsArray;

    std::vector<int > TotalBasePointsArray;
    std::vector<int > NormalBasePointsArray;
    std::vector<int > BaseFreeSpinPointsArray;

    std::vector<int > TotalScatterPointsArray;
    std::vector<int > NormalScatterPointsArray;
    std::vector<int > ScatterFreeSpinPointsArray;

    long long int BaseSymbolHit[11][5] = {0};
    long long int BaseSymbolPoints[11][5] = {0};
    long long int BaseRespinSymbolHit[11][5] = {0};
    long long int BaseRespinSymbolPoints[11][5] = {0};


    long long int StackedWildsPositionHit[3] = {0};
    long long int StackedWildsPoints[3] = {0};


    long long int FreeGamesStackedWildsHits[3] = {0};
    long long int StackedWildsPointsFG[3] = {0};


    long long int JackpotHits[3] = {0};
    long long int JackpotPoints[3] = {0};

    // Number of rounds

    long long int TotalFreeGameRetriggerRounds = 0;
    long long int TotalFreeGameRounds = 0;

    // Number of wins

    long long int TotalFGWinRetriggerRounds = 0;
    long long int TotalFGWinRounds = 0;



    long long int ScatterSymbolHit[11][5] = {0};
    long long int ScatterSymbolPoints[11][5] = {0};
    long long int ScatterRespinSymbolHit[11][5] = {0};
    long long int ScatterRespinSymbolPoints[11][5] = {0};

    long long int Scatters = 0;

    long long int BaseStackedTriggers = 0;
    long long int OneBaseStackedTriggers = 0;
    long long int TwoBaseStackedTriggers = 0;
    long long int ThreeBaseStackedTriggers = 0;

    long long int FGStackedTriggers = 0;
    long long int OneFGStackedTriggers = 0;
    long long int TwoFGStackedTriggers = 0;
    long long int ThreeFGStackedTriggers = 0;

};


struct FeatureSubPart {
    double RTP = 0;
    double Variance = 0;
    double StandardDev = 0;
    double HitIntoRate = 0;
    double HitRate = 0;
    double SymbolRTP[11][5] = {0};
    double SymbolHit[11][5] = {0};



};

struct GameSummaryBlock {
    int Iterations = 0;  //
    int Timetaken_ms = 0;   //
    int Timetaken_min = 0;  //
    long long int TotalBet = 0; //
    long long int TotalWon = 0;  //
    int Defaultbet = 0; //
    double RTP = 0; //
    double Variance = 0;
    double StandardDev = 0;
    double HitRate = 0;
    double upper_level = 0;
    double lower_level =0;
    double risk =0;
    double wins_h1 = 0;
    double wins_h2 = 0;
    double wins_h1_hit = 0;
    double wins_h2_hit = 0;
    double wins_low = 0;
    double wins_low_hit = 0;

    double BaseStackedTriggers = 0.0;
    double OneBaseStackedTriggers = 0.0;
    double TwoBaseStackedTriggers = 0.0;
    double ThreeBaseStackedTriggers = 0.0;

    double HitBaseStackedTriggers = 0.0;
    double HitOneBaseStackedTriggers = 0.0;
    double HitTwoBaseStackedTriggers = 0.0;
    double HitThreeBaseStackedTriggers = 0.0;

    double HitScatter = 0;

    double HitFGStackedTriggers = 0;
    double HitOneFGStackedTriggers = 0;
    double HitTwoFGStackedTriggers = 0;
    double HitThreeFGStackedTriggers = 0;

    double AverageFreeGameRounds = 0;

    double StackedWildsRTP[3] = {0};
    double StackedWildsRTPFG[3] = {0};
    double JackpotRTP[3] = {0};
    double JackpotHit[3] = {0};



    FeatureSubPart Game;


};



struct FeatureSummaryBlock {
    FeatureSubPart Base;
    FeatureSubPart FreeGames;

    FeatureSubPart BaseSpins;
    FeatureSubPart BaseRespins;

    FeatureSubPart FeatureBaseSpins;
    FeatureSubPart FeatureRespins;

    double WildsRTP[3] = {0};
    double WildsHits[3] = {0};
    double WildsPosHits[3] = {0};

    double JackpotRTP[3] = {0};
    double JackpotHits[3] = {0};


};

struct GameReport {
    GameSummaryBlock GameSummary;
    FeatureSummaryBlock FeatureSummary;
};


Stats CompressThreadStats(std::vector<Stats> &GameStats);
GameReport GetReport(Stats &CleanedStats,
                     long long int Iterations,
                     int Timetaken_ms,
                     int Timetaken_min,
                     int Defaultbet);


void PrintReport(GameReport &report);

#endif //MANEATER2_HEADERS_H
