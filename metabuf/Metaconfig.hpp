#pragma once

#ifndef METABUF_MALLOC
#   include <stdlib.h>
#	define METABUF_MALLOC(S) malloc(S)
#endif

#ifndef METABUF_FREE
#   include <stdlib.h>
#	define METABUF_FREE(B, S) free(B)
#endif

#ifndef METABUF_MEMCPY
#	include <algorithm>
#	define METABUF_MEMCPY(Dest, Src, Size) std::copy((const uint8_t *)Src, (const uint8_t *)Src + Size, (uint8_t *)Dest)
#endif

#include <vector>
#include <exception>

namespace Metabuf
{
    template<class Type, class Allocator = std::allocator<Type> >
    using Vector = std::vector<Type, Allocator>;

    using Exception = std::exception;
}