#include <iostream>
#include <vector>

extern "C" std::vector<std::string> _split(const std::string input, const char ch);

extern "C" void _ltrim(std::string &s, const char ch);
extern "C" void _rtrim(std::string &s, const char ch);
extern "C" void _trim(std::string &s, const char ch);

extern "C" std::string execute(const std::string raw_cmd);
extern "C" int get_pid_by_name(const char *);
