//
// Created by sarantis on 01/02/2024.
//
#include <vector>
#include <chrono>
#include <future>
#include <thread>
#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <map>

#ifndef MANEATER2_CONFIG_H
#define MANEATER2_CONFIG_H


class Config {

public:
    Config(const std::string& file);
    std::map<std::string, std::vector<std::vector<int>>> AllReels;
    std::vector<std::vector<int>> paylines;
    std::vector<std::vector<int>> paytable;
    std::vector<int> JackpotValues;
    std::vector<double> JackpotProbabilities;
    YAML::Node config;
};


#endif //MANEATER2_CONFIG_H
