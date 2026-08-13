#include "xml2metabuf/XmlConverter.hpp"

#include "xml2metabuf/Xml2Metabuf.hpp"
#include "xml2metabuf/XmlProtocol.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    bool convertXml( const XmlProtocol * _protocol, const void * _buffer, size_t _size, const char * _meta, const char * _node, OutputAdapter & _output, std::string & _error )
    {
        _error.clear();
        _output.clear();

        const XmlMeta * meta = _protocol->getMeta( _meta );

        if( meta == nullptr )
        {
            _error = std::string( "protocol meta not found: " ) + _meta;

            return false;
        }

        const XmlNode * node = meta->getNode( _node );

        if( node == nullptr )
        {
            _error = std::string( "protocol node not found: " ) + _node;

            return false;
        }

        Xml2Metabuf xmlMetabuf( _protocol, meta );
        xmlMetabuf.initialize();

        if( xmlMetabuf.convert( _output, _buffer, _size, node ) == false )
        {
            _error = xmlMetabuf.getError();

            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
