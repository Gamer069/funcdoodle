#pragma once

#ifdef DEBUG
#define FUNCAOV(x) \
do { \
    if (!(x)) {\
        std::cout << "ASSERTION FAILED(DEBUG) at line" << __LINE__ << " in file " << __FILE__ << std::endl; \
    }\
} while (0)
#else
#ifdef NDEBUG
#define FUNCAOV(x) \
do { \
    if (!(x)) {\
        std::cout << "VERIFICATION FAILED at line" << __LINE__ << " in file " << __FILE__ << std::endl; \
    }\
} while (0)
#endif
#endif

#ifdef DEBUG
#define FUNCDASS(x) \
do { \
    if (!(x)) {\
        std::cout << "STRIPPED ASSERTION FAILED at line" << __LINE__ << " in file " << __FILE__ << std::endl; \
    }\
} while (0)
#else
#define FUNCDASS(x)
#endif

// VERSION SCHEME -- major, minor, patch, modifier -- minor/&debug || (m)ajor/&debug

#ifdef NDEBUG
#define FUNCVER "v0.0.3m"
#else
#define FUNCVER "v0.0.3md"
#endif