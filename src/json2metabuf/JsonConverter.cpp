#include "json2metabuf/JsonConverter.hpp"

#include "json2metabuf/Json2Metabuf.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    bool convertJson( const ProtocolInterface * _protocol, const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error )
    {
        const Json2Metabuf jsonMetabuf;

        if( jsonMetabuf.convert( _protocol, _buffer, _size, _meta, _node, _output, _error ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
