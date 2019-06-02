#include "Utils/Utils.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

void LWGC::PrintCallstack(void)
{
    int j, nptrs;
#define SIZE 100
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        perror("backtrace_symbols");
        return ;
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
}

std::string LWGC::GetExtension(const std::string & path)
{
    return path.substr(path.find_last_of(".") + 1);
}

std::string LWGC::GetFileName(const std::string & path)
{
    size_t l = path.find_last_of('/');
    if (l == std::string::npos)
        return path;
    else
        return path.substr(l + 1, path.size() - l);
}

std::string LWGC::GetFileNameWithoutExtension(const std::string & path)
{
    std::string fileName = GetFileName(path);
    size_t l = fileName.find('.');
    if (l == std::string::npos)
        return fileName;
    else
        return fileName.substr(0, l);
}

bool        LWGC::EndsWith(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
