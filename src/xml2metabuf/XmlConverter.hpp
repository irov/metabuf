#pragma once

#include "metabuf/Metaconvert.hpp"

#include <cstddef>
#include <string>

namespace Metabuf
{
    bool convertXml( const ProtocolInterface * _protocol, const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error );

    template<class T>
    bool convertXml( const ProtocolInterface * _protocol, const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, T & _output, std::string & _error )
    {
        OutputAdapter output( _output );

        if( convertXml( _protocol, _buffer, _size, _meta, _node, output, _error ) == false )
        {
            return false;
        }

        return true;
    }
}
