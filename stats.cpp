#include "headers.h"
#include <tabulate/table.hpp>
#include <tabulate/markdown_exporter.hpp>
#include <numeric>
#include <cmath>
#include <iomanip>

using namespace std;
using namespace tabulate;



void PrintSymbols(double SymbolHit[11][5], double SymbolRTP[11][5]) {
    double check_rtp = 0;
    for (int l = 0; l < 11; ++l) {
        cout <<  std::fixed << std::setprecision(0);

        cout << setw(2) << l << "   ";
        cout <<  std::fixed << std::setprecision(5);

        for (int k = 0; k < 5; ++k) {
            double hit = SymbolHit[l][k];
            if (hit > 0) {
                cout << setw(8) << hit*100 << " ";
            }
            else {
                cout<<  setw(8) << "       " << " ";
            }

        }

        cout << "    ";
        for (int k = 0; k < 5; ++k) {
            double rtp = SymbolRTP[l][k];
            check_rtp += SymbolRTP[l][k];

            if (rtp > 0) {
                cout << setw(8) << rtp * 100<< " "; // Base rtp values
            }
            else {
                cout<<  setw(8) << "       " << " ";
            }
        }
        cout << endl;
    }
    std::cout << check_rtp *100 << std::endl;
    cout << endl << endl;
}

int GetHits(const std::vector<int>& vec) {
    int count = 0;
    for (int num : vec) {
        if (num > 0) {
            ++count;
        }
    }
    return count;
}


std::array<long long int, 6> GetHighWins(const std::vector<int>& flat_array, int default_bet) {
    long long int h1l = static_cast<long long int>(default_bet) * 12;
    long long int h1h = static_cast<long long int>(default_bet) * 40;
    std::array<long long int, 6> win_frequency_count = {0,0,0,0,0,0};

    // h1_hit, h1_points, h2_hit, h2_points
    for (const int val : flat_array) {
        if (val < h1h && val >= h1l) {
            ++win_frequency_count[0];
            win_frequency_count[1] += val;
        } else if (val >= h1h) {
            ++win_frequency_count[2];
            win_frequency_count[3] += val;
        }
        else if (val < h1l){
            ++win_frequency_count[4];
            win_frequency_count[5] += val;
        }
    }

    return win_frequency_count;
}


std::pair<double, double> GetStats(const std::vector<int>& wins, long long sum_wins, long long n, double default_bet) {

    double sum_x_sq = 0;
    for (const auto& win : wins) {
        sum_x_sq += pow(static_cast<double>(win) / default_bet, 2) ;
    }

    long double sum_wins_sq = pow( sum_wins / default_bet, 2) ;

    double var = (1.0 / (n - 1)) * (sum_x_sq - (sum_wins_sq / static_cast<double>(n)));
    double std_dev = std::sqrt(var);

    return {var, std_dev};
}

long long int GetArraySum(std::vector< int> &wins_array) {

    long long int winnigns = std::accumulate(wins_array.begin(), wins_array.end(), 0LL);
    return winnigns;
}

