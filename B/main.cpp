#include <iostream>
#include <vector>

long long power_of_simple[1000010];

const long long big_simple_1 = 77232917, big_simple_2 = 82589933;

void CalcHashes(std::vector<long long>& straight_hash, std::vector<long long>& reverse_hash,
    const std::vector<long long>& data)
{
    straight_hash[0] = data[0];
    reverse_hash[0] = data[data.size() - 1];
    for (size_t i = 1; i < data.size(); ++i) {
        straight_hash[i] = (straight_hash[i - 1] * big_simple_1 + data[i]) % big_simple_2;
        reverse_hash[i] = (reverse_hash[i - 1] * big_simple_1 + data[data.size() - i - 1]) % big_simple_2;
    }
}

long long SectionHash(const std::vector<long long>& hashes, const long long& from, const long long& to)
{
    return (big_simple_2 + hashes[to] - hashes[from - 1] * power_of_simple[to - from + 1] % big_simple_2) % big_simple_2;
}

bool Check(const std::vector<long long>& data, long long rev_from, const long long len)
{
    long long straight_from = rev_from - 1;
    for (long long i = 0; i < len; i += 2) {
        if (data[rev_from++] != data[straight_from--]) {
            return false;
        }
    }

    return true;
}

int main()
{
    // считаем хеши от строки и обратной строки, ищем палиндромы чётной длины, каждый такой палиндром длины 2k ведёт за собой возможный ответ n-k
    long long N = 0, M = 0;
    std::cin >> N >> M;

    power_of_simple[0] = 1;
    for (long long i = 1; i <= N; ++i) {
        power_of_simple[i] = power_of_simple[i - 1] * big_simple_1 % big_simple_2;
    }

    std::vector<long long> colors(N);
    for (long long i = 0; i < N; ++i) {
        std::cin >> colors[i];
    }

    std::vector<long long> straight_hash(N);
    std::vector<long long> reverse_hash(N);
    
    CalcHashes(straight_hash, reverse_hash, colors);

    for (long long i = N / 2; i > 0; --i) {
        long long straight_from = i, straight_to = 2 * i - 1;
        long long rev_from = N - i, suf_to = N - 1;

        if (SectionHash(straight_hash, straight_from, straight_to) == SectionHash(reverse_hash, rev_from, suf_to)) {
            if (Check(colors, straight_from, i)) {
                std::cout << N - i << " ";
            }
        }
    }
    std::cout << N << std::endl;

    return 0;
}
