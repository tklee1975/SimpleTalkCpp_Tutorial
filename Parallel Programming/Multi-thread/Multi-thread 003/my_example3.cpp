#include "my_common.h"

class MyThreadLocalInt {
public:

	operator int() { return get(); }

	void operator+=(int v) {
		get() += v;
	}

private:
	int& get() {
		return _map[myGetThreadId()];
	}

	std::map<DWORD, int> _map;
};

class MyTlsInt {
public:
	MyTlsInt() {
		_tls = TlsAlloc();
	}

	~MyTlsInt() {
		release();
	}

	operator int() { return get(); }

	void operator+=(int v) {
		get() += v;
	}

	void release() {	
		if (_tls == TLS_OUT_OF_INDEXES)
			return;

		if (auto* p = TlsGetValue(_tls)) {
			delete p;
			TlsSetValue(_tls, nullptr);
		}
		TlsFree(_tls);
		_tls = TLS_OUT_OF_INDEXES;
	}

private:
	int& get() {
		auto* p = reinterpret_cast<int*>(TlsGetValue(_tls));
		if (!p) {
			p = new int();
			TlsSetValue(_tls, p);
		}
		return *p;
	}

	DWORD _tls = TLS_OUT_OF_INDEXES;
};

//int g_value = 0;
//MyThreadLocalInt g_value;
//MyTlsInt g_value;
thread_local int g_value = 0;

class Example3 {
public:
	static const int N = 10;

	std::mutex mutex;
	using Lock = std::lock_guard<std::mutex>;

	int add(int v) {
		Lock lock(mutex);

		g_value += v;
		Sleep(200);
		return g_value;
	}

	void run() {
		std::thread thread0([this](){
			for (int i = 0; i < N; i++) {
				printf("Thread %08X: a = %d\n", myGetThreadId(), add(1));
			}
		});

		for (int i = 0; i < N; i++) {
			printf("Thread %08X: a = %d\n", myGetThreadId(), add(-1));
		}

		thread0.join();
	}
};

void my_example3() {
	Example3 ex;
	ex.run();
}