//
// Created by ucchi_mchxyz on 2022/1/15.
//

#ifndef MOKV_DEFER_TCC
#define MOKV_DEFER_TCC

#include <functional>
#include <utility>

namespace MOKV {

template <class T>
class Defer {
private:
    T closure_;

public:
    Defer() = delete;

    Defer(const Defer&) = delete;

    Defer(Defer&&) = delete;

    explicit Defer(T& closure)
        : closure_(closure)
    {
    }

    // template<class Fn, class ...Args>
    // explicit Defer(Fn f, Args ...args) : closure_(std::bind(std::move(f), std::move(args)...)) {}

    explicit Defer(T&& closure)
        : closure_(std::move(closure))
    {
    }

    ~Defer()
    {
        closure_();
    }
};
}

#endif // MOKV_DEFER_TCC
