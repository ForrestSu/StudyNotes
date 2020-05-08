/**
 *  Author: sunquan
 *  Date: 2020-03-18 17:36:01
 */

#include <typeinfo>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <functional>

namespace test_future {

template<typename T>
class ready_future;

template<typename T>
struct is_future: std::false_type {
};

template<typename T>
struct is_future<ready_future<T>> : std::true_type {
};

//template <typename _Tp>
//  inline constexpr bool is_future_v = is_future<_Tp>::value;

template<typename T>
class ready_future {
public:
    using value_type = T;
    std::string _name;
    ready_future(const std::string &name) :
            _name(name) {
    }
    /*
     ready_future(ready_future& x) = delete;
     ready_future(ready_future &&x) {
     std::cout << "move:" << x._name << std::endl;
     _name.swap(x._name);
     }*/
    void show() {
        printf("show this = %p,  %s\n", this, _name.c_str());
    }
    ~ready_future() {
        std::cout << "~release(): " << _name << std::endl;
    }
public:

    template<typename Func, typename Enable>
    void then(Func, Enable);

    template<typename Func>
    void then(Func &&func, std::enable_if_t<std::is_same<std::result_of_t<Func(T&&)>, void>::value, void*> = nullptr) {
        //typename std::result_of_t<Func(T&&)> MYtype;
        std::cout << "v1: void" << std::endl;
        func(10);
    }

    // if return future
    template<typename Func>
    std::result_of_t<Func(T&&)> then(Func &&func, std::enable_if_t<is_future<std::result_of_t<Func(T&&)>>::value, void*> = nullptr) {
        //typename std::result_of_t<Func(T&&)> MYtype;
        // std::cout << typeid(MYtype).name() << endl;

        using U = typename std::result_of_t<Func(T&&)>::value_type;
        std::cout << "v2:" << typeid(U).name() << std::endl;
        printf("this = %p,  %s\n", this, _name.c_str());

        auto fut = func(10);
        return fut;
    }

};

inline void run() {
    ready_future<int> fut { "v0" };
    fut.then([](int x) {
        ready_future<int> retfut { "task1" };
        retfut.show();
        return retfut;
    }).then([](int x) {
        ready_future<int> retfut { "task2" };
        retfut.show();
        return retfut;
    }).then([](int x) {
        ready_future<int> retfut { "task3" };
        retfut.show();
        return retfut;
    }).then([](int x) {
        std::cout << "last: " << x << std::endl;
    });
}

inline void benchmark() {
    // about 100 ns/op, 10^7 op/s
    for (int i = 0; i < 10'000'000; i++) {
        run();
    }
}

}

int main() {
    test_future::run();
    //test_future::benchmark();
    return 0;
}
