#include "precompiledHeader.h"

void my_example1();
void my_example2();
void my_example3();

int main() {
	my_example1();
	my_example2();
	my_example3();

	printf("\n======= Program Ended ===========\n");
	printf("Press any key to exit\n");
	_getch();
    return 0;
}