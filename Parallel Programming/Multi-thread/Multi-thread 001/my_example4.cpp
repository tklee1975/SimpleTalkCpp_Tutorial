#include "precompiledHeader.h"

class Example4 {
public:

	template<class T, class MUTEX = std::mutex>
	class LockProtected {
	public:

		class ScopedLock {
		public:
			ScopedLock(MUTEX& mutex, T& data) {
				_mutex = &mutex;
				_data  = &data;
				_mutex->lock();
			}

			ScopedLock(ScopedLock && r) {
				_mutex = r._mutex;
				_data  = r._data;
				r._mutex = nullptr;
				r._data  = nullptr;
			}

			~ScopedLock() {
				if (_mutex ) {
					_mutex->unlock();
					_mutex = nullptr;
				}
			}

			T* operator->() { return _data; }

		private:
			MUTEX*	_mutex = nullptr;
			T*		_data  = nullptr;
		};

		ScopedLock scopedLock() { return ScopedLock(_mutex, _data); }

	private:
		T _data;
		MUTEX _mutex;
	};

	class MyRequest {
	public:
		using Lock = std::unique_lock<std::mutex>;

		void init(int64_t start, int64_t count) {
			auto r = _request.scopedLock();
			r->start = start;
			r->current = start;
			r->count = count;
		}

		int64_t getNext() {
			auto r = _request.scopedLock();
			if (r->current >= r->start + r->count)
				return 0;

			auto ret = r->current;
			r->current++;
			return ret;
		}

		void addResult(int64_t v) {
			auto r = _result.scopedLock();
			r->primeNumbers.push_back(v);
		}

		void finish() {
			auto r = _result.scopedLock();
			printf("result count: %lld\n", r->primeNumbers.size());
		}

	private:
		struct Request {
			int64_t start = 0;
			int64_t count = 0;
			int64_t current = 0;
		};

		struct Result {
			std::vector<int64_t> primeNumbers;
		};

		LockProtected<Request>	_request;
		LockProtected<Result>	_result;
	};

	class MyThread {
	public:
		~MyThread() {
			join();
		}

		void join() {
			if (_thread) {
				WaitForSingleObject(_thread, INFINITE);
			}
		}

		static DWORD WINAPI _proc(void* p) {
			reinterpret_cast<MyThread*>(p)->onProc();
			return 0;
		}

		void onProc() {
			printf("Thread %d: start\n", _id);
			SetThreadAffinityMask(_thread, 1LL << _id);

			for (;;) {
				int64_t v = _req->getNext();
				if (v == 0) {
					break;
				}

				if (primeNumber(v)) {
					printf("Thread %d: prime %lld\n", _id, v);
					_req->addResult(v);
				}
			}

			printf("Thread %d: ended\n", _id);
		}

		void run(MyRequest& req, int id) {
			_req = &req;
			_id = id;
			_thread = CreateThread(nullptr, 0, &_proc, this, 0, nullptr);
		}

		MyRequest* _req = nullptr;
		HANDLE  _thread = nullptr;
		int     _id = 0;
	};

	static void run() {
		printf("\n========== my_example4 ========\n");

		MyRequest req;
		MyThread threads[threadCount];

		MyTimer timer;
		req.init(primeStart, threadCount * batchSize);

		for (int i = 0; i < threadCount; i++) {
			threads[i].run(req, i);
		}

		for (int64_t i = 0; i < threadCount; i++) {
			threads[i].join();
		}

		printf("done\n");
		timer.print();
		req.finish();
	}

}; // Example4

void my_example4() {
	Example4::run();
}
