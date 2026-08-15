#include "metaconverter/Converter.hpp"

#include "metaconverter/MetabufWriter.hpp"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    bool makeHeader( const ProtocolInterface * _protocol, const MetaInterface * _meta, OutputAdapter & _output, std::string & _error )
    {
        _error.clear();
        _output.clear();

        if( _meta == nullptr )
        {
            _error = "protocol meta is null";

            return false;
        }

        const uint32_t header[] = {
            3133062829U,
            METABUF_BIN_VERSION,
            _protocol->getVersion(),
            _protocol->getCrc32(),
            _meta->getVersion()
        };

        const size_t headerSize = sizeof( header );
        _output.reserve( headerSize );
        _output.append( reinterpret_cast<const uint8_t *>(header), headerSize );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool writeMetabuf( const ProtocolInterface * _protocol, const MetaInterface * _meta, const NodeInterface * _node, const pugi::xml_node & _dataNode, OutputAdapter & _output, std::string & _error )
    {
        MetabufWriter writer( _protocol, _meta );
        writer.initialize();

        if( writer.write( _output, _dataNode, _node ) == false )
        {
            _error = writer.getError();

            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
