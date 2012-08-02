#	include "metabuf.hpp"

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	void Metadata::parse( char * _buff, size_t _size, size_t & _read )
	{
		size_t count;
		this->read( _buff, _size, _read, count );

		for( size_t i = 0; i != count; ++i )
		{
			size_t id;
			this->read( _buff, _size, _read, id );

			this->_parse( _buff, _size, _read, id );
		}
	}
}