#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <execution>
#include <future>
#include <thread>
#include <set>
#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>
#include <cmath>
#include <utility>
#include <iomanip>
#include <stdexcept>
#include "headers.h"
#include "config.h"
#include "useful.h"
#include <tabulate/table.hpp>
#include <tabulate/markdown_exporter.hpp>
#include <sstream>
#include <locale>
#include <cmath>
//#include "globals.h"

using std::accumulate;
using std::cout;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using ValueType = std::variant<int, double,
        std::vector<int>,
        std::vector<double>,
        std::string, std::vector<std::vector<int>>,
        std::vector<std::string>,
        std::vector<std::vector<double>>
        >;

void GameRuns(int start, int end,
              std::map<std::string, ValueType>& configMap,
              Stats &GameStats) {
    for (int k = start; k < end; ++k)
    {
        // Get Game View
        auto& reelBase = configMap.at("reelBase");
        std::vector<std::vector<int>> game_view = GameView(std::get<std::vector<std::vector<int>>>(reelBase));



        int TotalPoints = 0;
        BaseGameResults BaseGameResult = BaseGame(game_view,configMap);

        TotalPoints += BaseGameResult.TotalPoints + BaseGameResult.Extraspin.TotalPoints;
        GameStats.TotalBasePointsArray.push_back(BaseGameResult.TotalPoints + BaseGameResult.Extraspin.TotalPoints);
        GameStats.NormalBasePointsArray.push_back(BaseGameResult.TotalPoints);
        GameStats.BaseFreeSpinPointsArray.push_back(BaseGameResult.Extraspin.TotalPoints);



        if (BaseGameResult.Scatters == 3) {
            GameStats.Scatters += 1;
        }

        if (BaseGameResult.StackedWilds > 0) {
            GameStats.BaseStackedTriggers += 1;

            if (BaseGameResult.StackedWilds == 1) {
                GameStats.OneBaseStackedTriggers += 1;

            } else if (BaseGameResult.StackedWilds == 2) {
                GameStats.TwoBaseStackedTriggers += 1;

            } else if (BaseGameResult.StackedWilds == 3) {
                GameStats.ThreeBaseStackedTriggers += 1;
            }

        }

        for (int i = 0; i < 11; ++i) {
            for (int j = 0; j < 5; ++j) {
                GameStats.BaseSymbolHit[i][j] += BaseGameResult.SymbolHit[i][j];
                GameStats.BaseSymbolPoints[i][j] += BaseGameResult.SymbolPoints[i][j];
                GameStats.BaseRespinSymbolHit[i][j] += BaseGameResult.Extraspin.SymbolHit[i][j];
                GameStats.BaseRespinSymbolPoints[i][j] += BaseGameResult.Extraspin.SymbolPoints[i][j];
            }
        }
        for (int i = 0; i < 3; ++i) {
            GameStats.StackedWildsPositionHit[i] += BaseGameResult.StackedWildsPosition[i];
            GameStats.StackedWildsPoints[i] +=  BaseGameResult.StackedWildsPoints[i];

            GameStats.JackpotHits[i] += BaseGameResult.JackpotHits[i];
            GameStats.JackpotPoints[i] += BaseGameResult.JackpotPoints[i];

        }


        // Check if 3 Scatters Run FreeGames
         if (BaseGameResult.Scatters == 3 ) {
            BaseGameResults FreeGameResults = FreeGame(configMap);

            TotalPoints += FreeGameResults.TotalPoints + FreeGameResults.Extraspin.TotalPoints;
            GameStats.TotalScatterPointsArray.push_back(FreeGameResults.TotalPoints + FreeGameResults.Extraspin.TotalPoints);
            GameStats.NormalScatterPointsArray.push_back(FreeGameResults.TotalPoints);
            GameStats.ScatterFreeSpinPointsArray.push_back(FreeGameResults.Extraspin.TotalPoints);

            // Number of rounds
            GameStats.TotalFreeGameRounds += FreeGameResults.TotalFreeGameRounds;
            GameStats.TotalFreeGameRetriggerRounds += FreeGameResults.TotalFreeGameRetriggerRounds;

            // Number of wins
            GameStats.TotalFGWinRounds += FreeGameResults.TotalFGWinRounds;
            GameStats.TotalFGWinRetriggerRounds += FreeGameResults.TotalFGWinRetriggerRounds;

            GameStats.FGStackedTriggers += FreeGameResults.BaseStackedTriggers;
            GameStats.OneFGStackedTriggers += FreeGameResults.OneBaseStackedTriggers;
            GameStats.TwoFGStackedTriggers += FreeGameResults.TwoBaseStackedTriggers;
            GameStats.ThreeFGStackedTriggers += FreeGameResults.ThreeBaseStackedTriggers;
            for (int i = 0; i < 11; ++i) {
                for (int j = 0; j < 5; ++j) {
                    GameStats.ScatterSymbolHit[i][j] += FreeGameResults.SymbolHit[i][j];
                    GameStats.ScatterSymbolPoints[i][j] += FreeGameResults.SymbolPoints[i][j];
                    GameStats.ScatterRespinSymbolHit[i][j] += FreeGameResults.Extraspin.SymbolHit[i][j];
                    GameStats.ScatterRespinSymbolPoints[i][j] += FreeGameResults.Extraspin.SymbolPoints[i][j];
                }
            }
            for (int i = 0; i < 3; ++i) {
                 GameStats.FreeGamesStackedWildsHits[i] += FreeGameResults.StackedWildsPosition[i];
                 GameStats.StackedWildsPointsFG[i] += FreeGameResults.StackedWildsPoints[i];
            }

         }
        GameStats.TotalPointsArray.push_back(TotalPoints);

    }
}

