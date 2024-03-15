#ifndef AISD_LOG_H
#define AISD_LOG_H
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

class LOG {
  std::FILE* file;
public:
  LOG() : file(std::fopen("log.log", "wb")) {
    if (!file) {
      std::fprintf(stderr, "File cannot be opened, using stderr instead: \"%s\"\n", std::strerror(errno));
      file = stderr;
    }
  }

  void log(const char* format, ...) {
    va_list arg;

    va_start(arg, format);
    vfprintf(file, format, arg);
    va_end(arg);
  }

  void closeLog() {
    std::fclose(file);
  }
};
#endif // AISD_LOG_H
