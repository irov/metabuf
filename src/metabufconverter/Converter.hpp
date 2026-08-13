#pragma once

#include "metabufconverter/OutputAdapter.hpp"

#include <cstddef>
#include <string>

namespace Metabuf
{
    class XmlProtocol;

    bool makeHeader( const XmlProtocol * _protocol, const char * _meta, OutputAdapter & _output, std::string & _error );

    template<class T>
    bool makeHeader( const XmlProtocol * _protocol, const char * _meta, T & _output, std::string & _error )
    {
        OutputAdapter output( _output );

        if( makeHeader( _protocol, _meta, output, _error ) == false )
        {
            return false;
        }

        return true;
    }
}
