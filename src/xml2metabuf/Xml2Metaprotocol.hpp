#pragma once

#include "metaconverter/Protocol.hpp"

#include <string>

namespace Metabuf
{
    class Xml2Metaprotocol
    {
    public:
        explicit Xml2Metaprotocol( const Protocol * _protocol );

    public:
        bool generate( std::string & _header, std::string & _source ) const;

    protected:
        const Protocol * m_protocol;
    };
}
