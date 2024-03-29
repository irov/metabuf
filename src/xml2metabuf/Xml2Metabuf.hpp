#pragma once

#include "config/Metaconfig.hpp"

#include "pugixml.hpp"

#include <vector>
#include <map>
#include <string>
#include <sstream>

#include <stdint.h>

namespace Metabuf
{
    class XmlProtocol;
    class XmlMeta;
    class XmlNode;
    class XmlAttribute;

    typedef bool( *ValueSerialization )(class Xml2Metabuf * _metabuf, const char * _value, void * _user);

    class Xml2Metabuf
    {
    public:
        Xml2Metabuf( const XmlProtocol * _protocol, const XmlMeta * _meta );

    public:
        void initialize();
        void addSerializator( const std::string & _type, ValueSerialization _serializator, void * _user );

    public:
        bool header( void * _binBuff, size_t _binSize, uint32_t _metaVersion, size_t * _writeSize );
        bool convert( void * _binBuff, size_t _binSize, const void * _xmlBuff, size_t _xmlSize, size_t * _writeSize );
        std::string getError() const;

    protected:
        bool writeNode_( const XmlNode * _node, const pugi::xml_node & _xml_node );

        bool writeNodeData_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool writeNodeData2_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeDataSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, uint32_t & _count ) const;

        bool writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool writeNodeAttribute2_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeAttributeSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, uint32_t & _count ) const;

        bool writeNodeSingles_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeSinglesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, uint32_t & _count ) const;

        bool writeNodeIncludes_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeIncludesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, uint32_t & _count ) const;

        bool writeNodeChildren_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeChildrenSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _group, const std::string & _type, uint32_t & _count ) const;

        bool writeNodeGenerators_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeGeneratorSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const XmlNode * _inheritance, uint32_t & _count ) const;

        bool writeNodeDataValue_( const XmlAttribute * _attr, const pugi::xml_attribute & _xml_attr );
        bool writeNodeArgumentValue_( const XmlAttribute * _attr, const pugi::xml_attribute & _xml_attr );

    protected:
        bool hasMetaGenerators_( const XmlNode * _node ) const;
        bool hasMetaChildren_( const XmlNode * _node ) const;
        bool hasMetaIncludes_( const XmlNode * _node ) const;
        bool hasMetaSingles_( const XmlNode * _node ) const;
        bool hasMetaAttribute_( const XmlNode * _node ) const;

    public:
        template<class T>
        void write( const T & _value )
        {
            this->writeBuffer( (const uint8_t *)&_value, sizeof( T ) );
        }

        void writeSize( uint32_t _value );

        template<class T>
        void writeCount( const T * _value, uint32_t _count )
        {
            this->writeBuffer( (const uint8_t *)_value, sizeof( T ) * _count );
        }

        void writeString( const char * _value );

    protected:
        void writeBuffer( const uint8_t * _buff, size_t _size );

    protected:
        const XmlProtocol * m_protocol;
        const XmlMeta * m_meta;

        mutable std::stringstream m_error;

        struct SerializationDesc
        {
            ValueSerialization serialization;
            void * user;
        };

        typedef std::map<std::string, SerializationDesc> MapSerialization;
        MapSerialization m_serialization;

        typedef std::vector<std::string> VectorStringCache;
        VectorStringCache m_stringCache;

        typedef std::vector<uint8_t> VectorBlobject;
        VectorBlobject m_buff;
    };
}