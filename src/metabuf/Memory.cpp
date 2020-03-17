#include "metabuf/Memory.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////    
    static Metabuf_malloc_t s_Metabuf_malloc;
    static Metabuf_free_t s_Metabuf_free;
    static void * s_Metabuf_ud;
    //////////////////////////////////////////////////////////////////////////
    void set_Metabuf_allocator( Metabuf_malloc_t _malloc, Metabuf_free_t _free, void * _ud )
    {
        s_Metabuf_malloc = _malloc;
        s_Metabuf_free = _free;
        s_Metabuf_ud = _ud;
    }
    //////////////////////////////////////////////////////////////////////////
    void * Metabuf_malloc( size_t _size )
    {
        void * p = (*s_Metabuf_malloc)(_size, s_Metabuf_ud);

        return p;
    }
    //////////////////////////////////////////////////////////////////////////
    void Metabuf_free( void * _ptr )
    {
        (*s_Metabuf_free)(_ptr, s_Metabuf_ud);
    }
    //////////////////////////////////////////////////////////////////////////
}