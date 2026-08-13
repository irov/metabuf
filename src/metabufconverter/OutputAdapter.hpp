#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Metabuf
{
    class OutputAdapter
    {
    public:
        template<class T>
        explicit OutputAdapter( T & _output )
            : m_output( &_output )
            , m_clear( &OutputAdapter::clear_<T> )
            , m_reserve( &OutputAdapter::reserve_<T> )
            , m_append( &OutputAdapter::append_<T> )
        {
        }

    public:
        void clear() const
        {
            (*m_clear)(m_output);
        }

        void reserve( size_t _size ) const
        {
            (*m_reserve)(m_output, _size);
        }

        void append( const uint8_t * _buffer, size_t _size ) const
        {
            if( _size == 0 )
            {
                return;
            }

            (*m_append)(m_output, _buffer, _size);
        }

    protected:
        template<class T>
        static void clear_( void * _output )
        {
            T * output = static_cast<T *>(_output);
            output->clear();
        }

        template<class T>
        static void reserve_( void * _output, size_t _size )
        {
            T * output = static_cast<T *>(_output);
            output->reserve( _size );
        }

        template<class T>
        static void append_( void * _output, const uint8_t * _buffer, size_t _size )
        {
            T * output = static_cast<T *>(_output);
            output->insert( output->end(), _buffer, _buffer + _size );
        }

    protected:
        typedef void( *Clear )(void * _output);
        typedef void( *Reserve )(void * _output, size_t _size);
        typedef void( *Append )(void * _output, const uint8_t * _buffer, size_t _size);

        void * m_output;
        Clear m_clear;
        Reserve m_reserve;
        Append m_append;
    };
}
