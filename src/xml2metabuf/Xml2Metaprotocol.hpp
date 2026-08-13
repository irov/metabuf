#pragma once

#include "XmlProtocol.hpp"

#include <string>

namespace Metabuf
{
    class Xml2Metaprotocol
    {
    public:
        explicit Xml2Metaprotocol( const XmlProtocol * _protocol );

    public:
        bool generate( std::string & _header, std::string & _source ) const;

    protected:
        const XmlProtocol * m_protocol;
    };
}
