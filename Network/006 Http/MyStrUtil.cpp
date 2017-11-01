#include "MyStrUtil.h"

namespace MyStrUtil {

const char* getToken(std::string& token, const char* input, char sep) {
	token.clear();

	auto* p = input;

	//trim spaces
	for (;*p; p++) {
		if (*p != ' ' && *p != '\t')
			break;
	}

	auto* start = p;
		
	// find sep
	for(; *p; p++) {
		if (sep == *p) {
			break;
		}
	}

	token.assign(start, p);

	printf("token = [%s]\n", token.c_str());
	return p;
}

const char* getUpperToken(std::string& token, const char* input, char sep) {
	auto ret = getToken(token, input, sep);
	toUpper(token);
	return ret;
}

const char* getLine(std::string& outLine, const char* input) {
	outLine.clear();
	auto* e = strstr(input, "\r\n");
	if (!e) {
		outLine = input;
		return input + strlen(input);
	}
	outLine.assign(input, e);
	return e + 2;
}

void utfConvert(std::string& o, const wchar_t* s)
{
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> co;
	o = co.to_bytes(s);
}

void utfConvert(std::wstring& o, const char* s)
{
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> co;
	o = co.from_bytes(s);
}

bool startsWith(const char* s, const char* prefix)
{
	int n = strlen(prefix);
	return 0 == strncmp(s, prefix, n);
}

void toUpper(std::string& s) {
	for (auto& c : s) {
		c = toupper(c);
	}
}

}