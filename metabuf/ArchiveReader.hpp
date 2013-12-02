#	pragma once

#	include <stdint.h>
#	include <memory.h>

namespace Metabuf
{
    class ArchiveException
    {
    };

    class ArchiveReader
    {
    public:
        ArchiveReader( const unsigned char * _buff, size_t _size, size_t & _read, void * _userData )
            : m_buff(_buff)
            , m_size(_size)
            , m_read(_read)
            , m_userData(_userData)
        {
        }

    private:
        ArchiveReader & operator = ( const ArchiveReader & _ar )
        {
            (void)_ar;

            return *this;
        }

    public:
        template<class T>
        inline void read( T & _t );

        template<class T>
        inline void readPOD( T & _t )
        {
            unsigned char * buff = reinterpret_cast<unsigned char *>(&_t);
            this->readBuffer( buff, sizeof(T) );
        }

        template<class T>
        inline void readCount( T * _t, size_t _size )
        {
            unsigned char * buff = reinterpret_cast<unsigned char *>(_t);
            this->readBuffer( buff, sizeof(T) * _size );
        }

    public:        
        inline void readBuffer( unsigned char * _begin, size_t _size )
        {
            memcpy( _begin, m_buff + m_read, _size );
            m_read += _size;
        }

        inline void readSize( unsigned int & _size )
        {
            unsigned char size_1;
            this->readPOD( size_1 );

            if( size_1 == 255 )
            {
                unsigned int size_2;
                this->readPOD( size_2 );

                _size = size_2;
            }
            else
            {
                _size = size_1;
            }
        }

    public:
        template<class T>
        inline const T * current_buff() const
        {
            const unsigned char * current = m_buff + m_read;
            return reinterpret_cast<const T *>(current);
        }

        inline void skip( size_t _size )
        {
            m_read += _size;
        }

    protected:
        const unsigned char * m_buff;
        size_t m_size;
        size_t & m_read;
        void * m_userData;
    };
    
    template<class T>
    void archive_read( ArchiveReader & ar, T & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value );
    }
    
    template<class T>
    inline void ArchiveReader::read( T & _t )
    {
        archive_read( *this, _t, m_userData );
    }
}
