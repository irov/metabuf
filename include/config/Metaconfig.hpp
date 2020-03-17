#pragma once

#ifdef METABUF_EXTERNAL_CONFIG
#include "Metaexternalconfig.hpp"
#else
#include <string.h>

#define METABUF_MEMCPY(Dest, Src, Size) memcpy(Dest, Src, Size)

#include <vector>
#include <exception>

namespace Metabuf
{
    template<class Type, class Allocator = std::allocator<Type> >
    using Vector = std::vector<Type, Allocator>;

    using Exception = std::exception;
}
#endif