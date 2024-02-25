//
// Created by zxggx on 24.02.2024.
//

#ifndef MD4_EXPERIMENT_H
#define MD4_EXPERIMENT_H

#include "MD4.h"

bool isHashBitsEquals(std::vector<int8_t> &hashRandom, std::vector<int8_t> &initialHash, const int bits);

std::string bytesToBits(const std::vector<int8_t> &bytes);

void conductExperiment(MD4 &md4, int num_trials);


std::string generateRandomMessage(int length);

#endif //MD4_EXPERIMENT_H
