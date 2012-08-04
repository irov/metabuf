#	pragma once

#	include "ArchiveReader.hpp"

namespace Metabuf
{
	class Metadata
	{
	public:
		void parse( char * _buff, size_t _size, size_t & _read );
        void parseNode( char * _buff, size_t _size, size_t & _read );

	public:
		template<class T>
		inline void read( const char * _buff, size_t _size, size_t & _read, T & _t )
		{
			ArchiveReader(_buff, _size, _read) >> _t;
		}

        inline void readSize( const char * _buff, size_t _size, size_t & _read, size_t & _t )
        {
            ArchiveReader(_buff, _size, _read).readSize( _t );
        }

	protected:
		virtual void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t id ) = 0;
		virtual void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) = 0;
	};
}