#ifndef CPPUTIL_H
#define CPPUTIL_H

// Utility functions for C++ code

#include <string>

namespace cpputil {

std::string format(const char *fmt, ...)
#ifdef __GNUC__
    __attribute__ ((format (printf, 1, 2)))
#endif
    ;

std::string vformat(const char *fmt, va_list args);

}

#endif // CPPUTIL_H
