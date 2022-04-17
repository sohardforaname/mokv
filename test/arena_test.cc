//
// Created by ucchi_mchxyz on 2021/12/6.
//

#include "arena.tcc"
#include <chrono>
#include <cstdio>
#include <random>

using namespace std::chrono;

void test()
{
    MOKV::Arena arena;

    for (int i = 0; i < 1e5; ++i) {
        // arena.allocate(1600);
        arena.alignedAllocate(1601);
    }

    printf("%zu\n", arena.allocatedSize());
}

void benchmark()
{
    MOKV::Arena arena;
    std::mt19937 mt(steady_clock::now().time_since_epoch().count());
    static const size_t N = 1e6;

    auto st = steady_clock::now();

    for (int i = 0; i < N; ++i) {
        arena.allocate(mt() % 9000 + 3000);
    }

    auto ed = steady_clock::now();

    auto m = arena.allocatedSize() / 1048576.0;
    auto t = duration_cast<milliseconds>(ed - st).count();
    printf("allocate: %.6f MB, time: %zu ms, speed: %.6f MB/s", m, t, m * 1000.0 / t);
}

#define bench

int main()
{
#ifdef bench
    benchmark();
#else
    test();
#endif
    return 0;
}
