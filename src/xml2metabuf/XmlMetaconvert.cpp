#include "xml2metabuf/XmlMetaconvert.hpp"

#include "xml2metabuf/XmlConverter.hpp"

namespace Metabuf
{
    XmlMetaconvert::XmlMetaconvert()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    XmlMetaconvert::~XmlMetaconvert()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlMetaconvert::convert( const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error ) const
    {
        if( ::Metabuf::convertXml( m_protocol, _buffer, _size, _meta, _node, _output, _error ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    METABUF_EXPORT MetaconvertInterface * createXmlMetaconvert( const ProtocolGenerator * _generator )
    {
        if( _generator == nullptr )
        {
            return nullptr;
        }

        XmlMetaconvert * metaconvert = new XmlMetaconvert();
        metaconvert->generate( _generator );

        return metaconvert;
    }
    //////////////////////////////////////////////////////////////////////////
}
