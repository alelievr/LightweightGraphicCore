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

std::string LWGC::GetFileName(const std::string & path)
{
    size_t l = path.find('/');
    return path.substr(l, path.size() - l);
}

std::string LWGC::GetFileNameWithoutExtension(const std::string & path)
{
    std::string fileName = GetFileName(path);
    size_t l = fileName.find('.');
    return path.substr(l, fileName.size() - l);
}