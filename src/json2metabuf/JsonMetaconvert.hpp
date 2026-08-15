#pragma once

#include "metaconverter/Metaconvert.hpp"

namespace Metabuf
{
    class JsonMetaconvert
        : public Metaconvert
    {
    public:
        JsonMetaconvert();
        ~JsonMetaconvert() override;

    public:
        bool convert( const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error ) const override;
    };
}
