#include "precompiledHeader.h"

class Example2_Result {
public:
	using Lock = std::unique_lock<std::mutex>;

	void add(int64_t v) {
		Lock lock(mutex);

		result.push_back(v);
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
	std::mutex mutex;
	std::vector<int64_t> result;
};

class Example2_Thread {
public:
	Example2_Thread() {
		join();
	}

	void join() {
		if (_thread) {
			WaitForSingleObject(_thread, INFINITE);
		}
	}

	static DWORD WINAPI _proc(void* p) {
		reinterpret_cast<Example2_Thread*>(p)->onProc();
		return 0;
	}

	void onProc() {
		printf("Thread %d: start %lld %lld\n", _id, _start, _batch);
		SetThreadAffinityMask(_thread, 1LL << _id);

		for (int64_t i = 0; i < _batch; i++) {
			int64_t v = _start + i;
			if (primeNumber(v)) {
				printf("Thread %d: %lld\n", _id, v);
				_result->add(v);
			}
		}

		printf("Thread %d: ended\n", _id);
	}

	void run(Example2_Result& result, int id, int64_t start, int64_t batch) {
		_result = &result;
		_id = id;
		_start = start;
		_batch = batch;
		_thread = CreateThread(nullptr, 0, &_proc, this, 0, nullptr);
	}

	Example2_Result* _result = nullptr;
	HANDLE  _thread = nullptr;
	int     _id = 0;
	int64_t _start = 0;
	int64_t _batch = 0;
};

void my_example2() {
	printf("\n========== my_example2 ========\n");

	Example2_Result result;
	MyTimer timer;

	Example2_Thread threads[threadCount];

	for (int i = 0; i < threadCount; i++) {
		int64_t start = primeStart + i * batchSize;
		threads[i].run(result, i, start, batchSize);
	}

	for (int64_t i = 0; i < threadCount; i++) {
		threads[i].join();
	}

	printf("done\n");
	timer.print();
	result.finish();
}
