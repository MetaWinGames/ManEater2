#include "useful.h"


std::vector<std::vector<int>> GameView(std::vector<std::vector<int>>& reel) {
    std::vector<std::vector<int>> ree = reel;
    std::vector<std::vector<int>> game_view(5, std::vector<int>(3, 0));
    for (int i = 0; i < 5; ++i) {
        if (i >= ree.size() || ree[i].size() < 3) {
            continue;
        }
        auto row = ree[i];
        size_t length_of_wheel = row.size();
        row.push_back(row[0]);
        row.push_back(row[1]);
        row.push_back(row[2]);
        std::uniform_int_distribution<> distrib(0, length_of_wheel-1);
        int random_index = distrib(gen);
        int single_wheel[3] = {row[random_index], row[random_index + 1], row[random_index + 2]};

        for (int j = 0; j < 3; ++j) {
            game_view[i][j] = single_wheel[j];
        }
    }
    return game_view;
}

std::vector<std::vector<int>> GetGameLines(std::vector<std::vector<int>> &game_view, std::vector<std::vector<int>> &paylines) {
    int numLines = 25;
    int numElements = 5;

    std::vector<std::vector<int>> game_lines(numLines, std::vector<int>(numElements, 0));

    for (size_t line = 0; line < numLines; ++line) {
        for (size_t i = 0; i < numElements; ++i) {
            game_lines[line][i] = game_view[i][paylines[line][i]];
        }
    }

    return game_lines;
}

std::array<int, 2>  analyseArray(const std::vector<int> &arr, std::vector<std::vector<int>>& paytable) {
    int value = -1;
    bool isValueSet = false;
    bool countWild = true;
    int count = 0;
    int count_wilds = 0;


    for (int i =0; i < 5; ++i) {
        if ( arr[i] == 10 || arr[i] == 11 || arr[i] == value) {
            ++count;
            if (countWild) {
                ++count_wilds;
            }
        }
        else if (  arr[i] != 10 && arr[i] != 11 && !isValueSet) {
            value = arr[i];
            ++count;
            isValueSet = true;
            countWild = false;
        }
        else {
            break;
        }
    }
    if (count == 5 && !isValueSet) {
        value = 10;
        return {value, count-1};
    }

    if (countWild > 0) {
        if (paytable[value][count-1] < paytable[11][count_wilds-1]) {
            return {10, count_wilds-1};
        }
    }
    return {value , count-1};

}

std::vector<int> GameViewInfo(std::vector<std::vector<int>>& game_view) {
    std::vector<int> info(6,0);
    int Scatter = 0;
    int StackedWilds = 0;
    int pos1 = 0;
    int pos2 = 0;
    int pos3 = 0;
    int highWins = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (game_view[i][j] == 12) {
                ++Scatter;
            }
            if (game_view[i][j] > 4 && game_view[i][j] < 10) {
                highWins += 1;
            }
        }




        if (game_view[i][0] == 10 && game_view[i][1] == 10) {
            if (i == 1) {
                pos1 += 1;
            }
            if (i == 2) {
                pos2 += 1;
            }
            if (i == 3) {
                pos3 += 1;
            }
            StackedWilds += 1;
        }
        if (game_view[i][1] == 10 && game_view[i][2] == 10) {
            if (i == 1) {
                pos1 -= 1;
            }
            if (i == 2) {
                pos2 -= 1;
            }
            if (i == 3) {
                pos3 -= 1;
            }
            StackedWilds += 1;
        }

    }
    info[0]= Scatter;
    info[1]= StackedWilds;
    info[2]= pos1;
    info[3]= pos2;
    info[4]= pos3;
    info[5] = highWins;


    return info;

}

