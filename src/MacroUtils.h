#pragma once

#include <iostream>

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
// VERSION SCHEME -- major, minor, patch, modifier -- minor/&debug ||
// (m)ajor/&debug

#ifdef NDEBUG
#define FUNCVER "v0.0.6m"
#else
#define FUNCVER "v0.0.6md"
#endif

#define FDPVERMAJOR 0
#define FDPVERMINOR 3
