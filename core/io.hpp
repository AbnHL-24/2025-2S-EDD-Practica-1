#pragma once
#include <string>

namespace IO {
    int readInt(const std::string &prompt, int minVal, int maxVal);

    std::string readLine(const std::string &prompt);
}
