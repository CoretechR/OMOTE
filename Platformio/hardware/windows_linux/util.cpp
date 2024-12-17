unsigned long reverseBits(unsigned long n) {
    unsigned long reversed = 0;
    int numBits = sizeof(n) * 4; // Number of bits in unsigned long

    for (int i = 0; i < numBits; ++i) {
        if (n & (1ULL << i)) {
            reversed |= (1ULL << ((numBits - 1) - i));
        }
    }
    return reversed;
}