#include "precompiledHeader.h"

class Example5 {
public:

	template<class T, class Mutex = std::mutex, class CondVar = std::condition_variable>
	class LockProtected {
	public:

		class ScopedLock {
		public:
			using Lock = std::unique_lock<std::mutex>;

			ScopedLock(T& data, Mutex& mutex, CondVar& condvar) 
				: _lock(mutex)
			{
				_data  = &data;
				_condvar = &condvar;
			}

			ScopedLock(ScopedLock && r) 
				: _lock(std::move(r._lock))
			{
				_condvar = r._condvar;
				_data  = r._data;
				r._condvar = nullptr;
				r._data  = nullptr;
			}

			T* operator->() { return _data; }

			void wait() {
				if (_condvar) {
					_condvar->wait(_lock);
				}
			}

			void notify_one() {
				if (_condvar) {
					_condvar->notify_one();
				}
			}

			void notify_all() {
				if (_condvar) {
					_condvar->notify_all();
				}
			}

		private:
			T*			_data  = nullptr;
			Lock		_lock;
			CondVar*	_condvar = nullptr;
		};

		ScopedLock scopedLock() { return ScopedLock(_data, _mutex, _condvar); }

	private:
		T _data;
		Mutex _mutex;
		CondVar _condvar;
	};

	class MyRequest {
	public:
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
			r.notify_all();
		}

		void finish() {
			auto r = _result.scopedLock();
			printf("result count: %lld\n", r->primeNumbers.size());
		}

		void threadEnded() {
			auto r = _result.scopedLock();
			r->endedThread++;
			r.notify_all();
		}

		bool isDone() {
			auto r = _result.scopedLock();
			printf("===> result count: %lld\n", r->primeNumbers.size());
			if (r->endedThread >= kThreadCount) {
				return true;
			}
			r.wait();
			return false;
		}

	private:
		struct Request {
			int64_t start = 0;
			int64_t count = 0;
			int64_t current = 0;
		};

		struct Result {
			std::vector<int64_t> primeNumbers;
			int endedThread = 0;
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
			_req->threadEnded();
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
		MyThread threads[kThreadCount];

		MyTimer timer;
		req.init(kPrimeStart, kThreadCount * kBatchSize);

		for (int i = 0; i < kThreadCount; i++) {
			threads[i].run(req, i);
		}

		while (!req.isDone()) {
		}

		printf("done\n");
		timer.print();
		req.finish();
	}

}; // Example5

void my_example5() {
	Example5::run();
}
