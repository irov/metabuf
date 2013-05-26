#	pragma once

#	include <metabuf/ArchiveReader.hpp>
#   include <metabuf/Array.hpp>

namespace Metabuf
{
    const size_t header_size = 8;

	class Metadata
	{
	public:
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

        inline void readSize( const unsigned char * _buff, size_t _size, size_t & _read, unsigned int & _t ) const
        {
            ArchiveReader ar(_buff, _size, _read, m_userData);            
            ar.readSize( _t );
        }

	protected:
		virtual bool _parseArguments( const unsigned char * _buff, size_t _size, size_t & _read, unsigned int id ) = 0;

        virtual bool _preparationIncludes( unsigned int _includes, unsigned int _count ) = 0;
		virtual bool _parseIncludes( const unsigned char * _buff, size_t _size, size_t & _read, unsigned int _includes ) = 0;
        virtual bool _parseGenerators( const unsigned char * _buff, size_t _size, size_t & _read, unsigned int _generators ) = 0;

    protected:
        void * m_userData;
	};
}