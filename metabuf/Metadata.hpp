#	pragma once

//#   include <stdex/auto_array.h>

#   include "Reader.hpp"

namespace Metabuf
{
    const size_t header_size = 12;

	template<class T>
	void archive_read( Reader & ar, T & _value, void * _userData );

	class Metadata
	{
	public:
		Metadata();
		virtual ~Metadata();

	public:
		bool parseRoot( const unsigned char * _buff, size_t _size, size_t & _read, void * _userData );
		bool parse( const unsigned char * _buff, size_t _size, size_t & _read, void * _userData );

    protected:
		template<class T>
		inline void read( const unsigned char * _buff, size_t _size, size_t & _read, T & _t ) const
		{
			Reader ar( _buff, _size, _read );
			archive_read( ar, _t, m_userData );
		}

        inline void readSize( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _t ) const
        {
			Reader ar(_buff, _size, _read);
			ar.readSize( _t );			
        }

	public:
		void * operator new ( size_t _size );
		void * operator new [] ( size_t _size );

		void operator delete ( void * _ptr, size_t _size );
		void operator delete [] ( void * _ptr, size_t _size );

	protected:
		virtual void _parseData(const unsigned char * _buff, size_t _size, size_t & _read) = 0;
		virtual void _parseArguments( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t id ) = 0;

		virtual void _preparationIncludes( uint32_t _includes, uint32_t _count ) = 0;
		virtual void _parseIncludes( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _includes ) = 0;
		virtual void _parseGenerators( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _generators ) = 0;

	protected:
		virtual uint32_t getId() const = 0;

    protected:
        void * m_userData;
	};
}