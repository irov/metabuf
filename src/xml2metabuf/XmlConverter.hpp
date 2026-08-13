#pragma once

#include "metabufconverter/OutputAdapter.hpp"

#include <cstddef>
#include <string>

namespace Metabuf
{
    class XmlProtocol;

    bool convertXml( const XmlProtocol * _protocol, const void * _buffer, size_t _size, const char * _meta, const char * _node, OutputAdapter & _output, std::string & _error );

    template<class T>
    bool convertXml( const XmlProtocol * _protocol, const void * _buffer, size_t _size, const char * _meta, const char * _node, T & _output, std::string & _error )
    {
        OutputAdapter output( _output );

        if( convertXml( _protocol, _buffer, _size, _meta, _node, output, _error ) == false )
        {
            return false;
        }

        return true;
    }
}
