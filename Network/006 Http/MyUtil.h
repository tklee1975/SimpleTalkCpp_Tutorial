#pragma once

#include "my_common.h"

struct MyUtil {

// string 
static void toUpperString(std::string& s);
static void utfConvert(std::string&  o, const wchar_t* s);
static void utfConvert(std::wstring& o, const char*    s);

static const char* getStringToken(std::string& token, const char* input, char separator);
static const char* getUpperStringToken(std::string& token, const char* input, char separator);
static const char* getLine(std::string& outLine, const char* input);

static bool stringStartsWith(const char* s, const char* prefix);

static void stringAppendNumber(std::string& o, int v);

// path
static void getCurrentDir(std::string& outPath);
static void getAbsPath(std::string& outPath, const char* inPath);

static void toUnixPath(std::string& s);

};