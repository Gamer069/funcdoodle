#include "Frame.h"
#include "Test.h"

int FuncDoodle_RunColTests() {
	TEST_SCOPE("Col Tests");

	FuncDoodle::Col col1{255, 0, 0};
	FuncDoodle::Col col2{0, 255, 0};
	FuncDoodle::Col col1_copy{255, 0, 0};

	CHECK((col1 == col1_copy), "same values should be equal");
	CHECK((col1 != col2), "different values should not be equal");
	CHECK((!(col1 == col2)), "different values should not be equal");

	CHECK((col1.r == 255), "r channel should be 255");
	CHECK((col1.g == 0), "g channel should be 0");
	CHECK((col1.b == 0), "b channel should be 0");

	FuncDoodle::Col col_red{255, 0, 0};
	FuncDoodle::Col col_green{0, 255, 0};
	FuncDoodle::Col col_blue{0, 0, 255};
	FuncDoodle::Col col_cyan{0, 255, 255};
	FuncDoodle::Col col_white{255, 255, 255};
	FuncDoodle::Col col_black{0, 0, 0};

	CHECK((col_black < col_red), "black < red");
	CHECK((col_red < col_white), "red < white");
	CHECK((col_cyan < col_white), "cyan < white");

	CHECK((!(col_red < col_black)), "red < black is false");
	CHECK((col_green < col_cyan), "green < cyan");

	FuncDoodle::Col default_col;
	CHECK((default_col.r == 255), "default r should be 255");
	CHECK((default_col.g == 255), "default g should be 255");
	CHECK((default_col.b == 255), "default b should be 255");

	FuncDoodle::Col col_test{128, 64, 32};
	CHECK((col_test.r == 128), "r should be 128");
	CHECK((col_test.g == 64), "g should be 64");
	CHECK((col_test.b == 32), "b should be 32");

	col1 = col2;
	CHECK((col1 == col2), "assignment should work");

	return 0;
}
