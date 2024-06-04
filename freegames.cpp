#include "headers.h"
#include "useful.h"
#include <set>




void ReplaceLowWithWilds(std::vector<std::vector<int>>& game_view) {
    for (int i =0; i < 5 ; ++i) {
        for (int j =0; j <3; ++j){
            if ( game_view[i][j] <= 4 ) {
                 game_view[i][j] = 11;
            }
        }
    }
}

BaseGameResults FreeGame(std::map<std::string, ValueType>& configMap) {
    BaseGameResults Results;

    // Check for scatters
    int runs = 0;
    int total_game_rounds = 10;
    int total_respins_won = 0;
    int round_wins = 0;
    std::set<std::pair<int, int>> uniquePairsExtraSpin;
    std::set<std::pair<int, int>> uniquePairs;

    auto& reelFG = configMap.at("reelFG");
    auto& reelRespinFG = configMap.at("reelRespinFG");
    auto& payLines = configMap.at("payLines");
    auto& payTable = configMap.at("payTable");



    while (runs < total_game_rounds) {
        bool won=false;
        bool retrigger_win = false;
        std::vector<std::vector<int>> game_view = GameView(std::get<std::vector<std::vector<int>>>(reelFG));
        std::vector<int> Info = GameViewInfo(game_view);

        Results.Scatters = Info[0];
        Results.StackedWilds = Info[1];
        Results.StackedWildsPosition[0] = std::abs(Info[2]);
        Results.StackedWildsPosition[1] = std::abs(Info[3]);
        Results.StackedWildsPosition[2] = std::abs(Info[4]);

        if ( Results.Scatters > 0) {

            if (Results.Scatters == 2){
                total_game_rounds += 2;

            }
            if (Results.Scatters == 3){
                total_game_rounds += 10;

            }
        }


        if (Results.StackedWilds == 1) {
            Results.OneBaseStackedTriggers += 1;
            Results.BaseStackedTriggers += 1;

        }
        if (Results.StackedWilds == 2) {
            Results.TwoBaseStackedTriggers += 1;
            Results.BaseStackedTriggers += 1;

        }
        if (Results.StackedWilds == 3) {
            Results.ThreeBaseStackedTriggers += 1;
            Results.BaseStackedTriggers += 1;

        }


        int NumberSpins = (Results.StackedWilds > 0) ? 2 : 1;

        for (int i = 0; i < NumberSpins; ++i) {

            if (i == 1) {
                Results.TotalFreeGameRetriggerRounds += 1;
                game_view = GameView(std::get<std::vector<std::vector<int>>>(reelRespinFG));
                for (int pos = 0; pos < 3; ++pos) {
                    if (Info[pos+2] == 1) {
                        game_view[pos + 1][1] = 10;
                        game_view[pos + 1][2] = 10;
                    }
                    else if (Info[pos+2] == -1) {
                        game_view[pos + 1][0] = 10;
                        game_view[pos + 1][1] = 10;
                    }
                }

            }

            if (Results.StackedWilds > 0) {
                ReplaceLowWithWilds(game_view);
            }

//            if( i ==1) {
//                for (int kk = 0; kk < 5; ++kk) {
//                    for (int jj =0 ; jj < 3; ++jj) {
//                        std::cout << game_view[kk][jj] << " ";
//                    }
//                    std::cout << std::endl;
//                }
//                std::cout << std::endl;
//            }


            std::vector<std::vector<int>> game_lines = GetGameLines(game_view,
                                                                    std::get<std::vector<std::vector<int>>>(payLines));

            for (auto game_line: game_lines) {
                std::array<int, 2> WinInfo = analyseArray(game_line,
                                                          std::get<std::vector<std::vector<int>>>(payTable));

                int symbol = WinInfo[0];
                int occurance = WinInfo[1];
                int points = std::get<std::vector<std::vector<int>>>(payTable)[symbol][occurance];


                if (points > 0) {

                    if (i == 0) {
                        if (!won) {
                            round_wins +=1;
                            won=true;
                        }
                        Results.TotalPoints += points;
                        Results.SymbolPoints[symbol][occurance] += points;
                        Results.SymbolHit[symbol][occurance] = 1;

                        if (Results.StackedWilds > 0) {
                            Results.StackedWildsPoints[Results.StackedWilds - 1] += points;
                        }
                    }
                    if (i == 1) {
                        if (!retrigger_win) {
                            Results.TotalFGWinRetriggerRounds += 1;
                            retrigger_win=true;
                        }

                        Results.Extraspin.TotalPoints += points;
                        Results.Extraspin.SymbolPoints[symbol][occurance] += points;
                        Results.Extraspin.SymbolHit[symbol][occurance] = 1;

                    }
                }
            }


        }
        ++runs;
    }
    Results.TotalFGWinRounds += round_wins;
    Results.TotalFreeGameRounds += total_game_rounds;

    return Results;
}
