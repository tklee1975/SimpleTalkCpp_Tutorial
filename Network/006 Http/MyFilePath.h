#include "MyStrUtil.h"

namespace MyFilePath {

void currentDir(std::string& outPath);
void getAbsPath(std::string& outPath, const char* inPath);

void toUnixPath(wchar_t* str);

}