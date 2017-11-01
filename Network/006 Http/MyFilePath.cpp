#include "MyFilePath.h"

namespace MyFilePath {

void currentDir(std::string& outPath) {
	wchar_t tmp[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, tmp);
	tmp[MAX_PATH] = 0;
	toUnixPath(tmp);
	MyStrUtil::utfConvert(outPath, tmp);
}

void getAbsPath(std::string& outPath, const char* inPath) {
	std::wstring inPathW;
	MyStrUtil::utfConvert(inPathW, inPath);

	wchar_t tmp[MAX_PATH + 1];
	GetFullPathName(inPathW.c_str(), MAX_PATH, tmp, nullptr);
	tmp[MAX_PATH] = 0;
	toUnixPath(tmp);
	MyStrUtil::utfConvert(outPath, tmp);
}

void toUnixPath(wchar_t* str)
{
	for (auto* p = str; *p ; p++) {
		if (*p == '\\')
			*p = '/';
	}
}

} //namespace