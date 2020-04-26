#include <cstdio>
#include <cstdlib>
#include <vector>
#include <limits>
 
size_t max (size_t a, size_t b) {return (a > b) ? a : b;}
 
size_t min(size_t a, size_t b) {return (b > a) ? a : b;}
 
struct pair_t {
    int first = 0;
    size_t second;
};
 
constexpr auto neutral = pair_t{std::numeric_limits<int>::min(), 0};
 
template <class T, T (*F)(T, T)>
class SegmentTree_t {
public:
    std::vector<T> data;
    std::vector<T>* tr;
    size_t sz;
 
    SegmentTree_t(std::vector<T> data): data(data) {
 
        sz = data.size();
 
        tr = new std::vector<T>(4*sz, neutral);
 
        rec_build(1, 0, sz - 1);
    }
 
    void rec_build(size_t i, size_t n_left_b, size_t n_right_b){
 
        if (n_right_b == n_left_b){
            (*tr)[i] = data[n_left_b];
            return;
        }
 
        size_t m = (n_left_b + n_right_b)/2;
 
        rec_build(2*i, n_left_b, m);
        rec_build(2*i + 1, m + 1, n_right_b);
 
        (*tr)[i] = F((*tr)[2*i], (*tr)[2*i + 1]);
       
    }
 
   
    T rec_query(size_t i, size_t q_left_b, size_t q_right_b, size_t n_left_b, size_t n_right_b){
   
        if (q_left_b <= n_left_b && n_right_b <= q_right_b){
            return (*tr)[i];
        }
 
        if (q_right_b < n_left_b || q_left_b > n_right_b){
            return neutral;
        }
 
 
        size_t m = (n_left_b + n_right_b)/2;
 
        return F(rec_query(2*i    , q_left_b, q_right_b, n_left_b, m        ),
                 rec_query(2*i + 1, q_left_b, q_right_b, m + 1,    n_right_b));
    }
 
    T query(size_t q_left_b, size_t q_right_b){
        return rec_query(1, q_left_b, q_right_b, 0, sz - 1);
    }
 
};
 
 
pair_t pmax(pair_t a, pair_t b){
    return (a.first > b.first)?(a):(b);
}
 
int main(void){
 
    auto input  = fopen("index-max.in", "r");
    auto output = fopen("index-max.out", "w");
 
    size_t N = 0;
    int cur_val = 0;
 
    fscanf(input, "%lu", &N);
 
    auto vals = std::vector<pair_t>();
    for (size_t i = 0; i < N; ++i){
 
        fscanf(input, "%d", &cur_val);
        vals.push_back(pair_t{cur_val, i});
    }
 
 
    auto t = SegmentTree_t<pair_t, pmax>(vals);
 
    size_t K = 0;
    fscanf(input, "%lu", &K);
 
    size_t cur_l = 0;
    size_t cur_r = 0;
 
    for (size_t i = 0; i < K; ++i){
        fscanf(input, "%lu", &cur_l);
        fscanf(input, "%lu", &cur_r);
        fprintf(output, "%ld\n", t.query(cur_l - 1, cur_r - 1).second + 1);
    }
 
    fclose(input);
    fclose(output);
 
    return 0;
}