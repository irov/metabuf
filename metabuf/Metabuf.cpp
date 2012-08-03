#	include "metabuf.hpp"

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	void Metadata::parse( char * _buff, size_t _size, size_t & _read )
	{
		size_t attributeCount;
		this->read( _buff, _size, _read, attributeCount );

		for( size_t i = 0; i != attributeCount; ++i )
		{
			size_t id;
			this->read( _buff, _size, _read, id );

			this->_parseArguments( _buff, _size, _read, id );
		}

		size_t includeCount;
		this->read( _buff, _size, _read, includeCount );
		
		for( size_t i = 0; i != includeCount; ++i )
		{
			size_t id;
			this->read( _buff, _size, _read, id );

			this->_parseIncludes( _buff, _size, _read, id );
		}
	}
}