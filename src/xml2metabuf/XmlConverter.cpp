#include "xml2metabuf/XmlConverter.hpp"

#include "metaconverter/Converter.hpp"

#include "pugixml.hpp"

#include <sstream>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    bool convertXml( const ProtocolInterface * _protocol, const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error )
    {
        _error.clear();
        _output.clear();

        if( _meta == nullptr )
        {
            _error = "protocol meta is null";

            return false;
        }

        if( _node == nullptr )
        {
            _error = "protocol node is null";

            return false;
        }

        pugi::xml_document document;
        pugi::xml_parse_result result = document.load_buffer( _buffer, _size );

        if( result == false )
        {
            std::stringstream ss;
            ss << "XML parse error at byte " << result.offset << ": " << result.description();
            _error = ss.str();

            return false;
        }

        const pugi::xml_node root = document.document_element();

        if( writeMetabuf( _protocol, _meta, _node, root, _output, _error ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
