//
// Created by ucchi_mchxyz on 2022/1/23.
//

#ifndef MOKV_COMPARATOR_TCC
#define MOKV_COMPARATOR_TCC

namespace MOKV {
template <class T>
struct Comparator {

    int compare(const char* a, const char* b)
    {
        return static_cast<T*>(this)->compare(a, b);
    }
};
}

#endif // MOKV_COMPARATOR_TCC
