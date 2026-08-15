#pragma once

#include "metabuf/Metaconvert.hpp"

#include <cstddef>
#include <string>

namespace pugi
{
    class xml_node;
}

namespace Metabuf
{
    bool makeHeader( const ProtocolInterface * _protocol, const MetaInterface * _meta, OutputAdapter & _output, std::string & _error );
    bool writeMetabuf( const ProtocolInterface * _protocol, const MetaInterface * _meta, const NodeInterface * _node, const pugi::xml_node & _dataNode, OutputAdapter & _output, std::string & _error );

    template<class T>
    bool makeHeader( const ProtocolInterface * _protocol, const MetaInterface * _meta, T & _output, std::string & _error )
    {
        OutputAdapter output( _output );

        if( makeHeader( _protocol, _meta, output, _error ) == false )
        {
            return false;
        }

        return true;
    }
}
