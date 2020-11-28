#include <algorithm>
#include <cstdio>
#include <string.h>
#include <vector>

char str[1000001] = "";

struct Word {
    char* pos = nullptr;
    int len = 0;

    bool operator < (Word &word2)
    {
        int min_length = std::min(this->len, word2.len);
        int cmp = strncmp(this->pos, word2.pos, min_length);

        if (cmp == 0)
            return this->len < word2.len;

        return cmp < 0;
    }
};

std::vector<Word> words;
std::vector<Word> sorted;


int main()
{
    char* cur_ptr = str;

    scanf("%c", cur_ptr);
    if (*cur_ptr != '.') {
        words.push_back({cur_ptr, 1});
    }

    scanf("%c", ++cur_ptr);

    while (EOF != *cur_ptr && '\0' != *cur_ptr && '\n' != *cur_ptr) {

        if (*cur_ptr != '.' && *(cur_ptr - 1) != '.') {
            ++(words[words.size() - 1].len);
        }

        else if (*cur_ptr != '.' && *(cur_ptr - 1) == '.') {
            words.push_back({cur_ptr, 1});
        }

        scanf("%c", ++cur_ptr);
    }
    *cur_ptr = '\0';

    sorted = words;


    std::sort(sorted.begin(), sorted.end());
    
    
    if (0 == words.size()) {
        printf("%s", str);
        return 0;
    }

    int dots_qty = words[0].pos - str;
    fwrite(str, 1, dots_qty, stdout);

    for (int i = 0; i < sorted.size() - 1; ++i) {
        fwrite(sorted[i].pos, 1, sorted[i].len, stdout);

        dots_qty = words[i + 1].pos - words[i].pos - words[i].len;
        fwrite(words[i].pos + words[i].len, 1, dots_qty, stdout);
    }

    fwrite(sorted[sorted.size() - 1].pos, 1, sorted[sorted.size() - 1].len, stdout);

    printf("%s\n", words[words.size() - 1].pos + words[words.size() - 1].len);


    return 0;
}
