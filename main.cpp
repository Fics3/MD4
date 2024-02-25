#include <iostream>
#include <iomanip>
#include "MD4.h"
#include "experiment.h"

int main() {
    MD4 md4;
    std::vector<int8_t> result = md4.calculateDigest("Hello, MD4!");

    for (const int8_t &bb: result) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (bb & 0xff);
    }
    std::cout << std::endl;

    conductExperiment(md4, 1000);

    return 0;
}