Stats CompressThreadStats(std::vector<Stats> &GameStats) {
    Stats CleanedStats;
    for (Stats &vec: GameStats) {
        CleanedStats.TotalPointsArray.insert(CleanedStats.TotalPointsArray.begin(),
                                        vec.TotalPointsArray.begin(),
                                        vec.TotalPointsArray.end());
        CleanedStats.TotalBasePointsArray.insert(CleanedStats.TotalBasePointsArray.begin(),
                                            vec.TotalBasePointsArray.begin(),
                                            vec.TotalBasePointsArray.end());
        CleanedStats.NormalBasePointsArray.insert(CleanedStats.NormalBasePointsArray.begin(),
                                               vec.NormalBasePointsArray.begin(),
                                               vec.NormalBasePointsArray.end());
        CleanedStats.BaseFreeSpinPointsArray.insert(CleanedStats.BaseFreeSpinPointsArray.begin(),
                                                 vec.BaseFreeSpinPointsArray.begin(),
                                                 vec.BaseFreeSpinPointsArray.end());
        CleanedStats.TotalScatterPointsArray.insert(CleanedStats.TotalScatterPointsArray.begin(),
                                                 vec.TotalScatterPointsArray.begin(),
                                                 vec.TotalScatterPointsArray.end());
        CleanedStats.NormalScatterPointsArray.insert(CleanedStats.NormalScatterPointsArray.begin(),
                                            vec.NormalScatterPointsArray.begin(),
                                            vec.NormalScatterPointsArray.end());
        CleanedStats.ScatterFreeSpinPointsArray.insert(CleanedStats.ScatterFreeSpinPointsArray.begin(),
                                                    vec.ScatterFreeSpinPointsArray.begin(),
                                                    vec.ScatterFreeSpinPointsArray.end());

        CleanedStats.TotalFreeGameRounds += vec.TotalFreeGameRounds;
        CleanedStats.TotalFreeGameRetriggerRounds += vec.TotalFreeGameRetriggerRounds;

        CleanedStats.TotalFGWinRounds += vec.TotalFGWinRounds;
        CleanedStats.TotalFGWinRetriggerRounds += vec.TotalFGWinRetriggerRounds;

        CleanedStats.BaseStackedTriggers += vec.BaseStackedTriggers;
        CleanedStats.OneBaseStackedTriggers += vec.OneBaseStackedTriggers;
        CleanedStats.TwoBaseStackedTriggers += vec.TwoBaseStackedTriggers;
        CleanedStats.ThreeBaseStackedTriggers += vec.ThreeBaseStackedTriggers;

        CleanedStats.Scatters += vec.Scatters;

        CleanedStats.FGStackedTriggers += vec.FGStackedTriggers;
        CleanedStats.OneFGStackedTriggers += vec.OneFGStackedTriggers;
        CleanedStats.TwoFGStackedTriggers += vec.TwoFGStackedTriggers;
        CleanedStats.ThreeFGStackedTriggers += vec.ThreeFGStackedTriggers;


        for (int i = 0; i < 11 ; ++i) {
            for (int j =0; j < 5; ++j) {
                CleanedStats.BaseSymbolHit[i][j] += vec.BaseSymbolHit[i][j];
                CleanedStats.BaseSymbolPoints[i][j] += vec.BaseSymbolPoints[i][j];
                CleanedStats.BaseRespinSymbolHit[i][j] += vec.BaseRespinSymbolHit[i][j];
                CleanedStats.BaseRespinSymbolPoints[i][j] += vec.BaseRespinSymbolPoints[i][j];
                CleanedStats.ScatterSymbolHit[i][j] += vec.ScatterSymbolHit[i][j];
                CleanedStats.ScatterSymbolPoints[i][j] += vec.ScatterSymbolPoints[i][j];
                CleanedStats.ScatterRespinSymbolHit[i][j] += vec.ScatterRespinSymbolHit[i][j];
                CleanedStats.ScatterRespinSymbolPoints[i][j] += vec.ScatterRespinSymbolPoints[i][j];

            }
        }
        for (int i = 0; i < 3; ++i) {
            CleanedStats.JackpotHits[i] += vec.JackpotHits[i];
            CleanedStats.JackpotPoints[i] += vec.JackpotPoints[i];
            CleanedStats.StackedWildsPositionHit[i] += vec.StackedWildsPositionHit[i];
            CleanedStats.StackedWildsPoints[i] += vec.StackedWildsPoints[i];
            CleanedStats.FreeGamesStackedWildsHits[i] += vec.FreeGamesStackedWildsHits[i];
            CleanedStats.StackedWildsPointsFG[i] += vec.StackedWildsPointsFG[i];
        }
    }

    return CleanedStats;
}
GameReport GetReport(Stats &CleanedStats,
                     long long int Iterations,
                     int Timetaken_ms,
                     int Timetaken_min,
                     int Defaultbet) {

    long long int TotalWon = GetArraySum(CleanedStats.TotalPointsArray);

    long long int TotalWon_BASE = GetArraySum(CleanedStats.TotalBasePointsArray);
    long long int TotalWon_BASE_INIT = GetArraySum(CleanedStats.NormalBasePointsArray);
    long long int TotalWon_BASE_FS = GetArraySum(CleanedStats.BaseFreeSpinPointsArray);

    long long int TotalWon_FG = GetArraySum(CleanedStats.TotalScatterPointsArray);
    long long int TotalWon_FG_INIT = GetArraySum(CleanedStats.NormalScatterPointsArray);
    long long int TotalWon_FG_FS = GetArraySum(CleanedStats.ScatterFreeSpinPointsArray);

    std::pair<double, double> TOTAL_STATS = GetStats(CleanedStats.TotalPointsArray, TotalWon, CleanedStats.TotalPointsArray.size(), Defaultbet);

    std::pair<double, double> BASE_STATS = GetStats(CleanedStats.TotalBasePointsArray, TotalWon_BASE, CleanedStats.TotalBasePointsArray.size(), Defaultbet);
    std::pair<double, double> BASE_INIT_STATS = GetStats(CleanedStats.NormalBasePointsArray, TotalWon_BASE_INIT, CleanedStats.NormalBasePointsArray.size(), Defaultbet);
    std::pair<double, double> BASE_FG_STATS = GetStats(CleanedStats.BaseFreeSpinPointsArray, TotalWon_BASE_FS, CleanedStats.BaseFreeSpinPointsArray.size(), Defaultbet);
    std::pair<double, double> TOTAL_FG_STATS = GetStats(CleanedStats.TotalScatterPointsArray, TotalWon_FG, CleanedStats.TotalScatterPointsArray.size(), Defaultbet);
    std::pair<double, double> FG_INIT_STATS = GetStats(CleanedStats.NormalScatterPointsArray, TotalWon_FG_INIT, CleanedStats.NormalScatterPointsArray.size(), Defaultbet);
    std::pair<double, double> FG_FS_STATS = GetStats(CleanedStats.ScatterFreeSpinPointsArray, TotalWon_FG_FS, CleanedStats.ScatterFreeSpinPointsArray.size(), Defaultbet);

    std::array<long long int,6> HighWins = GetHighWins(CleanedStats.TotalPointsArray, Defaultbet);

    GameReport report;
    report.GameSummary.Iterations = Iterations;
    report.GameSummary.Timetaken_ms = Timetaken_ms;
    report.GameSummary.Timetaken_min = Timetaken_min;
    report.GameSummary.Defaultbet = Defaultbet;
    report.GameSummary.TotalBet = Iterations * Defaultbet;
    report.GameSummary.TotalWon = TotalWon;
    report.GameSummary.Defaultbet = Defaultbet;

    report.GameSummary.wins_h1 = HighWins[1] / static_cast<double>(Iterations  * Defaultbet);
    report.GameSummary.wins_h1_hit = HighWins[0] / static_cast<double>(Iterations);
    report.GameSummary.wins_h2 = HighWins[3] / static_cast<double>(Iterations * Defaultbet);
    report.GameSummary.wins_h2_hit = HighWins[2]/ static_cast<double>(Iterations);
    report.GameSummary.wins_low = HighWins[5] / static_cast<double>(Iterations * Defaultbet);
    report.GameSummary.wins_low_hit = HighWins[4] / static_cast<double>(Iterations );


    report.GameSummary.RTP = TotalWon / static_cast<double>(Iterations * Defaultbet);




    report.GameSummary.Variance = TOTAL_STATS.first;
    report.GameSummary.StandardDev = TOTAL_STATS.second;
    report.GameSummary.HitRate = GetHits(CleanedStats.TotalPointsArray) / static_cast<double>(Iterations);
    report.GameSummary.upper_level = report.GameSummary.RTP + (1.96 * (TOTAL_STATS.second / sqrt(Iterations)));
    report.GameSummary.lower_level = report.GameSummary.RTP - (1.96 * (TOTAL_STATS.second / sqrt(Iterations)));
    report.GameSummary.risk =  TOTAL_STATS.first / (1 - report.GameSummary.RTP);

    report.GameSummary.HitBaseStackedTriggers = CleanedStats.BaseStackedTriggers / static_cast<double>(Iterations);
    report.GameSummary.HitOneBaseStackedTriggers = CleanedStats.OneBaseStackedTriggers / static_cast<double>(Iterations);
    report.GameSummary.HitTwoBaseStackedTriggers = CleanedStats.TwoBaseStackedTriggers / static_cast<double>(Iterations);
    report.GameSummary.HitThreeBaseStackedTriggers = CleanedStats.ThreeBaseStackedTriggers / static_cast<double>(Iterations);

    std::cout <<  "Check 1 " << static_cast<double>(CleanedStats.OneFGStackedTriggers) << endl;
    std::cout <<  "Check 2  " << static_cast<double>(CleanedStats.TwoFGStackedTriggers) << endl;
    std::cout <<  "Check 3  " << static_cast<double>(CleanedStats.ThreeFGStackedTriggers) << endl;

    report.GameSummary.HitFGStackedTriggers = CleanedStats.FGStackedTriggers / static_cast<double>(CleanedStats.TotalFreeGameRounds);
    report.GameSummary.HitOneFGStackedTriggers = CleanedStats.OneFGStackedTriggers / static_cast<double>(CleanedStats.TotalFreeGameRounds);
    report.GameSummary.HitTwoFGStackedTriggers = CleanedStats.TwoFGStackedTriggers / static_cast<double>(CleanedStats.TotalFreeGameRounds);
    report.GameSummary.HitThreeFGStackedTriggers = CleanedStats.ThreeFGStackedTriggers / static_cast<double>(CleanedStats.TotalFreeGameRounds);

    report.GameSummary.HitScatter = CleanedStats.Scatters / static_cast<double>(Iterations);
    report.GameSummary.AverageFreeGameRounds = CleanedStats.TotalFreeGameRounds / static_cast<double>(CleanedStats.Scatters);

    std::cout <<  "FG GAMES " << static_cast<double>(CleanedStats.Scatters) << endl;
    std::cout <<  "TOTAL FG ROUNDS " << static_cast<double>( CleanedStats.TotalFreeGameRounds) << endl;


    for (int i = 0; i < 3; ++i) {
        report.GameSummary.JackpotRTP[i] = CleanedStats.JackpotPoints[i] / static_cast<double>(Iterations* Defaultbet);
        report.GameSummary.JackpotHit[i] = CleanedStats.JackpotHits[i] /  static_cast<double>(Iterations);
        report.GameSummary.StackedWildsRTP[i] += CleanedStats.StackedWildsPoints[i] /  static_cast<double>(Iterations* Defaultbet);
        report.GameSummary.StackedWildsRTPFG[i] += CleanedStats.StackedWildsPointsFG[i] /  static_cast<double>(Iterations* Defaultbet);

    }

    // GAME
    report.GameSummary.Game.RTP = TotalWon / static_cast<double>(Iterations * Defaultbet);
    report.GameSummary.Game.Variance = TOTAL_STATS.first;
    report.GameSummary.Game.StandardDev = TOTAL_STATS.second;
    report.GameSummary.Game.HitRate = GetHits(CleanedStats.TotalPointsArray) / static_cast<double>(Iterations);


    // TOTAL BASE
    report.FeatureSummary.Base.RTP = TotalWon_BASE / static_cast<double>(Iterations * Defaultbet);
    report.FeatureSummary.Base.Variance = BASE_STATS.first;
    report.FeatureSummary.Base.StandardDev = BASE_STATS.second;
    report.FeatureSummary.Base.HitIntoRate = 0;
    report.FeatureSummary.Base.HitRate = GetHits(CleanedStats.TotalBasePointsArray) / static_cast<double>(CleanedStats.TotalBasePointsArray.size());

    // TOTAL BASE INIT
    report.FeatureSummary.BaseSpins.RTP = TotalWon_BASE_INIT / static_cast<double>(Iterations * Defaultbet);
    report.FeatureSummary.BaseSpins.Variance = BASE_INIT_STATS.first;
    report.FeatureSummary.BaseSpins.StandardDev = BASE_INIT_STATS.second;
    report.FeatureSummary.BaseSpins.HitIntoRate = 0;
    report.FeatureSummary.BaseSpins.HitRate = GetHits(CleanedStats.NormalBasePointsArray) / static_cast<double>(CleanedStats.NormalBasePointsArray.size());

    // TOTAL BASE FS
    report.FeatureSummary.BaseRespins.RTP = TotalWon_BASE_FS / static_cast<double>(Iterations * Defaultbet);
    report.FeatureSummary.BaseRespins.Variance = BASE_FG_STATS.first;
    report.FeatureSummary.BaseRespins.StandardDev = BASE_FG_STATS.second;
    report.FeatureSummary.BaseRespins.HitIntoRate = 0;
    report.FeatureSummary.BaseRespins.HitRate = GetHits(CleanedStats.BaseFreeSpinPointsArray) / static_cast<double>(CleanedStats.BaseFreeSpinPointsArray.size());

    // TOTAL FG
    report.FeatureSummary.FreeGames.RTP = TotalWon_FG / static_cast<double>(Iterations * Defaultbet);
    report.FeatureSummary.FreeGames.Variance = TOTAL_FG_STATS.first;
    report.FeatureSummary.FreeGames.StandardDev = TOTAL_FG_STATS.second;
    report.FeatureSummary.FreeGames.HitIntoRate = 0;
    report.FeatureSummary.FreeGames.HitRate = GetHits(CleanedStats.TotalScatterPointsArray) / static_cast<double>(CleanedStats.TotalScatterPointsArray.size());

    // TOTAL FG INIT
    report.FeatureSummary.FeatureBaseSpins.RTP = TotalWon_FG_INIT / static_cast<double>(Iterations * Defaultbet);
    report.FeatureSummary.FeatureBaseSpins.Variance = FG_INIT_STATS.first;
    report.FeatureSummary.FeatureBaseSpins.StandardDev = FG_INIT_STATS.second;
    report.FeatureSummary.FeatureBaseSpins.HitIntoRate = 0;
    report.FeatureSummary.FeatureBaseSpins.HitRate =  CleanedStats.TotalFGWinRounds / static_cast<double>(CleanedStats.TotalFreeGameRounds);

    // TOTAL FG FS
    report.FeatureSummary.FeatureRespins.RTP = TotalWon_FG_FS / static_cast<double>(Iterations * Defaultbet);
    report.FeatureSummary.FeatureRespins.Variance = FG_FS_STATS.first;
    report.FeatureSummary.FeatureRespins.StandardDev = FG_FS_STATS.second;
    report.FeatureSummary.FeatureRespins.HitIntoRate = 0;
    report.FeatureSummary.FeatureRespins.HitRate = CleanedStats.TotalFGWinRetriggerRounds / static_cast<double>(CleanedStats.TotalFreeGameRetriggerRounds);

    std::cout << CleanedStats.TotalFGWinRetriggerRounds << std::endl;
    std::cout << static_cast<double>(CleanedStats.TotalFreeGameRetriggerRounds)<< std::endl;
    for (int i = 0; i < 11 ; ++i) {
        for (int j =0; j < 5; ++j) {
            report.FeatureSummary.BaseSpins.SymbolHit[i][j] = CleanedStats.BaseSymbolHit[i][j] /static_cast<double>(Iterations) ;
            report.FeatureSummary.BaseSpins.SymbolRTP[i][j] = CleanedStats.BaseSymbolPoints[i][j]/ static_cast<double>(Iterations*  Defaultbet) ;

            report.FeatureSummary.BaseRespins.SymbolHit[i][j] = CleanedStats.BaseRespinSymbolHit[i][j] /static_cast<double>(Iterations) ;
            report.FeatureSummary.BaseRespins.SymbolRTP[i][j]  = CleanedStats.BaseRespinSymbolPoints[i][j]/static_cast<double>(Iterations*  Defaultbet);

            report.FeatureSummary.FeatureBaseSpins.SymbolHit[i][j] = (CleanedStats.ScatterSymbolHit[i][j]) / static_cast<double>(CleanedStats.TotalFreeGameRounds);
            report.FeatureSummary.FeatureBaseSpins.SymbolRTP[i][j]  = CleanedStats.ScatterSymbolPoints[i][j]/static_cast<double>(Iterations*  Defaultbet) ;

            report.FeatureSummary.FeatureRespins.SymbolHit[i][j] = CleanedStats.ScatterRespinSymbolHit[i][j] /static_cast<double>(CleanedStats.TotalFreeGameRetriggerRounds) ;
            report.FeatureSummary.FeatureRespins.SymbolRTP[i][j] = CleanedStats.ScatterRespinSymbolPoints[i][j]/ static_cast<double>(Iterations*  Defaultbet) ;
        }
    }
    return report;
}

