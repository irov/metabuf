#include "metabuf/Metadata.hpp"

#include "config/Metaconfig.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    Metadata::Metadata()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Metadata::~Metadata()
    {
    }
	//////////////////////////////////////////////////////////////////////////
    void * Metadata::operator new (size_t _size)
    {
        return METABUF_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void * Metadata::operator new []( size_t _size )
    {
        return METABUF_MALLOC( _size );
    }
        //////////////////////////////////////////////////////////////////////////
        void Metadata::operator delete (void * _ptr, size_t _size)
    {
        (void)_size;

        METABUF_FREE( _ptr, _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Metadata::operator delete []( void * _ptr, size_t _size )
    {
        (void)_size;

        METABUF_FREE( _ptr, _size );
    }
}
