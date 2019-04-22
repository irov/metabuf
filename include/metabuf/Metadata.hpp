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

    protected:
        template<class T>
        inline void read( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData, T & _t ) const
        {
            Reader ar( _buff, _size, _read );
            archive_read( ar, _t, _userData );
        }

        inline void readSize( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t & _t ) const
        {
            Reader ar( _buff, _size, _read );
            ar.readSize( _t );
        }

    public:
        void * operator new (size_t _size);
        void * operator new[]( size_t _size );

        void operator delete (void * _ptr, size_t _size);
        void operator delete[]( void * _ptr, size_t _size );
    };
}