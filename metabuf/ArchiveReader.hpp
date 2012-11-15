#	pragma once

#	include <memory.h>

namespace Metabuf
{
    class ArchiveReader;

    template<class T>
    void archive_read( ArchiveReader & ar, T & _value, void * _userData );

    class ArchiveReader
    {
    public:
        ArchiveReader( const char * _buff, size_t _size, size_t & _read, void * _userData )
            : m_buff(_buff)
            , m_size(_size)
            , m_read(_read)
            , m_userData(_userData)
        {
        }

    public:
        template<class T>
        inline void read( T & _t )
        {
            Metabuf::archive_read( *this, _t, m_userData );
        }

        template<class T>
        inline void readPOD( T & _t )
        {
            char * buff = reinterpret_cast<char *>(&_t);
            this->readBuffer( buff, sizeof(T) );
        }

        template<class T>
        inline void readCount( T * _t, size_t _size )
        {
            char * buff = reinterpret_cast<char *>(_t);
            this->readBuffer( buff, sizeof(T) * _size );
        }

    public:        
        inline void readBuffer( char * _begin, size_t _size )
        {
            memcpy( _begin, m_buff + m_read, _size );
            m_read += _size;
        }

        inline void readSize( size_t & _size )
        {
            unsigned char size_1;
            char * begin = reinterpret_cast<char *>(&size_1);
            memcpy( begin, m_buff + m_read, 1 );
            m_read += 1;

            _size = size_1;
        }

    protected:
        const char * m_buff;
        size_t m_size;
        size_t & m_read;
        void * m_userData;
    };
    
    template<class T>
    void archive_read( ArchiveReader & ar, T & _value, void * _userData )
    {
        ar.readPOD( _value );
    }
}


