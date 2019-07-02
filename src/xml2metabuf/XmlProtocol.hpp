#pragma once

#include "pugixml.hpp"

#include <string>
#include <map>
#include <vector>

#include <sstream>

#include <stdint.h>

#define METABUF_BIN_VERSION 9

namespace Metabuf
{
    typedef int64_t( *MakeHash )(const void * _data, size_t _len);

    struct XmlEnum
    {
        std::string name;
        std::string write;
        uint32_t index;
    };

    struct XmlType
    {
        typedef std::vector<XmlEnum> TVectorEnumerators;
        TVectorEnumerators enumerators;

        std::string write;
        std::string evict;

        bool is_enumerator;
        bool is_ncr;
        bool is_template;
    };

    typedef std::map<std::string, XmlType> TMapTypes;

    class XmlAttribute
    {
    public:
        std::string getWriteName() const;

    public:
        uint32_t id;
        std::string name;
        std::string type;
        bool required;
        std::string default_value;
    };

    typedef std::map<std::string, XmlAttribute> TMapAttributes;

    class XmlMember
    {
    public:
        std::string getWriteName() const;

    public:
        std::string name;
        TMapAttributes attributes;

    public:
        const XmlAttribute * getAttribute( const std::string & _name ) const;
    };

    typedef std::map<std::string, XmlMember> TMapMembers;

    class XmlChildren
    {
    public:
        std::string group;
        std::string type;
    };

    typedef std::map<std::string, XmlChildren> TMapChildren;

    typedef std::map<std::string, class XmlNode *> TMapNodes;

    struct NoRequiredAttribute
    {
        std::string name;
        uint32_t id;
    };

    typedef std::vector<NoRequiredAttribute> TVectorNoRequiredAttributes;

    class XmlNode
    {
    public:
        XmlNode();
        ~XmlNode();

    public:
        uint32_t id;
        std::string name;
        std::string generator;
        std::string inheritance;

        const XmlNode * node_inheritance;
        const XmlNode * node_scope;

        mutable uint32_t enumerator;
        mutable uint32_t enumeratorNRA;

        TMapAttributes attributes;
        TMapMembers members;
        TMapChildren children;

        TMapNodes singles;
        TMapNodes includes;
        TMapNodes inheritances;
        TMapNodes generators;

        bool noWrite;

    public:
        const XmlAttribute * getAttribute( const std::string & _name ) const;
        const XmlMember * getMember( const std::string & _name ) const;

        const XmlNode * getSingle( const std::string & _name ) const;
        const XmlNode * getInclude( const std::string & _name ) const;
        const XmlNode * getInheritances( const std::string & _name ) const;
        const XmlNode * getGenerator( const std::string & _name ) const;

        void getNoRequiredAttributes( TVectorNoRequiredAttributes & _noRequiredAttributes ) const;
        void getNoRequiredAttributes2( const pugi::xml_node & _xml_node, TVectorNoRequiredAttributes & _noRequiredAttributes ) const;

        const std::string & getName() const;
        std::string getWriteName() const;
        std::string getScope() const;

        bool getNoWrite() const;

    public:
        bool hasNode( const std::string & _type ) const;
        const XmlNode * getNode( const std::string & _type ) const;
    };

    class XmlMeta
    {
    public:
        XmlMeta();
        ~XmlMeta();

    public:
        uint32_t getVersion() const;

    public:
        std::string getWriteName() const;

    public:
        bool hasNode( const std::string & _type ) const;
        const XmlNode * getNode( const std::string & _type ) const;
        const TMapNodes & getNodes() const;

    public:
        std::string m_name;
        uint32_t m_version;

        TMapNodes m_nodes;
    };

    typedef std::map<std::string, XmlMeta *> TMapMetas;

    typedef std::vector<std::string> TVectorInternalStrings;

    class XmlProtocol
    {
    public:
        XmlProtocol();
        ~XmlProtocol();

    public:
        uint32_t getVersion() const;
        uint32_t getCrc32() const;

    public:
        bool hasMeta( const std::string & _type ) const;
        const XmlMeta * getMeta( const std::string & _type ) const;
        const TMapMetas & getMetas() const;

    public:
        void setHashable( MakeHash _hashable );
        MakeHash getHashable() const;

    public:
        const TVectorInternalStrings & getInternals() const;

    public:
        bool hasType( const std::string & _name ) const;
        bool getType( const std::string & _name, XmlType & _type ) const;

    public:
        bool readProtocol( const void * _buff, size_t _size );
        std::string getError() const;

    protected:
        bool readType_( const pugi::xml_node & _xml_node );
        bool readEnum_( const pugi::xml_node & _xml_node );
        bool readMeta_( const pugi::xml_node & _xml_node );
        bool readNode_( XmlMeta * _meta, XmlNode * _node, const pugi::xml_node & _xml_node );

    protected:
        uint32_t m_version;
        uint32_t m_crc32;

        uint32_t m_enumerator;

        MakeHash m_hashable;

        TMapMetas m_metas;

        TMapTypes m_types;
        
        TVectorInternalStrings m_internals;

        std::stringstream m_error;
    };
}