#	include "metabuf.hpp"
#	include "metaconfig.hpp"

#	include <malloc.h>

#	ifndef METABUF_MALLOC
#	define METABUF_MALLOC malloc
#	endif

#	ifndef METABUF_FREE
#	define METABUF_FREE free
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
		this->readSize( _buff, _size, _read, rootId );

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
		this->readSize( _buff, _size, _read, attributeCount );

		for( uint32_t i = 0; i != attributeCount; ++i )
		{
			uint32_t id;
			this->readSize( _buff, _size, _read, id );

			if( this->_parseArguments( _buff, _size, _read, id ) == false )
            {
                return false;
            }
		}

        uint32_t includeTypeCount;
        this->readSize( _buff, _size, _read, includeTypeCount );

        for( uint32_t i = 0; i != includeTypeCount; ++i )
        {
    		uint32_t includeCount;
	    	this->readSize( _buff, _size, _read, includeCount );

            uint32_t id_includes;
            this->readSize( _buff, _size, _read, id_includes );

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
        this->readSize( _buff, _size, _read, generatorTypeCount );

        for( uint32_t i = 0; i != generatorTypeCount; ++i )
        {
            uint32_t generatorCount;
            this->readSize( _buff, _size, _read, generatorCount );

            uint32_t id_includes;
            this->readSize( _buff, _size, _read, id_includes );

            if( this->_preparationIncludes( id_includes, generatorCount ) == false )
            {
                return false;
            }

            for( uint32_t j = 0; j != generatorCount; ++j )
            {
                uint32_t id_generators;
                this->readSize( _buff, _size, _read, id_generators );

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
	void Metadata::operator delete ( void * _ptr, size_t _size )
	{
		METABUF_FREE(_ptr);
	}
}
