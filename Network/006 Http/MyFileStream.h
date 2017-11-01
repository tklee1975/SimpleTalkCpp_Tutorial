#pragma once

#include "my_common.h"

class MyFileStream {
public:
	MyFileStream() = default;
	~MyFileStream() { close(); }

	bool isOpened() const;

	void openRead(const char* filename);
	void close();

	uint64_t fileSize();

	void read(std::vector<char>& outBuf, size_t bytesToRead);

private:

#ifdef _WIN32
	HANDLE _h = INVALID_HANDLE_VALUE;
#else

#endif

};