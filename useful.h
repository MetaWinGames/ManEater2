#include <vector>
#include <algorithm>
#include<chrono>
#include <iostream>
#include <random>


using std::uniform_int_distribution;
using std::mt19937;

#ifndef MANEATER2_USEFUL_H
#define MANEATER2_USEFUL_H

static mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());

std::vector<std::vector<int>> GameView(std::vector<std::vector<int>>& reel) ;
std::vector<std::vector<int>> GetGameLines(std::vector<std::vector<int>> &game_view, std::vector<std::vector<int>> &paylines);
std::array<int,2>  analyseArray(const std::vector<int> &arr, std::vector<std::vector<int>>& paytable);
std::vector<int> GameViewInfo(std::vector<std::vector<int>>& game_view);






#endif //MANEATER2_USEFUL_H
