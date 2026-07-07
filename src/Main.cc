#include "Conf/FuncDoodle.h"
#include "Core/App.h"

#include <cstring>

int main(int argc, char** argv) {
	for (int i = 0; i < argc; i++) {
		if (std::strcmp(argv[i], "--no-limits") == 0) {
			FUNC_INF("no limits");
			FuncDoodle::g_NoWindowLimits = true;
		}
	}

	FuncDoodle::Application app;

	app.Run();

	// imo explicit return 0s in main are better than implicit ones :)
	return 0;
}
