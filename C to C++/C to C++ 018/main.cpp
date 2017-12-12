#include <stdio.h>
#include <memory>
#include <ostream>
#include <iostream>
#include <functional>
#include <vector>

#ifdef _MSC_VER
	#include <conio.h>
#endif

#define my_dumpvar(E) std::cout << #E << " = " << E << "\n";
#define run_test(E) std::cout << "\n==== " << #E << " ====\n"; E;

class Point {
public:
	Point() = default;

	explicit Point(int x_, int y_) 
	: x(x_)
	, y(y_)
	{
		std::cout << "call Point(int, int)\n";
	}

	Point(std::initializer_list<int> list) {
		std::cout << "call Point(intitializer_list)\n";

		for (auto& v : list) {
			x += v;
		}
	}

	// delegation constructor
	Point(int s) : Point(s,s) {
		std::cout << "call Point(int)\n";
	}

	int x = 0;
	int y = 0;
};

class Point3D : public Point {
public:
	int z;
};

static_assert(sizeof(Point) == sizeof(int) * 2, "some error message");
static_assert(std::is_same<decltype(Point::x), int>::value, "Point x must be int");
static_assert(std::is_base_of<Point, Point3D>::value, "Point3D must based on Point");

std::ostream& operator << (std::ostream& s, const Point& v) {
	s << "Point(" << v.x << ", " << v.y << ")";
	return s;
}

class UniquePtr {
public:
	UniquePtr() = default;
	~UniquePtr() { destroy(); }

	UniquePtr(Point* p)
		: m_p(p) 
	{
		std::cout << "call UniquePtr(Point*)\n";
	}

	UniquePtr(nullptr_t) {
		std::cout << "call UniquePtr(nullptr_t)\n";
	}

	UniquePtr(int v) {
		std::cout << "call UniquePtr(int)\n";
	}

	UniquePtr(const UniquePtr&) = delete;

	// move constructor
	UniquePtr(UniquePtr && r) {
		operator=(std::move(r));
	}

	void operator=(const UniquePtr&) = delete;
	// move
	void operator=(UniquePtr && r) {
		m_p = r.m_p;
		r.m_p = nullptr;
	}

	void create(int x, int y) {
		destroy();
		m_p = new Point(x, y);
	}
	
	void destroy() {
		if (m_p) {
			delete m_p;
			m_p = nullptr;
		}
	}

		  Point* getPtr()		{ return m_p; }
	const Point* getPtr() const { return m_p; }

private:
	Point* m_p = nullptr;
};

std::ostream& operator << (std::ostream& s, const UniquePtr& v) {
	auto* p = v.getPtr();
	if (!p) {
		s << "(null)";
	} else {
		s << *p;
	}
	return s;
}

void test_move() {
	UniquePtr a;
	a.create(3,4);

	UniquePtr b;
	b = std::move(a);

	my_dumpvar(a);
	my_dumpvar(b);
}

constexpr int div2(int m) { 
	return m / 2;
}

void test_constexp() {
	char a[div2(4)];
	my_dumpvar(sizeof(a));
}

void test_range_based_for_loop() {
	std::vector<int> v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i);
	}

	// C++ 03
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";

	// auto
	for (auto& it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";

	// range based for loop
	for (const auto& e : v) {
		std::cout << e << " ";
	}
	std::cout << "\n";
}

void test_initializer_list() {
	Point a = {1,2,3,4};
	my_dumpvar(a);
}

void call_lambda(std::function<int (int)> f) {
	for (int i = 0; i < 4; i++) {
		auto ret = f(i);
		std::cout << "call func i=" << i << ", ret=" << ret << "\n";
	}
}

void test_lambda() {
	int sum = 0;
	call_lambda([&sum](int i) -> int {
		sum += i;
		return i * 2;
	});

	my_dumpvar(sum);
}

void test_delegation_constructor() {
	Point p(3);
	my_dumpvar(p);
}

void test_nullptr_t() {
	UniquePtr a(NULL);          // <-- call UniquePtr(int)  !!! unexpected
	UniquePtr b(new Point());   // <-- call UniquePtr(Point*)
	UniquePtr c(nullptr);       // <-- call UniquePtr(nullptr_t)
}

enum class TestEnum : int8_t {
	A,
	B,
	C,
};

void test_enum_class() {
	TestEnum e = TestEnum::B;
	my_dumpvar(sizeof(e));

	my_dumpvar((std::is_same<TestEnum, int8_t>::value));

	typedef std::underlying_type<TestEnum>::type Under;
	my_dumpvar((std::is_same<Under,    int8_t>::value));
}

void test_explicit_constructor() {
//	Point a(1.1, 2.2); // not allowed for explicit constructor
}

template<typename FIRST, typename... ARGS>
void my_print_impl(const FIRST& first, const ARGS&... args) {
	std::cout << first << " ";
	my_print_impl(args...);
}

void my_print_impl() {
}

template<typename... ARGS>
void my_print(const ARGS&... args) {
	my_dumpvar(sizeof...(args));
	my_print_impl(args...);
}

void test_variadic_templates() {
	my_print (1, 2, 3, "a", "b");
}

// no template typedef !!!!
//template<typename T>
//typedef std::vector< std::unique_ptr<T> > UPtrVector;

template<typename T>
using UPtrVector = std::vector< std::unique_ptr<T> >;

void test_template_using() {
	UPtrVector<int> a;
}

int main(int argv, const char* argc[]) {
	run_test(test_move());
	run_test(test_constexp());
	run_test(test_range_based_for_loop());
	run_test(test_initializer_list());
	run_test(test_lambda());
	run_test(test_delegation_constructor());
	run_test(test_nullptr_t());
	run_test(test_enum_class());
	run_test(test_variadic_templates());
	run_test(test_template_using());

	std::cout << "\n==== Ended ====\n";
#ifdef _MSC_VER
	_getch();
#endif
	return 0;
}