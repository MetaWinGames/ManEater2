#include "headers.h"
#include "useful.h"
#include <set>
using std::uniform_int_distribution;
using std::mt19937;

static mt19937 gens(std::chrono::high_resolution_clock::now().time_since_epoch().count());

std::uniform_real_distribution<> distr(0.0, 1.0);


void ReplaceHighWithWilds(std::vector<std::vector<int>>& game_view,
                          int& toTurn) {
    int lookfor = 5;

    while (toTurn > 0 && lookfor < 10) {
        for (int i =0; i < 5 ; ++i) {
            for (int j =0; j <3; ++j){
                if (game_view[i][j] == lookfor) {
                    game_view[i][j] = 11;
                    toTurn -= 1;
                }

            }
        }
        lookfor += 1;
    }

}
int chooseIndex(const std::vector<double>& probabilities) {
    std::random_device rd;
    std::mt19937 gen(rd());

    double randomValue = distr(gen);
    double cumulativeSum = 0.0;
    for (size_t i = 0; i < probabilities.size(); ++i) {
        cumulativeSum += probabilities[i];
        if (randomValue <= cumulativeSum) {
            return i;
        }
    }
    return probabilities.size() - 1;
}


//std::uniform_real_distribution<> distr(0.0, 1.0);

using ValueType = std::variant<int, double,
        std::vector<int>,
        std::vector<double>,
        std::string, std::vector<std::vector<int>>,
        std::vector<std::string>,
        std::vector<std::vector<double>>
>;


BaseGameResults BaseGame(std::vector<std::vector<int>>& game_view,
                         std::map<std::string, ValueType>& configMap) {
    BaseGameResults Results;
    std::vector<int> Info = GameViewInfo(game_view);
    Results.Scatters = Info[0];
    Results.StackedWilds = Info[1];
    Results.StackedWildsPosition[0] = std::abs(Info[2]);
    Results.StackedWildsPosition[1] = std::abs(Info[3]);
    Results.StackedWildsPosition[2] = std::abs(Info[4]);

    int NumberSpins = (Results.StackedWilds > 0) ? 2 : 1;

     for (int i = 0; i <NumberSpins; ++i) {

        if (i == 1) {
            auto& reelRespinBase = configMap.at("reelRespinBase");
            game_view = GameView( std::get<std::vector<std::vector<int>>>(reelRespinBase) );

            for (int pos = 0; pos < 3; ++pos) {

                if (Info[pos+2] != 0) {

                    if (Info[pos+2] == 1) {
                        game_view[pos + 1][1] = 10;
                        game_view[pos + 1][2] = 10;
                    }
                    else if (Info[pos+2] == -1) {
                        game_view[pos + 1][0] = 10;
                        game_view[pos + 1][1] = 10;
                    }

                    double val = distr(gens);
                    auto& jackpotProb = configMap.at("jackpotProb");
                    auto& jackpotValues = configMap.at("jackpotValues");
//                    std::cout << val << std::endl;
//                    std::cout << "aaaa" << std::endl;

                    bool hasHitJackpot = (std::get<std::vector<double>>(jackpotProb)[pos + 1] >= val);
                    if (hasHitJackpot) {
//                        std::cout << "POI" << std::endl;
                        Results.JackpotHits[pos] += 1;
                        Results.JackpotPoints[pos] += std::get<std::vector<int>>(jackpotValues)[pos + 1] * 25;
                    }
                }

            }
        }

        if (Results.StackedWilds > 0) {
            std::string setChoice;
            if (Results.StackedWilds == 1) {
                setChoice = "probTransformOneStacked";
            }
            if (Results.StackedWilds == 2) {
                setChoice = "probTransformTwoStacked";
            }
            if (Results.StackedWilds == 3) {
                setChoice = "probTransformThreeStacked";
            }
//            std::cout << Info[5] << std::endl;
            if (Info[5] > 0) {
                auto& highWinsToTurn = std::get<std::vector<std::vector<double>>>(configMap.at(setChoice))[Info[5]-1];
                int valsToTurn = chooseIndex(static_cast<const std::vector<double>>(highWinsToTurn));
                ReplaceHighWithWilds(game_view, valsToTurn);
            }

        }

        auto& payLines = configMap.at("payLines");
        std::vector<std::vector<int>> game_lines = GetGameLines(game_view,std::get<std::vector<std::vector<int>>>(payLines));
        std::set<std::pair<int, int>> uniquePairs;

         auto& payTable = configMap.at("payTable");
         for (auto game_line: game_lines) {


            std::array<int, 2> WinInfo = analyseArray(game_line,
                                                      std::get<std::vector<std::vector<int>>>(payTable));
            int symbol = WinInfo[0];
            int occurance = WinInfo[1];
            int points = std::get<std::vector<std::vector<int>>>(payTable)[symbol][occurance];

            if (points > 0) {
                if (i == 0) {
                    Results.TotalPoints += points;
                    Results.SymbolPoints[symbol][occurance] += points;
                    Results.SymbolHit[symbol][occurance] = 1;

                    if (Results.StackedWilds > 0) {
                        Results.StackedWildsPoints[Results.StackedWilds - 1] += points;
                    }
                }
                if (i == 1) {

                    Results.Extraspin.TotalPoints += points;
                    Results.Extraspin.SymbolPoints[symbol][occurance] += points;
                    Results.Extraspin.SymbolHit[symbol][occurance] += 1;

                }
            }
        }

    }

    return Results;
}


//void TestBaseGame() {
//    std::vector<std::vector<int>>& game_view = {
//            {}
//    }
//    std::vector<int> Info = GameViewInfo(game_view);
//    Results.Scatters = Info[0];
//    Results.StackedWilds = Info[1];
//    Results.StackedWildsPosition[0] = std::abs(Info[2]);
//    Results.StackedWildsPosition[1] = std::abs(Info[3]);
//    Results.StackedWildsPosition[2] = std::abs(Info[4]);
//}
