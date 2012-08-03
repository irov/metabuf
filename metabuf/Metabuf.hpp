#	pragma once

#	include <memory.h>

namespace Metabuf
{
	class ArchiveReader
	{
	public:
		ArchiveReader( const char * _buff, size_t _size, size_t & _read )
			: m_buff(_buff)
			, m_size(_size)
			, m_read(_read)
		{
		}

	public:
		template<class T>
		inline void readPOD( T & _t )
		{
			char * buff = reinterpret_cast<char *>(&_t);
			this->readBuffer( buff, sizeof(T) );
		}

	public:
		inline void readBuffer( char * _begin, size_t _size )
		{
			memcpy( _begin, m_buff + m_read, _size );
			m_read += _size;
		}
		
	protected:
		const char * m_buff;
		size_t m_size;
		size_t & m_read;
	};

	template<class T>
	void operator >> ( ArchiveReader & ar, T & _value )
	{
		ar.readPOD( _value );
	}

	class Metadata
	{
	public:
		void parse( char * _buff, size_t _size, size_t & _read );

	public:
		template<class T>
		inline void read( const char * _buff, size_t _size, size_t & _read, T & _t )
		{
			ArchiveReader(_buff, _size, _read) >> _t;
		}

	protected:
		virtual void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t id ) = 0;
		virtual void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t id ) = 0;
	};
}