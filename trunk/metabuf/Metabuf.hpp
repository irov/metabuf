#	pragma once

#	include <metabuf/ArchiveReader.hpp>

#   include <stdex/auto_array.h>

namespace Metabuf
{
    const size_t header_size = 8;

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
		inline bool read( const unsigned char * _buff, size_t _size, size_t & _read, T & _t ) const
		{
            try
            {
			    ArchiveReader ar(_buff, _size, _read, m_userData);
                ar.read( _t );
            }
            catch( const ArchiveException & )
            {
                return false;
            }

            return true;
		}

        inline void readSize( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _t ) const
        {
            ArchiveReader ar(_buff, _size, _read, m_userData);            
            ar.readSize( _t );
        }

	public:
		void * operator new ( size_t _size );
		void operator delete ( void * _ptr, size_t _size );

	protected:
		virtual bool _parseArguments( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t id ) = 0;

        virtual bool _preparationIncludes( uint32_t _includes, uint32_t _count ) = 0;
		virtual bool _parseIncludes( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _includes ) = 0;
        virtual bool _parseGenerators( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _generators ) = 0;

	protected:
		virtual uint32_t getId() const = 0;

    protected:
        void * m_userData;
	};
}