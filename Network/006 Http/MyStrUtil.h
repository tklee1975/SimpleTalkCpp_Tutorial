#include "my_common.h"

namespace MyStrUtil {

void toUpper(std::string& s);

const char* getToken(std::string& token, const char* input, char sep);
const char* getUpperToken(std::string& token, const char* input, char sep);

const char* getLine(std::string& outLine, const char* input);

void utfConvert(std::string&  o, const wchar_t* s);
void utfConvert(std::wstring& o, const char*    s);

bool startsWith(const char* s, const char* prefix);

}