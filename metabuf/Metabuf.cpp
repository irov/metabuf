#	include "metabuf.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    bool Metadata::parse( const unsigned char * _buff, size_t _size, size_t & _read, void * _userData )
    {
        if( _size == _read )
        {
            return true;
        }

        m_userData = _userData;

		unsigned int attributeCount;
		this->readSize( _buff, _size, _read, attributeCount );

		for( unsigned int i = 0; i != attributeCount; ++i )
		{
			unsigned int id;
			this->readSize( _buff, _size, _read, id );

			if( this->_parseArguments( _buff, _size, _read, id ) == false )
            {
                return false;
            }
		}

        unsigned int includeTypeCount;
        this->readSize( _buff, _size, _read, includeTypeCount );

        for( unsigned int i = 0; i != includeTypeCount; ++i )
        {
    		unsigned int includeCount;
	    	this->read( _buff, _size, _read, includeCount );
		
            unsigned int id_includes;
            this->readSize( _buff, _size, _read, id_includes );

            if( this->_preparationIncludes( id_includes, includeCount ) == false )
            {
                return false;
            }

            for( unsigned int j = 0; j != includeCount; ++j )
            {
                if( this->_parseIncludes( _buff, _size, _read, id_includes ) == false )
                {
                    return false;
                }
            }
        }

        unsigned int generatorTypeCount;
        this->readSize( _buff, _size, _read, generatorTypeCount );

        for( unsigned int i = 0; i != generatorTypeCount; ++i )
        {
            unsigned int generatorCount;
            this->read( _buff, _size, _read, generatorCount );

            unsigned int id_includes;
            this->readSize( _buff, _size, _read, id_includes );

            if( this->_preparationIncludes( id_includes, generatorCount ) == false )
            {
                return false;
            }

            for( unsigned int j = 0; j != generatorCount; ++j )
            {
                unsigned int id_generators;
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
