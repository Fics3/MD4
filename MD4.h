#ifndef MD4_MD4_H
#define MD4_MD4_H


#include <cstddef>
#include <string>


#include <cstdint>
#include <string>
#include <vector>

class MD4 {
public:
    MD4();

    std::vector<int8_t> calculateDigest(const std::string &inputText);

private:
    void updateState(const std::vector<int8_t> &messageBytes, const int32_t &offset, const int32_t &messageLength);

    void processBlock(const std::vector<int8_t> &buffer, int32_t offset);

    void initializeState();

    static std::vector<int8_t> stringToByteVector(const std::string &text);

    static int32_t unsignedRightShift(const int32_t &base, const int32_t &shift);

    static int32_t rotate(const int32_t &t, const int32_t &s);

    uint64_t count{};
    std::vector<int32_t> context;
    std::vector<int32_t> extra;
    std::vector<int8_t> buffer;

    const int32_t A_INIT = 0x67452301;
    const int32_t B_INIT = 0xefcdab89;
    const int32_t C_INIT = 0x98badcfe;
    const int32_t D_INIT = 0x10325476;

    const int32_t CONSTANT_1 = 0x5A827999;
    const int32_t CONSTANT_2 = 0x6ED9EBA1;

    const int32_t BLOCK_LENGTH = 64;
};


#endif //MD4_MD4_H
