//
// Created by sarantis on 30/05/2024.
//

#ifndef MANEATER2_GLOBALS_H
#define MANEATER2_GLOBALS_H
// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <variant>
#include <vector>
#include <map>

// Define the ValueType with possible types
using ValueType = std::variant<int, double, std::string, std::vector<std::vector<int>>, std::vector<std::string>, std::vector<std::vector<double>>>;

// Declare the global map
extern std::map<std::string, ValueType> configMap;

#endif // GLOBALS_H

#endif //MANEATER2_GLOBALS_H
