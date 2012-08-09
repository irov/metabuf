#	include "metabuf.hpp"

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	bool Metadata::parse( const char * _buff, size_t _size, size_t & _read )
	{
        _read = 0;

        size_t nodeId;
        this->readSize( _buff, _size, _read, nodeId );

        if( this->parseNode( _buff, _size, _read ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Metadata::parseNode( const char * _buff, size_t _size, size_t & _read )
    {
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

            this->_preparationIncludes( id_includes );

            for( size_t i = 0; i != includeCount; ++i )
            {
                if( this->_parseIncludes( _buff, _size, _read, id_includes ) == false )
                {
                    return false;
                }
            }
        }

        size_t generatorCount;
        this->readSize( _buff, _size, _read, generatorCount );

        for( size_t i = 0; i != generatorCount; ++i )
        {
            size_t id_includes;
            this->readSize( _buff, _size, _read, id_includes );

            size_t id_generators;
            this->readSize( _buff, _size, _read, id_generators );

            for( size_t i = 0; i != includeCount; ++i )
            {
                if( this->_parseGenerators( _buff, _size, _read, id_includes, id_generators ) == false )
                {
                    return false;
                }
            }
        }

        return true;
	}
}
