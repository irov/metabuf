#	pragma once

#	include "Metaconfig.hpp"

#	include <stdint.h>

namespace Metabuf
{
	class ReaderException
		: public Exception
	{
	};

	class Reader
	{
	public:
		Reader( const void * _buff, size_t _capacity, size_t & _read )
			: m_buff(_buff)
			, m_capacity(_capacity)
			, m_read(_read)
		{
		}

	private:
		Reader & operator = ( const Reader & )
		{
			return *this;
		}

	public:
		template<class T>
		inline void readPOD( T & _t )
		{
			void * buff = (void *)(&_t);
			this->readBuffer( buff, sizeof(T) );
		}

		template<class T>
		inline void readPODs( T * _t, size_t _size )
		{
			void * buff = (void *)(_t);
			this->readBuffer( buff, sizeof(T) * _size );
		}

	public:        
		inline void readBuffer( void * _begin, size_t _size )
		{
			if( m_read + _size > m_capacity )
			{
				throw ReaderException();
			}

			const uint8_t * read_buff = (const uint8_t *)m_buff + m_read;
			METABUF_MEMCPY( _begin, read_buff, _size );

			m_read += _size;
		}

		inline void readSize( uint32_t & _size )
		{
			uint8_t size_1;
			this->readPOD( size_1 );

			if( size_1 == 255 )
			{
				uint32_t size_2;
				this->readPOD( size_2 );

				_size = size_2;
			}
			else
			{
				_size = (uint32_t)size_1;
			}
		}

	public:
		template<class T>
		inline const T * current_buff() const
		{
			const uint8_t * current = (const uint8_t *)m_buff + m_read;
			const T * buff = reinterpret_cast<const T *>(current);

			return buff;
		}

		inline void skip( size_t _size )
		{
			m_read += _size;
		}

	public:
		template<class T>
		void operator << ( T & _t )
		{
			this->readPOD( _t );
		}

	protected:
		const void * m_buff;
		size_t m_capacity;
		size_t & m_read;
	};
}
