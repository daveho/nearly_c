#include <cstdarg>
#include <cstdio>
#include "cpputil.h"

namespace {
const unsigned MAX_STRBUF_SIZE = 1024;
}

std::string cpputil::format(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  std::string s = vformat(fmt, args);
  va_end(args);

  return s;
}

std::string cpputil::vformat(const char *fmt, va_list args) {
  char buf[MAX_STRBUF_SIZE];

  vsnprintf(buf, MAX_STRBUF_SIZE, fmt, args);
  buf[MAX_STRBUF_SIZE-1] = '\0';

  return std::string(buf);
}