int main() {
    std::map<std::string, ValueType> configMap;

    configMap["payTable"] = std::vector<std::vector<int>>{
            {0, 0, 1, 5, 25}, // 0  l
            {0, 0, 1, 5, 25},  // 1  l
            {0, 0, 1, 5, 25},   // 2  l
            {0, 0, 1, 5, 25},   // 3  l
            {0,0,1,10,50 },     // 4  l
            {0,0,1,10,50 },     // 5   h
            {0,0,1,10,50 },     // 6   h
            {0,0,1,10,50 },     // 7   h
            {0,0,1,10,50 },     // 8   h
            {0,0,1,10,50 },     // 9   h
            {0,0,1,10,50 },     // 10  venus
            {0,0,1,10,50 },     // 11  wild
            {0,0,0,0,0},        // 12 scatter

    };
    // REEL FOR MAIN GAME
    configMap["reelBase"] = std::vector<std::vector<int>>{
            {0,9,6,1,8,2,12,3,4,0,5,1,7,0,4,3,1,2,0,9,2,6,8,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,10,10,3,4,0,5,1,7,0,4,3,1,2,0,11,9,2,6,8,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,10,10,3,4,0,5,1,7,0,4,3,1,2,0,11,9,2,6,8,12,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,10,10,3,4,0,5,1,7,0,4,3,1,2,0,11,9,2,6,8,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,12,3,4,0,5,1,7,0,4,3,1,11,2,0,11,9,2,6,8,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6}
    };
    // REEL FOR FREE GAMES (SCATTER GAMES)
    configMap["reelFG"] = std::vector<std::vector<int>>{
            {0,9,6,1,8,2,12,3,4,0,5,1,7,0,4,3,1,2,0,9,2,6,8,6,0,4,12,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,10,10,3,4,0,5,1,7,0,4,3,1,2,0,11,9,2,6,8,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,10,10,3,4,0,5,1,7,0,4,3,1,2,0,11,9,2,6,8,12,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,10,10,3,4,0,5,1,7,0,4,3,1,2,0,11,9,2,6,8,6,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6},
            {0,9,6,1,8,2,12,3,4,0,5,1,7,0,4,3,1,11,2,0,11,9,2,6,8,6,12,0,4,3,5,2,7,9,0,1,6,8,5,3,9,7,2,8,6}
    };

    // REEL FOR MAIN RESPIN
    configMap["reelRespinBase"] = std::vector<std::vector<int>>{
            {4,2,1,0,5,2,3,0,7,1,3,4,8,0,1,0,8,4,4,9,0,1,2,6,2,1,0},
            {0,0,6,0,1,2,3,3,4,0,1,3,5,2,0,6,4,3,4,8,1,0,4,7,1,0,3,9},
            {2,1,0,5,2,3,0,7,1,3,4,8,0,1,0,8,4,4,9,0,1,2,6,2,1,0},
            {0,0,6,0,1,2,3,3,4,0,1,3,5,2,0,6,4,3,4,8,1,0,4,7,1,0,3,9},
            {2,1,0,5,2,3,0,7,1,3,4,8,0,1,0,8,4,4,9,0,1,2,6,2,1,0},
    };

    // REEL FOR FREE GAMES RESPIN
    configMap["reelRespinFG"] = std::vector<std::vector<int>>{
            {5,6,6,5,1,7,5,6,7,9,5,6,7,9,5,9,8,7,5,6,6,5},
            {9,8,8,9,7,8,8,5,1,8,6,8,8,2,8,9,9,9,9,9,6},
            {5,6,6,5,1,7,5,6,7,9,5,6,7,9,5,9,8,7,5,6,6,5},
            {9,8,8,9,7,8,8,5,8,6,8,8,8,9,9,9,9,9,6},
            {5,6,6,5,1,7,5,6,7,9,5,6,7,9,5,9,8,7,5,6,6,5},
            };


    // PROBABILITIES OF TRANSFORMING HIGH SYMBOLS TO WILDS IN BASE GAME IF 1 STACKED WILD PRESENT
    configMap["probTransformOneStacked"] =  std::vector<std::vector<double>>{
            { 1 }, // 1 HIGH SYMBOLS PRESENT
            { 0.2, 0.8 }, // 2 HIGH SYMBOLS PRESENT, PROBABILITY TO TURN 1/2 TO WILDS  = 0.2, PROBABILITY TO TURN 2/2 TO WILDS = 0.8
            { 0.1, 0.3, 0.6 }, // 3
            {0.1, 0.1, 0.2, 0.6 }, // 4
            {0.1, 0.1, 0.1, 0.1, 0.6 }, // 5
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.3 }, // 6
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.2, 0.1}, // 7
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.1, 0.1, 0.1}, // 8
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.7, 0.2, 0.1}, // 9
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.7, 0.1, 0.1, 0.1}, // 10
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1}, // 11
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0}, // 12
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0}, // 13
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0}, // 14


    };
    // PROBABILITIES OF TRANSFORMING HIGH SYMBOLS TO WILDS IN BASE GAME IF 2 STACKED WILD PRESENT

    configMap["probTransformTwoStacked"] =  std::vector<std::vector<double>>{
            { 1 }, // 1
            { 0.2, 0.8 }, // 2
            { 0.1, 0.3, 0.6 }, // 3
            {0.1, 0.1, 0.2, 0.6 }, // 4
            {0.1, 0.1, 0.1, 0.1, 0.6 }, // 5
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.3 }, // 6
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.2, 0.1}, // 7
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.1, 0.1, 0.1}, // 8
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.7, 0.2, 0.1}, // 9
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.7, 0.1, 0.1, 0.1}, // 10
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1}, // 11
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0}, // 12
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0}, // 13
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0}, // 14


    };

    // PROBABILITIES OF TRANSFORMING HIGH SYMBOLS TO WILDS IN BASE GAME IF 3 STACKED WILD PRESENT
    configMap["probTransformThreeStacked"] =  std::vector<std::vector<double>>{
            { 1 }, // 1
            { 0.2, 0.8 }, // 2
            { 0.1, 0.3, 0.6 }, // 3
            {0.1, 0.1, 0.2, 0.6 }, // 4
            {0.1, 0.1, 0.1, 0.1, 0.6 }, // 5
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.3 }, // 6
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.2, 0.1}, // 7
            {0.1, 0.1, 0.1, 0.1, 0.3, 0.1, 0.1, 0.1}, // 8
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.7, 0.2, 0.1}, // 9
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.7, 0.1, 0.1, 0.1}, // 10
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1}, // 11
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0}, // 12
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0}, // 13
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0}, // 14


    };

    configMap["jackpotValues"] =  std::vector<int>{
            0,10,500,200,0
    };
    configMap["jackpotProb"] =  std::vector<double>{
            0, 0.1, 0.1, 0.1, 0
    };

    configMap["payLines"] = std::vector<std::vector<int>>{
            {1,1,1,1,1},
            {0,0,0,0,0},
            {2,2,2,2,2},
            {0,1,2,0,1},
            {2,1,0,1,2},
            {0,0,1,2,2},
            {2,2,1,0,0},
            {0,1,1,1,0},
            {2,1,1,1,2},
            {1,0,0,0,1},
            {1,2,2,2,1},
            {1,1,0,1,1},
            {1,1,2,1,1},
            {0,1,0,1,0},
            {2,1,2,1,2},
            {0,2,0,2,0},
            {2,0,2,0,2},
            {0,2,2,2,0},
            {2,0,0,0,2},
            {1,0,1,2,1},
            {1,2,1,0,1},
            {0,0,2,2,2},
            {2,2,0,0,0},
            {0,2,1,2,0},
            {2,0,1,0,2}
    };



    std::vector<std::thread> threads;
    // SIMULATION SETTINGS
    long long int num_elements = 10'000'000;  // NUMBER OF ITERATIONS TO BE RUN
    const int num_threads = 20;         // NUMBER OF CORES TO USE

    std::vector<Stats> GameStats(num_threads);

    const int elements_per_thread = num_elements / num_threads;
    auto start_time = std::chrono::high_resolution_clock::now();;


    for (int i = 0; i < num_threads; ++i) {
        int start = i * elements_per_thread;
        int end = (i == num_threads - 1) ? num_elements : start + elements_per_thread;
        threads.emplace_back(GameRuns,
                             start, end,
                             std::ref(configMap),
                             std::ref(GameStats[i])

        );
    }

    for (auto &thread: threads) {
        thread.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    auto duration_mins = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time);
    std::cout << "T: " << duration.count() << "  Mins: " << duration_mins.count() << std::endl;



    Stats CleanedStats = CompressThreadStats(GameStats);

    GameReport report = GetReport(CleanedStats,
                                  num_elements,
                                  duration.count(),
                                  duration_mins.count(),
                                  25);
    PrintReport(report);

    return 0;
}
