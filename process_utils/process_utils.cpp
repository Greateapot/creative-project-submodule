/*
    pid by name: tasklist /FI "IMAGENAME eq {image_name} /FO CSV /NH"
    kill pid: tasklist /F /IM {image_name}
*/
#include "process_utils.h"

#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <algorithm>

extern "C" std::vector<std::string> _split(const std::string input, const char ch)
{
    std::stringstream ss(input);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(ss, segment, ch))
        seglist.push_back(segment);

    return seglist;
}

extern "C" void _ltrim(std::string &s, const char ch)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&ch](unsigned char c)
                                    { return c != ch; }));
}

extern "C" void _rtrim(std::string &s, const char ch)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [&ch](unsigned char c)
                         { return c != ch; })
                .base(),
            s.end());
}

extern "C" void _trim(std::string &s, const char ch)
{
    _rtrim(s, ch);
    _ltrim(s, ch);
}

extern "C" BOOL _start(const std::string raw_cmd, STARTUPINFOW *si, PROCESS_INFORMATION *pi)
{
    std::wstring wCommand(raw_cmd.begin(), raw_cmd.end());
    const wchar_t *cmd = wCommand.c_str();

    return CreateProcessW(NULL, (LPWSTR)cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, si, pi);
}

extern "C" std::string _execute(const std::string raw_cmd)
{
    std::string strResult;

    HANDLE hPipeRead, hPipeWrite;

    SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES)};
    saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe to get results from child's stdout.
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
        return strResult;

    STARTUPINFOW si = {sizeof(STARTUPINFOW)};
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; // 0x101 -> 5
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
                              // Requires STARTF_USESHOWWINDOW in dwFlags.

    PROCESS_INFORMATION pi = {0};

    BOOL fSuccess = _start(raw_cmd, &si, &pi);

    if (!fSuccess)
    {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        return strResult;
    }

    bool bProcessEnded = false;
    for (; !bProcessEnded;)
    {
        // Give some timeslice (100 ms), so we won't waste 100% CPU.
        bProcessEnded = WaitForSingleObject(pi.hProcess, 100) == WAIT_OBJECT_0; // == 0 -> false

        // Even if process exited - we continue reading, if
        // there is some data available over pipe.
        for (;;)
        {
            char buf[1024];
            DWORD dwRead = 0;
            DWORD dwAvail = 0;

            if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
                break;

            if (!dwAvail) // No data available, return
                break;

            // typecast for 32-bit (idk why)
            if (!::ReadFile(hPipeRead, buf, std::min((unsigned long)(sizeof(buf) - 1), dwAvail), &dwRead, NULL) || !dwRead)
                // Error, the child process might ended
                break;

            buf[dwRead] = 0;
            strResult += buf;
        }
    }

    CloseHandle(hPipeWrite);
    CloseHandle(hPipeRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return strResult;
}

extern "C" int start_by_name(const char *image_name, const char *args)
{
    const std::string image_name_str(image_name);
    const std::string args_str(args);
    const std::string command = image_name_str + " " + args_str;

    STARTUPINFOW si = {sizeof(STARTUPINFOW)};
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; // 0x101 -> 5
    si.wShowWindow = SW_HIDE;                                 // Prevents cmd window from flashing.
                                                              // Requires STARTF_USESHOWWINDOW in dwFlags.

    PROCESS_INFORMATION pi = {0};

    return _start(command, &si, &pi);
}

extern "C" int kill_by_name(const char *image_name)
{
    const std::string image_name_str(image_name);
    const std::string command = "taskkill /F /IM " + image_name_str;
    std::string result = _execute(command);
    // TODO: assert err (idk how)
    return 1;
}

extern "C" int get_pid_by_name(const char *image_name)
{
    const std::string image_name_str(image_name);
    const std::string command = "tasklist /FI \"IMAGENAME eq " + image_name_str + "\" /FO CSV /NH";
    std::string result = _execute(command);
    if (result.find(image_name) != std::string::npos)
    {
        std::vector<std::string> data = _split(result, ',');
        std::string raw_pid = data[1];
        _trim(raw_pid, '"');
        return std::stoi(raw_pid);
    }
    return -1;
}