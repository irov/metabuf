#pragma once

#include "metabuf/Metaconvert.hpp"

#include "pugixml.hpp"

#include <string>
#include <string_view>
#include <map>
#include <vector>

#include <sstream>

#include <stddef.h>
#include <stdint.h>

namespace Metabuf
{
    struct Enum
    {
        std::string name;
        std::string write;
        uint32_t index;
    };

    struct Type
        : public TypeInterface
    {
        typedef std::vector<Enum> TVectorEnumerators;
        TVectorEnumerators enumerators;

        std::string write;
        std::string evict;

        bool is_enumerator;
        bool is_ncr;
        bool is_template;

    public:
        void addEnumerator( const std::string & _name, const std::string & _write, uint32_t _index ) override;
    };

    typedef std::map<std::string, Type, std::less<>> TMapTypes;

    class Attribute
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

    typedef std::map<std::string, Attribute, std::less<>> TMapAttributes;

    class Member
    {
    public:
        std::string getWriteName() const;

    public:
        std::string name;
        TMapAttributes attributes;

    public:
        const Attribute * getAttribute( std::string_view _name ) const;
    };

    typedef std::map<std::string, Member, std::less<>> TMapMembers;

    class Children
    {
    public:
        std::string group;
        std::string type;
    };

    typedef std::map<std::string, Children, std::less<>> TMapChildren;

    typedef std::map<std::string, class Node *, std::less<>> TMapNodes;

    struct NoRequiredAttribute
    {
        std::string name;
        uint32_t id;
    };

    typedef std::vector<NoRequiredAttribute> TVectorNoRequiredAttributes;

    class Node
        : public NodeInterface
    {
    public:
        Node();
        Node( uint32_t _id, const std::string & _name, const std::string & _generator, const std::string & _inheritance, uint32_t _enumerator, uint32_t _enumeratorNRA, bool _noWrite, const Node * _node_scope );
        ~Node() override;

    public:
        void * operator new (size_t _size);
        void operator delete (void * _ptr, size_t _size);

    public:
        uint32_t id;
        std::string name;
        std::string generator;
        std::string inheritance;

        const Node * node_inheritance;
        const Node * node_scope;

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
        const std::string & getName() const override;
        const std::string & getGenerator() const override;
        const Node * getScopeNode() const override;

        const std::string * getAttributeType( std::string_view _name ) const override;
        bool hasMember( std::string_view _name ) const override;
        const std::string * getMemberAttributeType( std::string_view _member, std::string_view _name ) const override;

        const Attribute * getAttribute( std::string_view _name ) const;
        const Member * getMember( std::string_view _name ) const;

        const Node * getSingle( std::string_view _name ) const override;
        const Node * getInclude( std::string_view _name ) const override;
        const Node * getInheritances( std::string_view _name ) const override;
        const Node * getGenerator( std::string_view _name ) const override;

        const std::string * getChildrenType( std::string_view _group ) const override;

    public:
        void setInheritance( const NodeInterface * _inheritance ) override;
        void addAttribute( uint32_t _id, const std::string & _name, const std::string & _type, bool _required, const std::string & _defaultValue ) override;
        void addMemberAttribute( const std::string & _member, uint32_t _id, const std::string & _name, const std::string & _type, bool _required, const std::string & _defaultValue ) override;
        void addChildren( const std::string & _name, const std::string & _group, const std::string & _type ) override;

        void addSingle( NodeInterface * _single ) override;
        void addInclude( NodeInterface * _include ) override;
        void addInheritance( NodeInterface * _inheritance ) override;
        void addGenerator( NodeInterface * _generator ) override;

        void getNoRequiredAttributes( TVectorNoRequiredAttributes & _noRequiredAttributes ) const;
        void getNoRequiredAttributes2( const pugi::xml_node & _xml_node, TVectorNoRequiredAttributes & _noRequiredAttributes ) const;

        std::string getWriteName() const;
        std::string getScope() const;

        bool getNoWrite() const;

    public:
        bool hasNode( std::string_view _type ) const;
        const Node * getNode( std::string_view _type ) const;
    };

    class Meta
        : public MetaInterface
    {
    public:
        Meta();
        ~Meta() override;

    public:
        void * operator new (size_t _size);
        void operator delete (void * _ptr, size_t _size);

    public:
        uint32_t getVersion() const override;

    public:
        std::string getWriteName() const;

    public:
        bool hasNode( std::string_view _type ) const;
        const Node * getNode( std::string_view _type ) const override;
        const TMapNodes & getNodes() const;

    public:
        void addNode( NodeInterface * _node ) override;

    public:
        std::string m_name;
        uint32_t m_version;

        TMapNodes m_nodes;
    };

    typedef std::map<std::string, Meta *, std::less<>> TMapMetas;

    typedef std::vector<std::string> TVectorInternalStrings;

    class Protocol
        : public ProtocolInterface
    {
    public:
        Protocol();
        ~Protocol() override;

    public:
        void * operator new (size_t _size);
        void operator delete (void * _ptr, size_t _size);

    public:
        void finalize();

    public:
        uint32_t getVersion() const override;
        uint32_t getCrc32() const override;

    public:
        void setVersion( uint32_t _version, uint32_t _crc32 ) override;

        TypeInterface * addType( const std::string & _name, const std::string & _write, const std::string & _evict, bool _enumerator, bool _ncr, bool _template ) override;
        MetaInterface * addMeta( const std::string & _name, uint32_t _version ) override;
        NodeInterface * addNode( uint32_t _id, const std::string & _name, const std::string & _generator, const std::string & _inheritance, uint32_t _enumerator, uint32_t _enumeratorNRA, bool _noWrite, const NodeInterface * _scope ) override;
        void addInternal( const std::string & _internal ) override;

    public:
        bool hasMeta( std::string_view _type ) const;
        const Meta * getMeta( std::string_view _type ) const override;
        const TMapMetas & getMetas() const;

    public:
        const TVectorInternalStrings & getInternals() const;

    public:
        bool hasType( std::string_view _name ) const;
        bool getType( std::string_view _name, Type & _type ) const;
        const TMapTypes & getTypes() const;

    public:
        bool readProtocol( const void * _buff, size_t _size );
        std::string getError() const;

    protected:
        void clear_();

    protected:
        bool readType_( const pugi::xml_node & _xml_node );
        bool readEnum_( const pugi::xml_node & _xml_node );
        bool readMeta_( const pugi::xml_node & _xml_node );
        bool readNode_( Meta * _meta, Node * _node, const pugi::xml_node & _xml_node );

    protected:
        uint32_t m_version;
        uint32_t m_crc32;

        uint32_t m_enumerator;

        TMapMetas m_metas;

        TMapTypes m_types;

        TVectorInternalStrings m_internals;

        std::stringstream m_error;
    };
}
