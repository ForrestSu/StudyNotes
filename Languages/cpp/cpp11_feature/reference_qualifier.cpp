#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

/**
 * @brief C++11 引用限定符：
 * - 函数的引用限定符作用：
 * - 让成员函数只能被左值对象调用，或者只能被右值对象调用。
 */

/**
 * @brief 测试：限定左值
 */
namespace left_value {
    class Hello {
        void show() & {
            std::cout << "just for left-value\n";
        }
    };

    inline void run() {
        Hello t;
        t.show(); // ok

        Hello{}.show(); // compile error: passing 'left_value::Hello' as 'this' argument discards qualifiers [-fpermissive]
    }
}// namespace

/**
 * @brief 测试：限定右值
 */
namespace right_value {
    struct Test {
        void show() && {
            std::cout << " just for right value\n";
        }
    };

    inline void run() {
        Test t;
        t.show(); //compile error: passing 'right_value::Test' as 'this' argument discards qualifiers [-fpermissive]

        Test{}.show();  //ok
    }
}// namespace

/***
 * @brief 引用限定符+const结合使用
 * eg: 以下是 4 个重载函数
 */
namespace use_with_const_overload {
    struct Hello {
        void f() & {
            std::cout << "lvalue object\n";
        }
        void f() && {
            std::cout << "rvalue object\n";
        }

        void f() const & {
            std::cout << "const lvalue object\n";
        }
        void f() const && {
            std::cout << "const rvalue object\n";
        }
    };

    inline void run() {
        Hello t;
        t.f(); // lvalue
        Hello().f(); // rvalue

        const Hello t3;
        t3.f();            // const lvalue
        std::move(t3).f(); // const rvalue
    }
}// namespace

int main() {
    left_value::run();
    right_value::run();
    use_with_const_overload::run();
    return 0;
}
