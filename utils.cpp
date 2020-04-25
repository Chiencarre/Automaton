//
// Created by lenna on 25/04/2020.
//

#include <vector>
#include <sstream>
#include "utils.h"


void utils::splitString(std::string p_string, std::vector<std::string> &result, char delim) {
    std::stringstream ss(p_string);
    std::string token;
    while (std::getline(ss, token, delim)) {
        result.push_back(token);
    }
}