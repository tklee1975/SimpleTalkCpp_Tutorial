#include "stdafx.h"

const int N = 1000;

DWORD WINAPI my_example1_thread_func(void* param) {
	for (int i = 0; i < N * 2; i++) {
		printf("+");
	}

	return 0;
}

void my_example1() {
	HANDLE my_thread = CreateThread(nullptr, 0, &my_example1_thread_func, nullptr, 0, nullptr);
	for (int i = 0; i < N; i++) {
		printf(".");
	}

	WaitForSingleObject(my_thread, INFINITE);
}