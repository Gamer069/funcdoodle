#pragma once

#include <iostream>

// define to get around the stupid VS thing where it doesn't see header files
#define FUNCAOV(x)

#ifdef DEBUG
#define FUNCAOV(x)                                                             \
	do {                                                                       \
		if (!(x)) {                                                            \
			std::cout << "ASSERTION FAILED(DEBUG) at line" << __LINE__         \
					  << " in file " << __FILE__ << std::endl;                 \
		}                                                                      \
	} while (0)
#else
#ifdef NDEBUG
#define FUNCAOV(x)                                                             \
	do {                                                                       \
		if (!(x)) {                                                            \
			std::cout << "VERIFICATION FAILED at line" << __LINE__             \
					  << " in file " << __FILE__ << std::endl;                 \
		}                                                                      \
	} while (0)
#endif
#endif

#ifdef DEBUG
#define FUNCDASS(x)                                                            \
	do {                                                                       \
		if (!(x)) {                                                            \
			std::cout << "STRIPPED ASSERTION FAILED at line" << __LINE__       \
					  << " in file " << __FILE__ << std::endl;                 \
		}                                                                      \
	} while (0)
#else
#define FUNCDASS(x)
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

#include "Gui.h"

#define INVERTED_IMCOL(r, g, b) IM_COL32(255 - r, 255 - g, 255 - b, 255)
// VERSION SCHEME -- major, minor, patch, modifier -- minor/&debug ||
// (m)ajor/&debug

#ifdef NDEBUG
#define FUNCVER "v0.0.9a"
#else
#define FUNCVER "v0.0.9ad"
#endif

#define FDPVERMAJOR 0
#define FDPVERMINOR 4
