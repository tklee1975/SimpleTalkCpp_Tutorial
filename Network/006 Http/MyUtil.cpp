#include "MyUtil.h"

const char* MyUtil::getStringToken(std::string& token, const char* input, char separator) {
	token.clear();

	auto* p = input;

	//trim spaces
	for (;*p; p++) {
		if (*p != ' ' && *p != '\t')
			break;
	}

	auto* start = p;
		
	// find separator
	for(; *p; p++) {
		if (separator == *p) {
			break;
		}
	}

	token.assign(start, p);

	//printf("token = [%s]\n", token.c_str());
	return p;
}

const char* MyUtil::getUpperStringToken(std::string& token, const char* input, char separator) {
	auto ret = getStringToken(token, input, separator);
	toUpperString(token);
	return ret;
}

const char* MyUtil::getLine(std::string& outLine, const char* input) {
	outLine.clear();
	auto* e = strstr(input, "\r\n");
	if (!e) {
		outLine = input;
		return input + strlen(input);
	}
	outLine.assign(input, e);
	return e + 2;
}

void MyUtil::utfConvert(std::string& o, const wchar_t* s)
{
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> co;
	o = co.to_bytes(s);
}

void MyUtil::utfConvert(std::wstring& o, const char* s)
{
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> co;
	o = co.from_bytes(s);
}

bool MyUtil::stringStartsWith(const char* s, const char* prefix)
{
	int n = strlen(prefix);
	return 0 == strncmp(s, prefix, n);
}

void MyUtil::stringAppendNumber(std::string& o, int v)
{
	char tmp[64 + 1];
	snprintf(tmp, 64, "%d", v);
	tmp[64] = 0;
	o.append(tmp);
}

void MyUtil::toUpperString(std::string& s) {
	for (auto& c : s) {
		c = toupper(c);
	}
}

void MyUtil::getCurrentDir(std::string& outPath) {
	wchar_t tmp[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, tmp);
	tmp[MAX_PATH] = 0;
	utfConvert(outPath, tmp);
	toUnixPath(outPath);
}

void MyUtil::getAbsPath(std::string& outPath, const char* inPath) {
	std::wstring inPathW;
	utfConvert(inPathW, inPath);

	wchar_t tmp[MAX_PATH + 1];
	GetFullPathName(inPathW.c_str(), MAX_PATH, tmp, nullptr);
	tmp[MAX_PATH] = 0;
	utfConvert(outPath, tmp);
	toUnixPath(outPath);
}

void MyUtil::toUnixPath(std::string& s)
{
	for (auto& c : s) {
		if (c == '\\')
			c = '/';
	}
}

