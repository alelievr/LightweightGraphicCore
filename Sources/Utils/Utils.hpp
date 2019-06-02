#pragma once

#include <iostream>

namespace LWGC
{
    void	PrintCallstack(void);
    std::string GetFileName(const std::string & path);
    std::string GetFileNameWithoutExtension(const std::string & path);
    std::string GetExtension(const std::string & path);
    bool        EndsWith(std::string const & value, std::string const & ending);
}