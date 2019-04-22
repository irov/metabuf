#pragma once

#ifdef METABUF_EXTERNAL_CONFIG
#include "Metaexternalconfig.hpp"
#else
#include <stdlib.h>
#include <string.h>

#define METABUF_MALLOC(S) malloc(S)
#define METABUF_FREE(B, S) free(B)
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