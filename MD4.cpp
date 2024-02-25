#include <algorithm>
#include "MD4.h"

#define F(X, Y, Z) (((X) & (Y)) | ((~X) & (Z)))
#define G(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))

MD4::MD4() {
    initializeState();
}

std::vector<int8_t> MD4::calculateDigest(const std::string &inputText) {
    updateState(stringToByteVector(inputText), 0, inputText.length());

    const auto bufferIndex = static_cast<int32_t>(count % BLOCK_LENGTH);
    const int32_t paddingLength = (bufferIndex < 56) ? 56 - bufferIndex : 120 - bufferIndex;

    std::vector<int8_t> paddingAndLength(paddingLength + 8, 0);
    paddingAndLength[0] = static_cast<int8_t>(0x80);

    for (int32_t i = 0; i < 8; ++i) {
        paddingAndLength[paddingLength + i] = static_cast<int8_t>(unsignedRightShift(count * 8, 8 * i));
    }

    updateState(paddingAndLength, 0, paddingAndLength.size());

    std::vector<int8_t> result(16, 0);
    for (int32_t i = 0; i < 4; ++i) {
        for (int32_t j = 0; j < 4; ++j) {
            result[i * 4 + j] = static_cast<int8_t>(unsignedRightShift(context[i], 8 * j));
        }
    }

    initializeState();
    return result;
}

void MD4::updateState(const std::vector<int8_t> &messageBytes, const int32_t &offset, const int32_t &messageLength) {
    auto bufferIndex = static_cast<int32_t>(count % BLOCK_LENGTH);
    count += messageLength;
    const int32_t partialLength = BLOCK_LENGTH - bufferIndex;
    int32_t i = 0;

    if (messageLength >= partialLength) {
        for (int32_t j = 0; j < partialLength; ++j) {
            buffer[bufferIndex + j] = messageBytes[offset + j];
        }
        processBlock(buffer, 0);
        i = partialLength;
        while (i + BLOCK_LENGTH - 1 < messageLength) {
            processBlock(messageBytes, offset + i);
            i += BLOCK_LENGTH;
        }
        bufferIndex = 0;
    }

    if (i < messageLength) {
        for (int32_t j = 0; j < messageLength - i; ++j) {
            buffer[bufferIndex + j] = messageBytes[offset + i + j];
        }
    }
}

void MD4::processBlock(const std::vector<int8_t> &buffer, int32_t offset) {
    for (int32_t i = 0; i < 16; ++i) {
        extra[i] = ((buffer[offset + 0] & 0xff)) |
                   ((buffer[offset + 1] & 0xff) << 8) |
                   ((buffer[offset + 2] & 0xff) << 16) |
                   ((buffer[offset + 3] & 0xff) << 24);
        offset += 4;
    }

    int32_t a = context[0];
    int32_t b = context[1];
    int32_t c = context[2];
    int32_t d = context[3];

    for (const int32_t &i: {0, 4, 8, 12}) {
        a = rotate(a + F(b, c, d) + extra[i + 0], 3);
        d = rotate(d + F(a, b, c) + extra[i + 1], 7);
        c = rotate(c + F(d, a, b) + extra[i + 2], 11);
        b = rotate(b + F(c, d, a) + extra[i + 3], 19);
    }

    for (const int32_t &i: {0, 1, 2, 3}) {
        a = rotate(a + G(b, c, d) + extra[i + 0] + CONSTANT_1, 3);
        d = rotate(d + G(a, b, c) + extra[i + 4] + CONSTANT_1, 5);
        c = rotate(c + G(d, a, b) + extra[i + 8] + CONSTANT_1, 9);
        b = rotate(b + G(c, d, a) + extra[i + 12] + CONSTANT_1, 13);
    }

    for (const int32_t &i: {0, 2, 1, 3}) {
        a = rotate(a + H(b, c, d) + extra[i + 0] + CONSTANT_2, 3);
        d = rotate(d + H(a, b, c) + extra[i + 8] + CONSTANT_2, 9);
        c = rotate(c + H(d, a, b) + extra[i + 4] + CONSTANT_2, 11);
        b = rotate(b + H(c, d, a) + extra[i + 12] + CONSTANT_2, 15);
    }

    context[0] += a;
    context[1] += b;
    context[2] += c;
    context[3] += d;
}

void MD4::initializeState() {
    count = 0;
    context.assign(4, 0);
    context[0] = A_INIT;
    context[1] = B_INIT;
    context[2] = C_INIT;
    context[3] = D_INIT;
    extra.assign(16, 0);
    buffer.assign(BLOCK_LENGTH, 0);
}

std::vector<int8_t> MD4::stringToByteVector(const std::string &text) {
    std::vector<int8_t> bytes;
    bytes.reserve(text.size());

    std::transform(text.begin(), text.end(), std::back_inserter(bytes),
                   [](char ch) { return static_cast<int8_t>(ch); });

    return bytes;
}

int32_t MD4::unsignedRightShift(const int32_t &base, const int32_t &shift) {
    if (shift < 0 || shift >= 32 || base == 0) {
        return 0;
    }
    return (base > 0 ? base : static_cast<uint32_t>(base)) >> shift;
}

int32_t MD4::rotate(const int32_t &t, const int32_t &s) {
    return (t << s) | unsignedRightShift(t, 32 - s);
}
