#include "MyDB.h"

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")


namespace MyDB {

class MySQL_Conn : public Conn {
public:
	MySQL_Conn(const char* host, const char* db, const char* user, const char* password);
	~MySQL_Conn();

	virtual void directExec(const char* sql);

private:
	MYSQL* _conn = nullptr;
};

MySQL_Conn::MySQL_Conn(const char* host, const char* db, const char* user, const char* password) {
	_conn = mysql_init(nullptr);
	if (!_conn)
		throw MyError("mysql_init");

	if (nullptr == mysql_real_connect(_conn, host, user, password, db, 0, nullptr, 0)) {
		throw MyError(mysql_error(_conn));
	}
}

MySQL_Conn::~MySQL_Conn() {
	if (_conn) {
		mysql_close(_conn);
	}
}

void MySQL_Conn::directExec(const char* sql) {
	if (!_conn) 
		throw MyError("directExec _conn is null");

	if (mysql_query(_conn, sql)) {
		throw MyError("mysql_query");
	}
}

Conn* connectMySQL(const char* host, const char* db, const char* user, const char* password) {
	return new MySQL_Conn(host, db, user, password);
}

}