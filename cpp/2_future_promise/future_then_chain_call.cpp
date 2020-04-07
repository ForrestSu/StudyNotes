/**
 *  Author: sunquan
 *  Date: 2020-03-18 17:36:01
 */

#include <typeinfo>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <functional>

using namespace std;


template <typename T>
class my_future;


template <typename T>
struct is_future : std::false_type {};

template <typename T>
struct is_future<my_future<T>> : std::true_type {};

template <typename Tp>
  inline constexpr bool is_future_v = is_future<Tp>::value;


template <typename T>
class my_future {
public:
    using value_type = T;
    std::string _name;
    my_future(const std::string& name):_name(name){
    }
    void show(){
        //std::cout<< _name <<std::endl;
    }
public:

    template <typename Func, typename Enable>
    void then(Func, Enable);

    template<typename Func>
    void then(Func &&func,
            std::enable_if_t<std::is_same<std::result_of_t<Func(T&&)>, void>::value, void*> = nullptr)
    {
        //typename std::result_of_t<Func(T&&)> MYtype;
        std::cout <<"v1: void" << endl;
        func(10);
    }

    // if return future
    template<typename Func>
    std::result_of_t<Func(T&&)> then(Func &&func,
            std::enable_if_t<is_future<std::result_of_t<Func(T&&)>>::value, void*> = nullptr)
    {
        //typename std::result_of_t<Func(T&&)> MYtype;
       // std::cout << typeid(MYtype).name() << endl;

        using U = typename std::result_of_t<Func(T&&)>::value_type;
        std::cout <<"v2:" << typeid(U).name() << endl;

        auto fut = func(10);
        return std::move(fut);
    }

};


inline void run(){
    my_future<int> fut{"v0"};
    fut.then([](int x) {
        my_future<int> retfut{"task1"};
        retfut.show();
        return std::move(retfut);
    }).then([](int x){
        my_future<int> retfut{"task2"};
        retfut.show();
        return std::move(retfut);
    }).then([](int x){
        my_future<int> retfut{"task3"};
        retfut.show();
        return std::move(retfut);
    }).then([](int x){
        std::cout<< "last: " << x << std::endl;
    });
}

inline void benchmark() {
    // about 100 ns/op, 10^7 op/s
    for (int i = 0; i < 10'000'000; i++) {
        run();
    }
}

int main()
{
    run();
    return 0;
}