void PrintReport(GameReport &report) {
    Table ReportTable;
    ReportTable.format().font_style({FontStyle::bold}).font_align(FontAlign::center).width(130);

    Table GameSummaryTable;

    GameSummaryTable.add_row({"Iterations", to_string(report.GameSummary.Iterations)});
    GameSummaryTable.add_row({"Time Taken", to_string(report.GameSummary.Timetaken_ms)});
    GameSummaryTable.add_row({"Total Bet", to_string(report.GameSummary.TotalBet)});
    GameSummaryTable.add_row({"Total Won", to_string(report.GameSummary.TotalWon)});
    GameSummaryTable.add_row({"Default Bet", to_string(report.GameSummary.Defaultbet)});
    GameSummaryTable.add_row({"RTP", to_string(report.GameSummary.RTP*100)});
    GameSummaryTable.add_row({"Upper Conf L", to_string(report.GameSummary.upper_level*100)});
    GameSummaryTable.add_row({"Lower Conf L", to_string(report.GameSummary.lower_level*100)});
    GameSummaryTable.add_row({"Risk", to_string(report.GameSummary.risk)});
    GameSummaryTable.add_row({"RTP 12x - 40x", to_string(report.GameSummary.wins_h1*100)});
    GameSummaryTable.add_row({"RTP 40x - ", to_string(report.GameSummary.wins_h2*100)});
    GameSummaryTable.add_row({"HIT 12x - 40x", to_string(report.GameSummary.wins_h1_hit)});
    GameSummaryTable.add_row({"HIT 40x - ", to_string(report.GameSummary.wins_h2_hit)});




    Table FeatureSummaryTable;
    FeatureSummaryTable.add_row({"Feature", "RTP", "Variance", "Standard Dev", "Hit Rate"});

    FeatureSummaryTable.add_row({"GAME",
                                 to_string(report.GameSummary.Game.RTP * 100),
                                 to_string(report.GameSummary.Game.Variance),
                                 to_string(report.GameSummary.Game.StandardDev),
                                 to_string(report.GameSummary.Game.HitRate),
                                });
    FeatureSummaryTable.add_row({"",
                                 "","","",""
                                });
    FeatureSummaryTable.add_row({"Base Total",
                                 to_string(report.FeatureSummary.Base.RTP * 100),
                                 to_string(report.FeatureSummary.Base.Variance),
                                 to_string(report.FeatureSummary.Base.StandardDev),
                                 to_string(report.FeatureSummary.Base.HitRate),
                                });
    FeatureSummaryTable.add_row({"Normal Spin",
                                 to_string(report.FeatureSummary.BaseSpins.RTP*100),
                                 to_string(report.FeatureSummary.BaseSpins.Variance),
                                 to_string(report.FeatureSummary.BaseSpins.StandardDev),
                                 to_string(report.FeatureSummary.BaseSpins.HitRate),
                                });
    FeatureSummaryTable.add_row({" Respin",
                                 to_string(report.FeatureSummary.BaseRespins.RTP*100),
                                 to_string(report.FeatureSummary.BaseRespins.Variance),
                                 to_string(report.FeatureSummary.BaseRespins.StandardDev),
                                 to_string(report.FeatureSummary.BaseRespins.HitRate),
                                 });
    FeatureSummaryTable.add_row({"",
                                 "","","",""
                                });

    FeatureSummaryTable.add_row({"Free Spins Total",
                                 to_string(report.FeatureSummary.FreeGames.RTP*100),
                                 to_string(report.FeatureSummary.FreeGames.Variance),
                                 to_string(report.FeatureSummary.FreeGames.StandardDev),
                                 to_string(report.FeatureSummary.FreeGames.HitRate),
                                });
    FeatureSummaryTable.add_row({"Free Spins Normal",
                                 to_string(report.FeatureSummary.FeatureBaseSpins.RTP*100),
                                 to_string(report.FeatureSummary.FeatureBaseSpins.Variance),
                                 to_string(report.FeatureSummary.FeatureBaseSpins.StandardDev),
                                 to_string(report.FeatureSummary.FeatureBaseSpins.HitRate),
                               });
    FeatureSummaryTable.add_row({"Free Spins Respin",
                                 to_string(report.FeatureSummary.FeatureRespins.RTP*100),
                                 to_string(report.FeatureSummary.FeatureRespins.Variance),
                                 to_string(report.FeatureSummary.FeatureRespins.StandardDev),
                                 to_string(report.FeatureSummary.FeatureRespins.HitRate),
                                });


//    Table HitFrequency;
//    HitFrequency.add_row({"Feature", "Hit rate 1 in "});
//    HitFrequency.add_row({"Free games ", to_string(1/ report.GameSummary.HitScatter)});
//    HitFrequency.add_row({"Base Retrigger", to_string(1 / report.GameSummary.HitBaseStackedTriggers)});
//    HitFrequency.add_row({"Base 1 Retriggers", to_string(1 / report.GameSummary.HitOneBaseStackedTriggers)});
//    HitFrequency.add_row({"Base 2 Retriggers", to_string(1 / report.GameSummary.HitTwoBaseStackedTriggers)});
//    HitFrequency.add_row({"Base 3 Retriggers", to_string(1 / report.GameSummary.HitThreeBaseStackedTriggers)});
//
//    HitFrequency.add_row({"FG Retriggers", to_string(1 / report.GameSummary.HitFGStackedTriggers)});
//    HitFrequency.add_row({"FG 1 Retriggers", to_string(1 / report.GameSummary.HitOneFGStackedTriggers)});
//    HitFrequency.add_row({"FG 2 Retriggers", to_string(1 / report.GameSummary.HitTwoFGStackedTriggers)});
//    HitFrequency.add_row({"FG 3 Retriggers", to_string(1 / report.GameSummary.HitThreeFGStackedTriggers)});


    Table FreeGameInfo;
    FreeGameInfo.add_row({"Average Number of Rounds", to_string(report.GameSummary.AverageFreeGameRounds)});
    FreeGameInfo.add_row({"Hit Into Free Games", to_string(1/ report.GameSummary.HitScatter)});

    Table Jackpots;

    Jackpots.add_row({"reel jackpots", "rtp", "hit"});
    Jackpots.add_row({"JP 1",
                      to_string(report.GameSummary.JackpotRTP[0]),
                      to_string( report.GameSummary.JackpotHit[0])
    });
    Jackpots.add_row({"JP 2",
                      to_string(report.GameSummary.JackpotRTP[1]),
                      to_string( report.GameSummary.JackpotHit[1])
                     });
    Jackpots.add_row({"JP 3",
                      to_string(report.GameSummary.JackpotRTP[2]),
                      to_string( report.GameSummary.JackpotHit[2])
                     });

    Table StackedWildsSummmary;
    StackedWildsSummmary.add_row({"Number of stacked Wilds", "rtp", "hit"});
    StackedWildsSummmary.add_row({"Base",
                                  to_string((report.GameSummary.StackedWildsRTP[0]+
                                  report.GameSummary.StackedWildsRTP[1]+
                                  report.GameSummary.StackedWildsRTP[2])*100),

                                  to_string(1 / report.GameSummary.HitBaseStackedTriggers)
                                 });
    StackedWildsSummmary.add_row({"1", to_string(report.GameSummary.StackedWildsRTP[0]*100),
                                  to_string(1 / report.GameSummary.HitOneBaseStackedTriggers)
    });

    StackedWildsSummmary.add_row({"2", to_string(report.GameSummary.StackedWildsRTP[1]*100),
                                  to_string(1 / report.GameSummary.HitTwoBaseStackedTriggers)
    });
    StackedWildsSummmary.add_row({"3", to_string(report.GameSummary.StackedWildsRTP[2]*100),
                                  to_string(1 / report.GameSummary.HitThreeFGStackedTriggers)
    });
    StackedWildsSummmary.add_row({"Free Games",
                                  to_string((report.GameSummary.StackedWildsRTPFG[0]+
                                            report.GameSummary.StackedWildsRTPFG[1]+
                                            report.GameSummary.StackedWildsRTPFG[2])*100),

                                  to_string(1 / report.GameSummary.HitFGStackedTriggers)
                                 });

    StackedWildsSummmary.add_row({"1", to_string(report.GameSummary.StackedWildsRTPFG[0]*100),
                                  to_string(1 / report.GameSummary.HitOneFGStackedTriggers)
                                 });

    StackedWildsSummmary.add_row({"2", to_string(report.GameSummary.StackedWildsRTPFG[1]*100),
                                  to_string(1 / report.GameSummary.HitTwoFGStackedTriggers)
                                 });
    StackedWildsSummmary.add_row({"3", to_string(report.GameSummary.StackedWildsRTPFG[2]*100),
                                  to_string(1 / report.GameSummary.HitThreeFGStackedTriggers)
                                 });


    ReportTable.add_row({GameSummaryTable});
    ReportTable.add_row({FeatureSummaryTable});
//    ReportTable.add_row({HitFrequency});
    ReportTable.add_row({FreeGameInfo});
    ReportTable.add_row({StackedWildsSummmary});

    ReportTable.add_row({Jackpots});





    std::cout << ReportTable << endl;
    std::cout << "Base"<< endl;
    PrintSymbols(report.FeatureSummary.BaseSpins.SymbolHit, report.FeatureSummary.BaseSpins.SymbolRTP);
    std::cout << "Base Respin"<< endl;

    PrintSymbols(report.FeatureSummary.BaseRespins.SymbolHit,report.FeatureSummary.BaseRespins.SymbolRTP);
    std::cout << "FG"<< endl;

    PrintSymbols(report.FeatureSummary.FeatureBaseSpins.SymbolHit,report.FeatureSummary.FeatureBaseSpins.SymbolRTP);

    std::cout << "FG Respin"<< endl;
    PrintSymbols(report.FeatureSummary.FeatureRespins.SymbolHit, report.FeatureSummary.FeatureRespins.SymbolRTP);

}

