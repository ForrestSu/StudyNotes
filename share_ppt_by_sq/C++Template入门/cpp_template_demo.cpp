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

/**
 * 2 class templates
 */
namespace Test02 {
// class templates
template<typename T>
class ClassX // template definition
{
public:
	void show() {
		std::cout<< "show" << std::endl;
	}
};

template<>
class ClassX<int>
{
public:
	void show() {
		std::cout<< "<int>show" << std::endl;
	}
};

inline void run() {
	ClassX<int> obj;
	obj.show();
}
}

/**
 * 3 variable template
 */
namespace Test03 {
template<typename T>
constexpr T pi = T(3.1415926535897932385L);  // variable template

template<typename T>
T circular_area(T r) // function template
{
    return pi<T> * r * r; // pi<T> is a variable template instantiation
}
inline void run() {
	std::cout<< "area is "<< circular_area<double>(10) <<std::endl;
}
}




/**
 * 4 Template aliases
 */
namespace Test04 {
template<class T>
using StrVec = std::vector<std::string>;
}


namespace Test05 {

template<typename T1, typename T2, typename T3>
T1 add(T2 x, T3 y)
{
    return x + y;
}
/*
template<typename T2>
string add(string x, T2 y)
{
	std::cout << "call add(string, T2)" <<endl;
    return x;
}
// 全特化
template<>
double add(double x, double y)
{
	std::cout << "call add(double, double)" <<endl;
    return x + y;
}*/

inline void run(){
	std::cout<< "Test05::run()" <<endl;
	//add("3", 5);
	//add(99.8f, 0.2f);
}
}


/**
 * 5 偏特化
 */
namespace Test06 {

void printX(){
}
template <typename T, typename... Types>
void printX(const T& head, const Types&... args)
{
     std::cout << head << ", "<< sizeof...(args) << std::endl;
     printX(args...);
}

// Induction
template <int N>
struct Factorial {
  static const int value = N * Factorial<N - 1>::value;
};
// Base case via template specialization:
template <>
struct Factorial<0> {
  static const int value = 1;
};

inline void run() {
	std::cout<< "printX() "<<std::endl;
	printX(1, 200.10, "sunquan" );
	std::cout<< "Calculate factorial is "<<  Factorial<3>::value << std::endl;
}
}

namespace Test07 {

template<typename ... Values> class tup;
// 偏特化(最终)
template<> class tup<> {
};
// 偏特化(递归)
template<typename Head, typename ... Tail>
class tup<Head, Tail...>: private tup<Tail...> {
	typedef tup<Tail...> inherited;
public:
	tup() {
	}
	tup(Head v, Tail ... vtail) : inherited(vtail...), m_head(v){
	}
	Head head() {
		return m_head;
	}
	inherited& tail() {
		return *this;
	}
protected:
	Head m_head;
};

inline void run() {
	string ww = "nico";
	tup<int, float, string> it1(41, 6.3, "nico");
	cout << "sizeof(string) " << sizeof(ww) << endl;
	// sizeof(tup) = 32  (4+4+24) + align(空的class = 1)
	cout << "sizeof(tup) " << sizeof(it1) << endl;
	cout << "== show tuple<3> === " << endl;
	cout << it1.head() << endl;
	cout << it1.tail().head() << endl;
	cout << it1.tail().tail().head() << endl;
	//
	tup<string, complex<int>, bitset<16>, double> it2("Ace", complex<int>(3, 8),
			bitset < 16 > (377), 3.14);
	cout << "== show tuple<4> === " << endl;
	cout << it2.head() << endl;
	cout << it2.tail().head() << endl;
	cout << it2.tail().tail().head() << endl;
	cout << it2.tail().tail().tail().head() << endl;
}
}


/**
 * 6 思考：怎么判断一个变量是否为const?
 */
namespace Test09 {
template<typename T>
struct is_const {
	static bool const value = false;
};

template<typename T>
struct is_const<const T> {
	static const bool value = true;
};

inline void run() {
	int a = 10;
	const int b = 10;
    std::cout << "a is const:" << std::boolalpha << is_const<decltype(a)>::value << std::endl;
	std::cout << "b is const:" << std::boolalpha << is_const<decltype(b)>::value << std::endl;
}
}

int main()
{
	Test01::run();
	Test02::run();
	Test03::run();


	Test05::run();
	Test06::run();
	Test07::run();

	Test09::run();
	return 0;
}
