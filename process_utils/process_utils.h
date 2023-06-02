#include <iostream>
#include <vector>

#include <windows.h>

extern "C" std::vector<std::string> _split(const std::string, const char);

extern "C" void _ltrim(std::string &, const char);
extern "C" void _rtrim(std::string &, const char);
extern "C" void _trim(std::string &, const char);

extern "C" BOOL _start(const std::string, STARTUPINFOW *, PROCESS_INFORMATION *);
extern "C" std::string _execute(const std::string);

// EXPORT
extern "C" int start_by_name(const char *, const char *);
extern "C" int kill_by_name(const char *);
extern "C" int get_pid_by_name(const char *);
