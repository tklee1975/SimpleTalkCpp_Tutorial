#include "MyFileStream.h"
#include "MyStrUtil.h"

#ifdef _WIN32

void MyFileStream::openRead(const char* filename)
{
	close();
	std::wstring filenameW;
	MyStrUtil::utfConvert(filenameW, filename);

	_h = CreateFile(filenameW.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (_h == INVALID_HANDLE_VALUE)
		throw MyError("MyFileStream::openRead");
}

void MyFileStream::close()
{
	if (_h != INVALID_HANDLE_VALUE) {
		CloseHandle(_h);
		_h = INVALID_HANDLE_VALUE;
	}
}

uint64_t MyFileStream::fileSize()
{
	LARGE_INTEGER t;
	if (!GetFileSizeEx(_h, &t)) {
		return 0;
	}

	return (static_cast<uint64_t>(t.HighPart) << 32) | t.LowPart;
}

void MyFileStream::read(std::vector<char>& outBuf, size_t bytesToRead) {
	outBuf.clear();
	if (bytesToRead > std::numeric_limits<DWORD>::max()) {
		throw MyError("bytesToRead is too big");
	}

	outBuf.resize(bytesToRead);

	DWORD bytesRead = 0;
	if (!ReadFile(_h, outBuf.data(), bytesToRead, &bytesRead, nullptr)) {
		throw MyError("MyFileStream::read");
	}

	outBuf.resize(bytesRead);
}

#else

#endif