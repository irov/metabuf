#include "metabufconverter/Converter.hpp"

#include "xml2metabuf/Xml2Metabuf.hpp"
#include "xml2metabuf/XmlProtocol.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    bool makeHeader( const XmlProtocol * _protocol, const char * _meta, OutputAdapter & _output, std::string & _error )
    {
        _error.clear();
        _output.clear();

        const XmlMeta * meta = _protocol->getMeta( _meta );

        if( meta == nullptr )
        {
            _error = std::string( "protocol meta not found: " ) + _meta;

            return false;
        }

        Xml2Metabuf writer( _protocol, meta );
        writer.initialize();

        const size_t headerSize = sizeof( uint32_t ) * 5;
        uint8_t header[headerSize];
        size_t writeSize;

        if( writer.header( header, headerSize, meta->getVersion(), &writeSize ) == false )
        {
            _error = writer.getError();

            return false;
        }

        _output.reserve( writeSize );
        _output.append( header, writeSize );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
