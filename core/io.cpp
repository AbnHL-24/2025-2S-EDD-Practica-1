#include "io.hpp"
#include <iostream>
#include <limits>

int IO::readInt(const std::string &prompt, int minVal, int maxVal) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) {
            if (v >= minVal && v <= maxVal) return v;
            std::cout << "Valor fuera de rango [" << minVal << "," << maxVal << "]\n";
        } else {
            std::cin.clear();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string IO::readLine(const std::string &prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin >> std::ws, s);
    return s;
}
