#	include "metabuf.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    bool Metadata::parse( const char * _buff, size_t _size, size_t & _read, void * _userData )
    {
        m_userData = _userData;

		size_t attributeCount;
		this->readSize( _buff, _size, _read, attributeCount );

		for( size_t i = 0; i != attributeCount; ++i )
		{
			size_t id;
			this->readSize( _buff, _size, _read, id );

			if( this->_parseArguments( _buff, _size, _read, id ) == false )
            {
                return false;
            }
		}

        size_t includeTypeCount;
        this->readSize( _buff, _size, _read, includeTypeCount );

        for( size_t i = 0; i != includeTypeCount; ++i )
        {
    		size_t includeCount;
	    	this->read( _buff, _size, _read, includeCount );
		
            size_t id_includes;
            this->readSize( _buff, _size, _read, id_includes );

            if( this->_preparationIncludes( id_includes, includeCount ) == false )
            {
                return false;
            }

            for( size_t j = 0; j != includeCount; ++j )
            {
                if( this->_parseIncludes( _buff, _size, _read, id_includes ) == false )
                {
                    return false;
                }
            }
        }

        size_t generatorTypeCount;
        this->readSize( _buff, _size, _read, generatorTypeCount );

        for( size_t i = 0; i != generatorTypeCount; ++i )
        {
            size_t generatorCount;
            this->read( _buff, _size, _read, generatorCount );

            size_t id_includes;
            this->readSize( _buff, _size, _read, id_includes );

            if( this->_preparationIncludes( id_includes, generatorCount ) == false )
            {
                return false;
            }

            for( size_t j = 0; j != generatorCount; ++j )
            {
                size_t id_generators;
                this->readSize( _buff, _size, _read, id_generators );

                if( this->_parseGenerators( _buff, _size, _read, id_generators ) == false )
                {
                    return false;
                }
            }
        }

        return true;
	}
}
