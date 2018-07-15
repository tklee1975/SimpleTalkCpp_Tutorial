#include "precompiledHeader.h"

class Example3_Result {
public:
	using Lock = std::unique_lock<std::mutex>;

	void init(int64_t start, int64_t count) {
		_start = start;
		_count = count;
		_current = start;
	}

	void add(int64_t v) {
		Lock lock(mutex);

		result.push_back(v);
	}

	int64_t getNext() {
		Lock lock(mutex);

		if (_current >= _start + _count)
			return 0;

		auto ret = _current;
		_current++;
		return ret;
	}

	void finish() {
		Lock lock(mutex);

		std::sort(result.begin(), result.end());
		printf("result count: %lld\n", result.size());
		//for (auto& e : result) {
		//	printf("prime: %lld\n", e);
		//}
	}

private:
	int64_t _start = 0;
	int64_t _count = 0;
	int64_t _current = 0;

	std::mutex mutex;
	std::vector<int64_t> result;
};

class Example3_Thread {
public:
	Example3_Thread() {
		join();
	}

	void join() {
		if (_thread) {
			WaitForSingleObject(_thread, INFINITE);
		}
	}

	static DWORD WINAPI _proc(void* p) {
		reinterpret_cast<Example3_Thread*>(p)->onProc();
		return 0;
	}

	void onProc() {
		printf("Thread %d: start\n", _id);
		SetThreadAffinityMask(_thread, 1LL << _id);

		for (;;) {
			int64_t v = _result->getNext();
			if (v == 0) {
				break;
			}

			if (primeNumber(v)) {
				printf("Thread %d: %lld\n", _id, v);
				_result->add(v);
			}
		}

		printf("Thread %d: ended\n", _id);
	}

	void run(Example3_Result& result, int id) {
		_result = &result;
		_id = id;
		_thread = CreateThread(nullptr, 0, &_proc, this, 0, nullptr);
	}

	Example3_Result* _result = nullptr;
	HANDLE  _thread = nullptr;
	int     _id = 0;
};

void my_example3() {
	printf("\n========== my_example3 ========\n");

	Example3_Result result;
	MyTimer timer;

	Example3_Thread threads[threadCount];

	result.init(primeStart, threadCount * batchSize);

	for (int i = 0; i < threadCount; i++) {
		threads[i].run(result, i);
	}

	for (int64_t i = 0; i < threadCount; i++) {
		threads[i].join();
	}

	printf("done\n");
	timer.print();
	result.finish();
}
