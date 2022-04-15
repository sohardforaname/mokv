#include "slice.tcc"
#include <mcheck.h>

void test()
{
    MOKV::AllocSlice es;
    auto p = es.data();
    MOKV::AllocSlice s("xyz", 3);
    auto s1 = s;
    auto s2 = s1;
}

int main()
{
    setenv("MALLOC_TRACE", "output", 1);
    mtrace();
    test();
    return 0;
}