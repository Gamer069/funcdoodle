#pragma once

#include <iostream>

#ifdef DEBUG
#define FUNC_AOV(x)                                                            \
	do {                                                                       \
		if (!(x)) {                                                            \
			std::cout << "ASSERTION FAILED(DEBUG) at line" << __LINE__         \
					  << " in file " << __FILE__ << std::endl;                 \
		}                                                                      \
	} while (0)
#else
#ifdef NDEBUG
#define FUNC_AOV(x)                                                            \
	do {                                                                       \
		if (!(x)) {                                                            \
			std::cout << "VERIFICATION FAILED at line" << __LINE__             \
					  << " in file " << __FILE__ << std::endl;                 \
		}                                                                      \
	} while (0)
#endif
#endif

#ifdef DEBUG
#define FUNC_AOV_EX(x, str)                                                    \
	do {                                                                       \
		if (!(x)) {                                                            \
			std::cout << "FUNC_AOV_EX failed with this message: "              \
					  << std::endl;                                            \
			FUNC_INF(str);                                                     \
		}                                                                      \
	} while (0)
#else
#ifdef NDEBUG
#define FUNC_AOV_EX(x, str)                                                    \
	do {                                                                       \
		if (!(x)) {                                                            \
			FUNC_FATAL("VERIFICATION FAILED, description:");                   \
			FUNC_INF(str);                                                     \
		}                                                                      \
	} while (0)
#endif
#endif

#ifdef DEBUG
#define FUNC_DASS(x)                                                           \
	do {                                                                       \
		if (!(x)) {                                                            \
			std::cout << "STRIPPED ASSERTION FAILED at line" << __LINE__       \
					  << " in file " << __FILE__ << std::endl;                 \
		}                                                                      \
	} while (0)
#else
#define FUNC_DASS(x)
#endif

#ifdef DEBUG
#define FUNC_DBG(x)                                                            \
	do {                                                                       \
		std::cout << "\033[36m[Debug]: " << x << "\033[0m" << std::endl;       \
	} while (0)
#else
#define FUNC_DBG(x)
#endif

#define FUNC_INF(x)                                                            \
	do {                                                                       \
		std::cout << "\033[34m[Info]: " << x << "\033[0m" << std::endl;        \
	} while (0)

#define FUNC_WARN(x)                                                           \
	do {                                                                       \
		std::cout << "\033[33m[Warn]: " << x << "\033[0m" << std::endl;        \
	} while (0)

#define FUNC_GRAY(x)                                                           \
	do {                                                                       \
		std::cout << "\033[90m[Note]: " << x << "\033[0m" << std::endl;        \
	} while (0)

#define FUNC_ERR(x)                                                            \
	do {                                                                       \
		std::cout << "\033[1;35m[Error]: " << x << "\033[0m" << std::endl;     \
	} while (0)

#define FUNC_FATAL(x)                                                          \
	do {                                                                       \
		std::cout << "\033[1;31m[FATAL]: " << x << "\033[0m" << std::endl;     \
		std::exit(-1);                                                         \
	} while (0)

#include "Gui.h"

#define INVERTED_IMCOL(r, g, b) IM_COL32(255 - r, 255 - g, 255 - b, 255)

#ifdef _WIN32
#include <windows.h>
#define OPEN_FILE_EXPLORER(path)                                               \
	ShellExecuteA(nullptr, "explore", path.c_str(), nullptr, nullptr, SW_SHOW)
#elif __APPLE__
#include <cstdlib>
#define OPEN_FILE_EXPLORER(path)                                               \
	std::system(("open " + path.string() + " &").c_str())
#elif __linux__
#include <cstdlib>
#define OPEN_FILE_EXPLORER(path)                                               \
	std::system(("xdg-open " + path.string() + " &").c_str())
#else
#define OPEN_FILE_EXPLORER(path)                                               \
	std::cerr << "Unsupported platform!" << std::endl
#endif

// VERSION SCHEME -- major, minor, patch, modifier -- minor/&debug ||
// (m)ajor/&debug

#ifdef NDEBUG
#define FUNCVER "v0.1.0a"
#else
#define FUNCVER "v0.1.0a"
#endif

#define FDPVERMAJOR 0
#define FDPVERMINOR 4
