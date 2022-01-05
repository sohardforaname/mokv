//
// Created by ucchi_mchxyz on 2021/11/30.
//

#include "skip_list.tcc"
#include <chrono>

using namespace std::chrono;

template<class T>
struct IntCmp {
    static int compare(T a, T b) {
        if (a < b) {
            return -1;
        }
        return a == b ? 0 : 1;
    }
};

#define bench

void test() {
    DB::SkipList<long long, IntCmp<long long>> skip_list;
    skip_list.insert(1);
    skip_list.insert(4);
    skip_list.insert(7);
    skip_list.insert(6);
    skip_list.insert(5);
    printf("scan: %zu\n", skip_list.scan([&](const long long v) -> bool {
        printf("%lld\n", v);
        return v % 2;
    }));

    skip_list.erase(5);
    skip_list.erase(10);

    printf("scan: %zu\n", skip_list.scan([&](const long long v) -> bool {
        printf("%lld\n", v);
        return v % 2;
    }));
}

void benchmark() {
    const int N = 3.2e4;
    DB::SkipList<long long, IntCmp<long long>> skip_list;
    auto st = steady_clock::now();

    for (int i = N - 1; i >= 0; --i) {
        skip_list.insert(i);
    }

    for (int i = N - 1; i >= 0; --i) {
        skip_list.insert(i);
    }

    for (int i = 0; i < N; ++i) {
        skip_list.find(i);
    }

    for (int i = 0; i < N; ++i) {
        skip_list.find(i + N);
    }

    auto ed = steady_clock::now();
    auto op = N * 4;
    auto t = duration_cast<microseconds>(ed - st).count();
    printf("ops: %d, time: %zu us, speed: %.6f op/s", op, t, op * 1.0 / t * 1e6);

}

int main() {
#ifdef bench
    benchmark();
#else
    test();
#endif
    return 0;
}