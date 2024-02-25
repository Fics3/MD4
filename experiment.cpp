#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <bitset>
#include <numeric>
#include "experiment.h"

std::string generateRandomMessage(int length) {
    std::string characters = "0123456789abcdefghijklmnopqrstuvwxyz";
    std::string message;
    for (int i = 0; i < length; ++i) {
        message += characters[rand() % characters.length()];
    }
    return message;
}

std::string bytesToBits(const std::vector<int8_t> &bytes) {
    std::string bits;
    for (const auto &byte: bytes) {
        bits += std::bitset<8>(byte).to_string();
    }
    return bits;
}

bool isHashBitsEquals(std::vector<int8_t> &hashRandom, std::vector<int8_t> &initialHash, const int bits) {
    std::string hashRandomBits = bytesToBits(hashRandom);
    std::string initialHashBits = bytesToBits(initialHash);

    return hashRandomBits.substr(0, bits) == initialHashBits.substr(0, bits);
}


void conductExperiment(MD4 &md4, int num_trials) {
    // Experiment parameters
    const int numExperiments = num_trials;

    // Passwords
    std::vector<std::string> passwords = {"password1", "securePW", "00000000"};

    // Bit sequences to consider
    std::vector<int> bitSizes = {8, 10, 12, 14, 16};
    for (int bits: bitSizes) {
        std::cout << "Bit Size: " << std::dec << bits << std::endl;

        // Storage for experiment results
        std::vector<int> secondPreImageComplexities(numExperiments, 0);
        std::vector<int> collisionComplexities(numExperiments, 0);

        for (int i = 0; i < numExperiments; ++i) {
            // Generate initial hash (y0)
            std::string password = passwords[i % passwords.size()];
            std::vector<int8_t> initialHash = MD4().calculateDigest(password);
            std::vector<std::vector<int8_t>> yHashes;

            int steps = 0;
            // Experiment 1: Finding the second pre-image
            while (true) {
                std::vector<int8_t> hashRandom = md4.calculateDigest(generateRandomMessage(password.size()));
                yHashes.push_back(hashRandom);
                if (isHashBitsEquals(hashRandom, initialHash, bits)) {
                    break;
                }
                steps++;
            }

            secondPreImageComplexities[i] = steps;

            // Experiment 2: Finding collisions
            int collisionSteps = 0;
            for (int j = 0; j < yHashes.size(); ++j) {
                for (int k = j + 1; k < yHashes.size(); ++k) {
                    collisionSteps++;
                    if (isHashBitsEquals(yHashes[j], yHashes[k], bits)) {
                        goto m1;
                    }
                }
            }
            m1:
            collisionComplexities[i] = collisionSteps;
        }

        // Calculate average complexities
        double avgSecondPreImageComplexity = std::accumulate(secondPreImageComplexities.begin(),
                                                             secondPreImageComplexities.end(), 0.0) / numExperiments;
        double avgCollisionComplexity = std::accumulate(collisionComplexities.begin(),
                                                        collisionComplexities.end(), 0.0) / numExperiments;

        std::cout << "Average Second Pre-image Complexity: " << avgSecondPreImageComplexity << " steps" << std::endl;
        std::cout << "Average Collision Complexity: " << avgCollisionComplexity << " steps" << std::endl;
    }
}
