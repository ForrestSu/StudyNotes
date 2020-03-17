//============================================================================
// Name        : cpp_template.cpp
// Author      : sunquan
// Version     : v0.1
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <complex>
using namespace std;

/**
 * 1 function templates
 */
namespace Test01 {
// function templates
template<typename T>
inline T max(T a, T b) {
	return a > b ? a : b;
}

template <>
bool max<bool>(bool a, bool b) {
	std::cout<< "call specialization max<bool>()" << std::endl;
    return a || b;
}

inline void run() {
	//隐式参数推导调用 max<int>
	std::cout << "max(a, b) :" << max(1, 2) << std::endl;
	//隐式参数推导调用 max<int>
    std::cout << "max(a, b) :" << max(5.0, 8.0) << std::endl;

//    compile error
//    std::cout << max(1, 7.0) << std::endl;

    // 显式地转换入参
    std::cout << max<double>(3, 7.0) << std::endl;

    std::cout << max(true, false) << std::endl;
}
}




int main()
{
	Test01::run();
	return 0;
}
