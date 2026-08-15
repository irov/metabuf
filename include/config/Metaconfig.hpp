#pragma once

#include <stdlib.h>
#include <string.h>

#include <vector>

#if defined(METABUF_EXTERNAL_ALLOCATOR)
extern void * _metabuf_malloc( size_t _size );
extern void _metabuf_free( void * _ptr );
#endif

#ifndef METABUF_UNUSED
#define METABUF_UNUSED(X) (void)X
#endif

#ifndef METABUF_MALLOC
#   if defined(METABUF_EXTERNAL_ALLOCATOR)
#       define METABUF_MALLOC(S) (_metabuf_malloc(S))
#   else
#       define METABUF_MALLOC(S) (::malloc(S))
#   endif
#endif

#ifndef METABUF_FREE
#   if defined(METABUF_EXTERNAL_ALLOCATOR)
#       define METABUF_FREE(B, S) (_metabuf_free(B))
#   else
#       define METABUF_FREE(B, S) (::free(B))
#   endif
#endif

#ifndef METABUF_MEMCPY
#define METABUF_MEMCPY(Dest, Src, Size) (::memcpy(Dest, Src, Size))
#endif

namespace Metabuf
{
    template<class Type>
    class StlAllocator
    {
    public:
        typedef Type value_type;

        StlAllocator() = default;
        template<class OtherType>
        StlAllocator( const StlAllocator<OtherType> & ) noexcept
        {
        }

        Type * allocate( std::size_t _count, const void * = nullptr )
        {
            std::size_t element_size = sizeof( Type );
            std::size_t total_size = element_size * _count;
            void * memory = METABUF_MALLOC( total_size );

            return static_cast<Type *>(memory);
        }

        void deallocate( Type * _memory, std::size_t _size )
        {
            METABUF_UNUSED( _size );

            METABUF_FREE( _memory, _size );
        }
    };

    template<class Type>
    bool operator == ( const StlAllocator<Type> &, const StlAllocator<Type> & )
    {
        return true;
    }

    template<class Type>
    bool operator != ( const StlAllocator<Type> &, const StlAllocator<Type> & )
    {
        return false;
    }

    template<class Type, class Allocator = StlAllocator<Type>>
    using Vector = std::vector<Type, Allocator>;
}
