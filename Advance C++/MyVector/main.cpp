#include "MyVector.h"
#include <conio.h>
#include <stdio.h>

class MyData : public NonCopyable {
public:
	MyData(int t) : test(t) {}

	MyData(MyData && r) 
		: test(r.test)
	{
		r.test = 0;
	}

	void print() const {
		printf("MyData %d\n", test);
	}

private:
	int test = 0;
};

void dump(const MyVector<int> & v) {
	printf("MyVector<int> size = %d, cap = %d\n", v.size(), v.capacity());
	for (int i = 0; i < v.size(); i++) {
		printf(" %d", v[i]);
	}
	printf("\n");
}

void dump(const MyVector<MyData> & v) {
	printf("MyVector<MyData> size = %d, cap = %d\n", v.size(), v.capacity());
	for (int i = 0; i < v.size(); i++) {
		v[i].print();
	}
	printf("\n");
}

void test001() {
	MyVector<int> v;

	for (int i = 0; i < 10; i++) {
		v.append(i + 10);
	}
	dump(v);
}

void test002() {
	MyData t(10);
	MyData a(std::move(t));  // <--- NonCopyable

	MyVector<MyData> v;
	v.append(std::move(t));
	v.append(std::move(a));
	v.append(MyData(99));

	v.emplace_back(std::move(a));
	v.emplace_back(77);

	dump(v);
}

int main() {
	test002();

	printf("\n====== Program Ended =========\n");
	_getch();
	return 0;
}