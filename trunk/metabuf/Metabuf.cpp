#	include "metabuf.hpp"

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	void Metadata::parse( char * _buff, size_t _size, size_t & _read )
	{
        _read = 0;

        size_t nodeId;
        this->readSize( _buff, _size, _read, nodeId );

        this->parseNode( _buff, _size, _read );
    }
    //////////////////////////////////////////////////////////////////////////
    void Metadata::parseNode( char * _buff, size_t _size, size_t & _read )
    {
		size_t attributeCount;
		this->readSize( _buff, _size, _read, attributeCount );

		for( size_t i = 0; i != attributeCount; ++i )
		{
			size_t id;
			this->readSize( _buff, _size, _read, id );

			this->_parseArguments( _buff, _size, _read, id );
		}

		size_t includeCount;
		this->readSize( _buff, _size, _read, includeCount );
		
		for( size_t i = 0; i != includeCount; ++i )
		{
			size_t id_includes;
			this->readSize( _buff, _size, _read, id_includes );

            size_t id_generators;
            this->readSize( _buff, _size, _read, id_generators );

			this->_parseIncludes( _buff, _size, _read, id_includes, id_generators );
		}
	}
}
