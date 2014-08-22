#	include "Metabuf.hpp"
#	include "Metaconfig.hpp"

#	ifndef METABUF_MALLOC
#	include <malloc.h>
#	define METABUF_MALLOC(S) malloc(S)
#	endif

#	ifndef METABUF_FREE
#	include <malloc.h>
#	define METABUF_FREE(B, S) free(B)
#	endif

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	Metadata::Metadata()
		: m_userData(nullptr)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Metadata::~Metadata()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool Metadata::parseRoot( const unsigned char * _buff, size_t _size, size_t & _read, void * _userData )
	{
		if( _size == _read )
		{
			return true;
		}

		uint32_t rootId;
		if( this->readSize( _buff, _size, _read, rootId ) == false )
		{
			return false;
		}

		uint32_t metaId = this->getId();
		if( rootId != metaId )
		{
			return false;
		}

		bool result = this->parse( _buff, _size, _read, _userData );

		return result;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Metadata::parse( const unsigned char * _buff, size_t _size, size_t & _read, void * _userData )
    {
        if( _size == _read )
        {
            return true;
        }

        m_userData = _userData;

		uint32_t attributeCount;
		if( this->readSize( _buff, _size, _read, attributeCount ) == false )
		{
			return false;
		}

		for( uint32_t i = 0; i != attributeCount; ++i )
		{
			uint32_t id;
			if( this->readSize( _buff, _size, _read, id ) == false )
			{
				return false;
			}

			if( this->_parseArguments( _buff, _size, _read, id ) == false )
            {
                return false;
            }
		}

        uint32_t includeTypeCount;
        if( this->readSize( _buff, _size, _read, includeTypeCount ) == false )
		{
			return false;
		}

        for( uint32_t i = 0; i != includeTypeCount; ++i )
        {
    		uint32_t includeCount;
	    	if( this->readSize( _buff, _size, _read, includeCount ) == false )
			{
				return false;
			}

            uint32_t id_includes;
            if( this->readSize( _buff, _size, _read, id_includes ) == false )
			{
				return false;
			}

            if( this->_preparationIncludes( id_includes, includeCount ) == false )
            {
                return false;
            }

            for( uint32_t j = 0; j != includeCount; ++j )
            {
                if( this->_parseIncludes( _buff, _size, _read, id_includes ) == false )
                {
                    return false;
                }
            }
        }

        uint32_t generatorTypeCount;
        if( this->readSize( _buff, _size, _read, generatorTypeCount ) == false )
		{
			return false;
		}

        for( uint32_t i = 0; i != generatorTypeCount; ++i )
        {
            uint32_t generatorCount;
            if( this->readSize( _buff, _size, _read, generatorCount ) == false )
			{
				return false;
			}

            uint32_t id_includes;
            if( this->readSize( _buff, _size, _read, id_includes ) == false )
			{
				return false;
			}

            if( this->_preparationIncludes( id_includes, generatorCount ) == false )
            {
                return false;
            }

            for( uint32_t j = 0; j != generatorCount; ++j )
            {
                uint32_t id_generators;
                if( this->readSize( _buff, _size, _read, id_generators ) == false )
				{
					return false;
				}

                if( this->_parseGenerators( _buff, _size, _read, id_generators ) == false )
                {
                    return false;
                }
            }
        }

        return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void * Metadata::operator new ( size_t _size )
	{
		return METABUF_MALLOC(_size);
	}
	//////////////////////////////////////////////////////////////////////////
	void * Metadata::operator new [] ( size_t _size )
	{
		return METABUF_MALLOC(_size);
	}
	//////////////////////////////////////////////////////////////////////////
	void Metadata::operator delete ( void * _ptr, size_t _size )
	{		
		(void)_size;

		METABUF_FREE(_ptr, _size);
	}
	//////////////////////////////////////////////////////////////////////////
	void Metadata::operator delete [] ( void * _ptr, size_t _size )
	{
		(void)_size;

		METABUF_FREE(_ptr, _size);
	}
}
