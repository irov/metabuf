#pragma once

#ifdef METABUF_EXTERNAL_CONFIG
#include "Metaexternalconfig.hpp"
#else
#include <malloc.h>
#include <string.h>

#ifndef METABUF_MALLOC
#define METABUF_MALLOC(S) (::malloc(S))
#endif

#ifndef METABUF_FREE
#define METABUF_FREE(B, S) (::free(B))
#endif

#define METABUF_MEMCPY(Dest, Src, Size) (::memcpy(Dest, Src, Size))

#include <vector>
#include <exception>

namespace Metabuf
{
    template<class Type, class Allocator = std::allocator<Type> >
    using Vector = std::vector<Type, Allocator>;

    using Exception = std::exception;
}
#endif