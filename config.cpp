#include "config.h"
#include <yaml-cpp/yaml.h>

Config::Config(const std::string& file) {
    config = YAML::LoadFile(file);
    std::vector<int> payline;

    // REELS
    if (config["wheels"] && config["wheels"].IsMap()) {
        for (const auto &node: config["wheels"]) {
            std::string wheelName = node.first.as<std::string>();   // Reel Name
            std::vector<std::vector<int>> wheelsData;               // Reel Data

            for (const auto &wheel: config["wheels"][wheelName]) {
                std::vector<int> wheelVec;
                for (const auto &value: wheel) {
                    wheelVec.push_back(value.as<int>());
                }
                wheelsData.push_back(wheelVec);
            }
            AllReels[wheelName] = wheelsData;
        }
    }
    // PAYLINES
    for (const auto &line: config["pay_lines"]) {
        std::vector<int> lineVec;
        for (const auto &value: line) {
            lineVec.push_back(value.as<int>());
        }
        paylines.push_back(lineVec);
    }

    payline.clear();

    // PAYTABLE
    if (config["pay_table"]) {
        for (const auto &entry: config["pay_table"]) {
            for (const auto &value: entry.second) {
                payline.push_back(value.as<int>());
            }
            paytable.push_back(payline);
            payline.clear();

        }
    }
    // JACKPOT VALUES
    JackpotProbabilities = config["jackpot_probability"].as<std::vector<double>>();
    JackpotValues = config["jackpots"].as<std::vector<int>>();
}