#include "json2metabuf/JsonMetaconvert.hpp"

#include "json2metabuf/JsonConverter.hpp"

namespace Metabuf
{
    JsonMetaconvert::JsonMetaconvert()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    JsonMetaconvert::~JsonMetaconvert()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool JsonMetaconvert::convert( const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error ) const
    {
        if( ::Metabuf::convertJson( m_protocol, _buffer, _size, _meta, _node, _output, _error ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    METABUF_EXPORT MetaconvertInterface * createJsonMetaconvert( const ProtocolGenerator * _generator )
    {
        if( _generator == nullptr )
        {
            return nullptr;
        }

        JsonMetaconvert * metaconvert = new JsonMetaconvert();
        metaconvert->generate( _generator );

        return metaconvert;
    }
    //////////////////////////////////////////////////////////////////////////
}
