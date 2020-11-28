﻿#include <iostream>
#include <vector>
#include <assert.h>
 
 
int powP[1000001];
 
 
void CalcHashes(std::vector<long long>& origHashes, std::vector<long long>& revHashes,
                const std::vector<long long>& data, const int p, const int q) {

    long long size = data.size();
 
    origHashes[0] = data[0];
    revHashes[0] = data[size - 1];
    for (int i = 1; i < size; ++i) {
        origHashes[i] = (origHashes[i - 1] * p + data[i]) % q;
        revHashes[i] = (revHashes[i - 1] * p + data[size - i - 1]) % q;
    }
}
 
 
long long MidHash(const std::vector<long long>& hashes, const long long& beg, const long long& end,
                  const int& p, const int& q) {

 
    return (q + hashes[end] - hashes[beg - 1] * powP[end - beg + 1] % q) % q;
}
 
 
bool Check(const std::vector<long long>& data, long long revBeg, const long long len) {

 
    long long origBeg = revBeg - 1;
    for (int i = 0; i < len; i += 2)
        if (data[revBeg++] != data[origBeg--])
            return false;
 
    return true;
}
 
 
int main() {
    const long long p = 999983, q = 10004983;
 
    int N = 0, M = 0;
    std::cin >> N >> M;
 
    powP[0] = 1;
    for (int i = 1; i < N + 1; ++i)
        powP[i] = powP[i - 1] * p % q;
 
    std::vector<long long> cubes(N);
    for (int i = 0; i < cubes.size(); ++i)
        std::cin >> cubes[i];
 
    std::vector<long long> origHashes(N);
    std::vector<long long> revHashes(N);
    CalcHashes(origHashes, revHashes, cubes, p, q);
 
 
    for (long long i = N / 2; i > 0; --i) {
        long long origBeg = i, origEnd = 2 * i - 1;
        long long revBeg = N - i, revEnd = N - 1ll;
 
        if (MidHash(origHashes, origBeg, origEnd, p, q) == MidHash(revHashes, revBeg, revEnd, p, q))
            if (Check(cubes, origBeg, i))
                std::cout << N - i << " ";
    }
    std::cout << N;
 
    return 0;
}