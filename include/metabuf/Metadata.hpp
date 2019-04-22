#pragma once

#include "metabuf/Reader.hpp"

#include <stddef.h>
#include <stdint.h>

namespace Metabuf
{
    enum HeaderError
    {
        HEADER_SUCCESSFUL,
        HEADER_INVALID_MAGIC,
        HEADER_INVALID_VERSION,
        HEADER_INVALID_PROTOCOL_VERSION,
        HEADER_INVALID_PROTOCOL_CRC32,
        HEADER_INVALID_METAVERSION,
    };

    /**********************************************************************
    *    template<class T>
    *    void archive_read( Reader & ar, T & _value, void * _userData );
    ***********************************************************************/

    class Metadata
    {
    public:
        Metadata();
        ~Metadata();

    public:
        void * operator new (size_t _size);
        void * operator new[]( size_t _size );

        void operator delete (void * _ptr, size_t _size);
        void operator delete[]( void * _ptr, size_t _size );
    };

    class Metaparse
        : public Metadata
    {
    public:
        Metaparse();
        ~Metaparse();

    public:
        virtual bool parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData ) = 0;
    };
}