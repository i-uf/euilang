#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <functional>
#include <vector>

static bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.size() < suffix.size()) {
        return false;
    }
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static void error(std::string message) {
    throw std::runtime_error("runtime error: " + message);
}

#endif