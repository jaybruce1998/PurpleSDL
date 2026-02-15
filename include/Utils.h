#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace utils {
inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        out.push_back(item);
    }
    return out;
}

inline std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    return s.substr(start, end - start + 1);
}

inline std::vector<std::string> readLines(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

inline std::string readFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

inline double rand01() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

inline int randInt(int minInclusive, int maxExclusive) {
    static std::mt19937 rng(std::random_device{}());
    if (maxExclusive <= minInclusive) {
        return minInclusive;
    }
    std::uniform_int_distribution<int> dist(minInclusive, maxExclusive - 1);
    return dist(rng);
}
}  // namespace utils
