#pragma once

#include "config/Metaconfig.hpp"

#include "metabuf/Metaconvert.hpp"

#include "pugixml.hpp"

#include <vector>
#include <map>
#include <string>
#include <sstream>

#include <stdint.h>

namespace Metabuf
{
    class Protocol;
    class Meta;
    class Node;
    class Attribute;

    typedef bool( *ValueSerialization )(class MetabufWriter * _metabuf, const char * _value, void * _user);

    class MetabufWriter
    {
    public:
        MetabufWriter( const ProtocolInterface * _protocol, const MetaInterface * _meta );

    public:
        void initialize();
        void addSerializator( const std::string & _type, ValueSerialization _serializator, void * _user );

    public:
        bool write( OutputAdapter & _output, const pugi::xml_node & _dataNode, const NodeInterface * _protocolNode );

        template<class T>
        bool write( T & _output, const pugi::xml_node & _dataNode, const NodeInterface * _protocolNode )
        {
            OutputAdapter output( _output );

            if( this->write( output, _dataNode, _protocolNode ) == false )
            {
                return false;
            }

            return true;
        }

        std::string getError() const;

    protected:
        bool writeNode_( const Node * _node, const pugi::xml_node & _xml_node );

        bool writeNodeData_( const Node * _node, const pugi::xml_node & _xml_node );
        bool writeNodeData2_( const Node * _node, const pugi::xml_node & _xml_node );
        bool getNodeDataSize_( const Node * _node, const pugi::xml_node & _xml_node, uint32_t & _count ) const;

        bool writeNodeAttribute_( const Node * _node, const pugi::xml_node & _xml_node );
        bool writeNodeAttribute2_( const Node * _node, const pugi::xml_node & _xml_node );
        bool getNodeAttributeSize_( const Node * _node, const pugi::xml_node & _xml_node, uint32_t & _count ) const;

        bool writeNodeSingles_( const Node * _node, const pugi::xml_node & _xml_node );
        bool getNodeSinglesSize_( const Node * _node, const pugi::xml_node & _xml_node, const std::string & _type, uint32_t & _count ) const;

        bool writeNodeIncludes_( const Node * _node, const pugi::xml_node & _xml_node );
        bool getNodeIncludesSize_( const Node * _node, const pugi::xml_node & _xml_node, const std::string & _type, uint32_t & _count ) const;

        bool writeNodeChildren_( const Node * _node, const pugi::xml_node & _xml_node );
        bool getNodeChildrenSize_( const Node * _node, const pugi::xml_node & _xml_node, const std::string & _group, const std::string & _type, uint32_t & _count ) const;

        bool writeNodeGenerators_( const Node * _node, const pugi::xml_node & _xml_node );
        bool getNodeGeneratorSize_( const Node * _node, const pugi::xml_node & _xml_node, const Node * _inheritance, uint32_t & _count ) const;

        bool writeNodeDataValue_( const Attribute * _attr, const pugi::xml_attribute & _xml_attr );
        bool writeNodeArgumentValue_( const Attribute * _attr, const pugi::xml_attribute & _xml_attr );

    protected:
        bool hasMetaGenerators_( const Node * _node ) const;
        bool hasMetaChildren_( const Node * _node ) const;
        bool hasMetaIncludes_( const Node * _node ) const;
        bool hasMetaSingles_( const Node * _node ) const;
        bool hasMetaAttribute_( const Node * _node ) const;

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
        const Protocol * m_protocol;
        const Meta * m_meta;

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
