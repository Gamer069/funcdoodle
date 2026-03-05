#include "exepath.h"
#include "Test.h"
#include <cstring>

int FuncDoodle_RunExepathTests() {
	TEST_SCOPE("Exepath Tests");

	const char* path = exepath::get();
	CHECK((path != nullptr), "get() should return non-null");
	CHECK((strlen(path) > 0), "get() should return non-empty string");
	CHECK((strstr(path, "/") != nullptr || strstr(path, "\\") != nullptr),
		"path should contain directory separator");

	return 0;
}
