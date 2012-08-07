#	pragma once

#	include "ArchiveReader.hpp"

namespace Metabuf
{
	class Metadata
	{
	public:
		bool parse( const char * _buff, size_t _size, size_t & _read );
		
    public:
        virtual Metadata * generateMetadata( size_t _id ) = 0;

    public:
        bool parseNode( const char * _buff, size_t _size, size_t & _read );

    protected:
		template<class T>
		inline void read( const char * _buff, size_t _size, size_t & _read, T & _t )
		{
			ArchiveReader ar(_buff, _size, _read);
            ar >> _t;
		}

        inline void readSize( const char * _buff, size_t _size, size_t & _read, size_t & _t )
        {
            ArchiveReader(_buff, _size, _read).readSize( _t );
        }

	protected:
		virtual bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t id ) = 0;
		virtual bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) = 0;        
	};
}