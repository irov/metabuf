#	pragma once

#	include "ArchiveReader.hpp"

namespace Metabuf
{
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

        inline void readSize( const unsigned char * _buff, size_t _size, size_t & _read, size_t & _t ) const
        {
            ArchiveReader ar(_buff, _size, _read, m_userData);            
            ar.readSize( _t );
        }

	protected:
		virtual bool _parseArguments( const unsigned char * _buff, size_t _size, size_t & _read, size_t id ) = 0;

        virtual bool _preparationIncludes( size_t _includes, size_t _count ) = 0;
		virtual bool _parseIncludes( const unsigned char * _buff, size_t _size, size_t & _read, size_t _includes ) = 0;
        virtual bool _parseGenerators( const unsigned char * _buff, size_t _size, size_t & _read, size_t _generators ) = 0;

    protected:
        void * m_userData;
	};
}