#include "my_common.h"

class Example1 {
public:
	using Lock = std::unique_lock<std::mutex>;

	static std::string getThreadId() {
		std::stringstream ss;
		ss << std::this_thread::get_id();
		return ss.str();
	}

	static void my_sleep() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	class BankAccount {
	public:
		void transfer(BankAccount& to, int amount) {
			doTransfer_DeadLock(to, amount);
			//doTransfer_LockInOrder(to, amount);
			//doTransfer_TryLock(to, amount);
		}

		void doTransfer_DeadLock(BankAccount& to, int amount) {
			auto threadId = getThreadId();

			printf("Thread %s: start transfer %p -> %p\n", threadId.c_str(), this, &to);

			printf("Thread %s: lock %p\n", threadId.c_str(), this);
			Lock lockFrom(_mutex);

			my_sleep();

			printf("Thread %s: lock %p\n", threadId.c_str(), &to);
			Lock lockTo(to._mutex);

			my_sleep();
			_balance -= amount;
			to._balance += amount;

			printf("Thread %s: end transfer %p -> %p\n", threadId.c_str(), this, &to);
		}

		void doTransfer_LockInOrder(BankAccount& to, int amount) {
			auto threadId = getThreadId();

			printf("Thread %s: start transfer %p -> %p\n", threadId.c_str(), this, &to);

			Lock lockFrom(_mutex, std::defer_lock);
			my_sleep();
			Lock lockTo(to._mutex, std::defer_lock);

			if (this < &to) {
				printf("Thread %s: lock %p\n", threadId.c_str(), this);
				lockFrom.lock(); // <--- lockA first

				my_sleep();

				printf("Thread %s: lock %p\n", threadId.c_str(), &to);
				lockTo.lock();

			} else {
				printf("Thread %s: lock %p\n", threadId.c_str(), &to);
				lockTo.lock(); // <-- lockB first

				my_sleep();

				printf("Thread %s: lock %p\n", threadId.c_str(), this);
				lockFrom.lock();
			}

			my_sleep();
			_balance -= amount;
			to._balance += amount;

			printf("Thread %s: end transfer %p -> %p\n", threadId.c_str(), this, &to);
		}

		void doTransfer_TryLock(BankAccount& to, int amount) {
			auto threadId = getThreadId();

			printf("Thread %s: start transfer %p -> %p\n", threadId.c_str(), this, &to);

			for(;;) {
				printf("Thread %s: lock %p\n", threadId.c_str(), this);
				Lock lockFrom(_mutex);

				my_sleep();

				printf("Thread %s: try lock %p\n", threadId.c_str(), this);
				Lock lockTo(to._mutex, std::try_to_lock);

				if (!lockTo.owns_lock()) {
					printf("Thread %s: unlock %p\n", threadId.c_str(), this);
					lockFrom.unlock(); // <--- unlock before sleep
					my_sleep();
					continue;
				}

				my_sleep();
				_balance -= amount;
				to._balance += amount;

				printf("Thread %s: end transfer %p -> %p\n", threadId.c_str(), this, &to);
				break;
			}
		}

		int balance() {
			Lock lock(_mutex);
			return _balance;
		}

	private:
		std::mutex _mutex;
		int _balance = 1000;
	};

	void run() {
		{
			auto threadId = getThreadId();
			printf("main thread %s\n", threadId.c_str());
		}

		std::thread thread0([this](){
			auto threadId = getThreadId();
			printf("thread0 %s\n", threadId.c_str());
			accountA.transfer(accountB, 10);
		});

		accountB.transfer(accountA, 1);

		thread0.join();
	}

	BankAccount accountA;
	BankAccount accountB;
};

void my_example1() {
	Example1 ex;
	ex.run();
}