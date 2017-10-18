#pragma once

#include "my_common.h"

#ifdef MYDB_EXPORTS
	#define MyDB_API __declspec(dllexport)
#else
	#define MyDB_API __declspec(dllimport)
	#pragma comment(lib, "MyDB.lib")
#endif

namespace MyDB {

// connection interface
class Conn : public MyNonCopyable {
public:
	virtual ~Conn() {}

	virtual void directExec(const char* sql) = 0;
};

MyDB_API Conn* connectMySQL(const char* host, const char* db, const char* user, const char* password);
MyDB_API Conn* connectSQLite3();

}