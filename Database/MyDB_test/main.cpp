#include "../MyDB/MyDB.h"
#include <conio.h>

int main() {

	std::unique_ptr<MyDB::Conn> conn(MyDB::connectMySQL("localhost", "test_db", "test_user", "1234"));

	conn->directExec("INSERT INTO `test_db`.`test_table` (`name`, `count`, `weight`) VALUES ('cccc', '300', '5.12');");

	printf("\n==== Program Ended ======\n");
	_getch();

	return 0;
}