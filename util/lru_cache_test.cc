//
// Created by ucchi_mchxyz on 2021/12/21.
//

#include "lru_cache.tcc"
#include <chrono>
#include <vector>
#include <cstdio>

using namespace std::chrono;

const int N = 1e5;

void benchmark() {
    DB::LruCache<int, int> cache(256);

    std::vector<int> v(N);

    for (auto& n : v) {

    }

    auto st = steady_clock::now();

    for (int i = 0; i < N; ++i) {
        cache.get(v[i]);
        cache.put(v[i] + 1, &v[i]);
    }

    auto ed = steady_clock::now();

    size_t ops = N * 2, tms = duration_cast<microseconds>(ed - st).count();
    printf("op: %zu op, time: %zu ms, speed %.6f op/s\n", ops, tms, ops * 1e6 / tms);
}

int main() {
    benchmark();
    return 0;
}