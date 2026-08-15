#include "metaconverter/Protocol.hpp"
#include "metaconverter/XmlCRC32.hpp"

#include "config/Metaconfig.hpp"

#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <utility>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    void Type::addEnumerator( const std::string & _name, const std::string & _write, uint32_t _index )
    {
        this->enumerators.emplace_back( Enum{_name, _write, _index} );
    }
    //////////////////////////////////////////////////////////////////////////
    std::string Attribute::getWriteName() const
    {
        std::string write_name;
        write_name = "m_" + this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string Member::getWriteName() const
    {
        std::string write_name;
        write_name = "m_" + this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    const Attribute * Member::getAttribute( std::string_view _name ) const
    {
        TMapAttributes::const_iterator it_found = this->attributes.find( _name );

        if( it_found == this->attributes.end() )
        {
            return nullptr;
        }

        const Attribute * attr = &it_found->second;

        return attr;
    }
    //////////////////////////////////////////////////////////////////////////
    Node::Node()
        : id( 0 )
        , node_inheritance( nullptr )
        , node_scope( nullptr )
        , enumerator( 0 )
        , enumeratorNRA( 0 )
        , noWrite( false )
    {

    }
    //////////////////////////////////////////////////////////////////////////
    Node::Node( uint32_t _id, const std::string & _name, const std::string & _generator, const std::string & _inheritance, uint32_t _enumerator, uint32_t _enumeratorNRA, bool _noWrite, const Node * _node_scope )
        : id( _id )
        , name( _name )
        , generator( _generator )
        , inheritance( _inheritance )
        , node_inheritance( nullptr )
        , node_scope( _node_scope )
        , enumerator( _enumerator )
        , enumeratorNRA( _enumeratorNRA )
        , noWrite( _noWrite )
    {

    }
    //////////////////////////////////////////////////////////////////////////
    Node::~Node()
    {
        for( TMapNodes::const_iterator
            it = includes.begin(),
            it_end = includes.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            delete node;
        }

        for( TMapNodes::const_iterator
            it = inheritances.begin(),
            it_end = inheritances.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            delete node;
        }

        for( TMapNodes::const_iterator
            it = generators.begin(),
            it_end = generators.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            delete node;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void * Node::operator new (size_t _size)
    {
        return METABUF_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::operator delete (void * _ptr, size_t _size)
    {
        METABUF_UNUSED( _size );

        METABUF_FREE( _ptr, _size );
    }
    //////////////////////////////////////////////////////////////////////////
    const std::string & Node::getGenerator() const
    {
        return this->generator;
    }
    //////////////////////////////////////////////////////////////////////////
    const Node * Node::getScopeNode() const
    {
        return this->node_scope;
    }
    //////////////////////////////////////////////////////////////////////////
    const std::string * Node::getAttributeType( std::string_view _name ) const
    {
        const Attribute * attribute = this->getAttribute( _name );

        if( attribute == nullptr )
        {
            return nullptr;
        }

        const std::string * type = &attribute->type;

        return type;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::hasMember( std::string_view _name ) const
    {
        const Member * member = this->getMember( _name );

        return member != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    const std::string * Node::getMemberAttributeType( std::string_view _member, std::string_view _name ) const
    {
        const Member * member = this->getMember( _member );

        if( member == nullptr )
        {
            return nullptr;
        }

        const Attribute * attribute = member->getAttribute( _name );

        if( attribute == nullptr )
        {
            return nullptr;
        }

        const std::string * type = &attribute->type;

        return type;
    }
    //////////////////////////////////////////////////////////////////////////
    const Attribute * Node::getAttribute( std::string_view _name ) const
    {
        TMapAttributes::const_iterator it_found = this->attributes.find( _name );

        if( it_found == this->attributes.end() )
        {
            if( this->node_inheritance == nullptr )
            {
                return nullptr;
            }
            else
            {
                const Attribute * attr_inheritance = this->node_inheritance->getAttribute( _name );

                return attr_inheritance;
            }
        }

        const Attribute * attr = &it_found->second;

        return attr;
    }
    //////////////////////////////////////////////////////////////////////////
    const Member * Node::getMember( std::string_view _name ) const
    {
        TMapMembers::const_iterator it_found = this->members.find( _name );

        if( it_found == this->members.end() )
        {
            if( this->node_inheritance == nullptr )
            {
                return nullptr;
            }
            else
            {
                const Member * member_inheritance = this->node_inheritance->getMember( _name );

                return member_inheritance;
            }
        }

        const Member * member = &it_found->second;

        return member;
    }
    //////////////////////////////////////////////////////////////////////////
    const Node * Node::getSingle( std::string_view _name ) const
    {
        TMapNodes::const_iterator it_found = this->singles.find( _name );

        if( it_found == this->singles.end() )
        {
            return nullptr;
        }

        Node * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const Node * Node::getInclude( std::string_view _name ) const
    {
        TMapNodes::const_iterator it_found = this->includes.find( _name );

        if( it_found == this->includes.end() )
        {
            return nullptr;
        }

        Node * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const Node * Node::getInheritances( std::string_view _name ) const
    {
        TMapNodes::const_iterator it_found = this->inheritances.find( _name );

        if( it_found == this->inheritances.end() )
        {
            return nullptr;
        }

        Node * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const Node * Node::getGenerator( std::string_view _name ) const
    {
        TMapNodes::const_iterator it_found = this->generators.find( _name );

        if( it_found == this->generators.end() )
        {
            return nullptr;
        }

        const Node * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const std::string * Node::getChildrenType( std::string_view _group ) const
    {
        for( TMapChildren::const_iterator
            it = this->children.begin(),
            it_end = this->children.end();
            it != it_end;
            ++it )
        {
            if( it->second.group == _group )
            {
                const std::string * type = &it->second.type;

                return type;
            }
        }

        if( this->node_inheritance == nullptr )
        {
            return nullptr;
        }

        const std::string * type = this->node_inheritance->getChildrenType( _group );

        return type;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::setInheritance( const NodeInterface * _inheritance )
    {
        const Node * inheritance = static_cast<const Node *>(_inheritance);

        this->node_inheritance = inheritance;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addAttribute( uint32_t _id, const std::string & _name, const std::string & _type, bool _required, const std::string & _defaultValue )
    {
        Attribute attribute{_id, _name, _type, _required, _defaultValue};

        this->attributes.emplace( _name, std::move( attribute ) );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addMemberAttribute( const std::string & _member, uint32_t _id, const std::string & _name, const std::string & _type, bool _required, const std::string & _defaultValue )
    {
        Member & member = this->members[_member];
        member.name = _member;

        Attribute attribute{_id, _name, _type, _required, _defaultValue};

        member.attributes.emplace( _name, std::move( attribute ) );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addChildren( const std::string & _name, const std::string & _group, const std::string & _type )
    {
        Children children{_group, _type};

        this->children.emplace( _name, std::move( children ) );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addSingle( NodeInterface * _single )
    {
        Node * single = static_cast<Node *>(_single);

        this->singles.emplace( single->name, single );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addInclude( NodeInterface * _include )
    {
        Node * include = static_cast<Node *>(_include);

        this->includes.emplace( include->name, include );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addInheritance( NodeInterface * _inheritance )
    {
        Node * inheritance = static_cast<Node *>(_inheritance);

        this->inheritances.emplace( inheritance->name, inheritance );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addGenerator( NodeInterface * _generator )
    {
        Node * generator = static_cast<Node *>(_generator);

        this->generators.emplace( generator->name, generator );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::getNoRequiredAttributes( TVectorNoRequiredAttributes & _noRequiredAttributes ) const
    {
        for( TMapAttributes::const_iterator
            it_attributes = this->attributes.begin(),
            it_attributes_end = this->attributes.end();
            it_attributes != it_attributes_end;
            ++it_attributes )
        {
            const Attribute * attr = &it_attributes->second;

            if( attr->required == true )
            {
                continue;
            }

            NoRequiredAttribute nra;
            nra.name = attr->name;
            nra.id = attr->id;

            _noRequiredAttributes.push_back( nra );
        }

        for( TMapMembers::const_iterator
            it_members = this->members.begin(),
            it_members_end = this->members.end();
            it_members != it_members_end;
            ++it_members )
        {
            const Member * member = &it_members->second;

            for( TMapAttributes::const_iterator
                it_attributes = member->attributes.begin(),
                it_attributes_end = member->attributes.end();
                it_attributes != it_attributes_end;
                ++it_attributes )
            {
                const Attribute * attr = &it_attributes->second;

                if( attr->required == true )
                {
                    continue;
                }

                NoRequiredAttribute nra;
                nra.name = member->name + "_" + attr->name;;
                nra.id = attr->id;

                _noRequiredAttributes.push_back( nra );
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////        
    void Node::getNoRequiredAttributes2( const pugi::xml_node & _xml_node, TVectorNoRequiredAttributes & _noRequiredAttributes ) const
    {
        for( TMapAttributes::const_iterator
            it = this->attributes.begin(),
            it_end = this->attributes.end();
            it != it_end;
            ++it )
        {
            const Attribute * attr = &it->second;

            if( attr->required == true )
            {
                continue;
            }

            pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

            if( xml_attr.empty() == true )
            {
                continue;
            }

            NoRequiredAttribute nra;
            nra.name = attr->name;
            nra.id = attr->id;

            _noRequiredAttributes.push_back( nra );
        }

        if( this->inheritance.empty() == false )
        {
            for( TMapAttributes::const_iterator
                it = this->node_inheritance->attributes.begin(),
                it_end = this->node_inheritance->attributes.end();
                it != it_end;
                ++it )
            {
                const Attribute * attr = &it->second;

                if( attr->required == true )
                {
                    continue;
                }

                pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

                if( xml_attr.empty() == true )
                {
                    continue;
                }

                NoRequiredAttribute nra;
                nra.name = attr->name;
                nra.id = attr->id;

                _noRequiredAttributes.push_back( nra );
            }
        }

        for( TMapMembers::const_iterator
            it = this->members.begin(),
            it_end = this->members.end();
            it != it_end;
            ++it )
        {
            const Member * member = &it->second;

            for( TMapAttributes::const_iterator
                it_attributes = member->attributes.begin(),
                it_attributes_end = member->attributes.end();
                it_attributes != it_attributes_end;
                ++it_attributes )
            {
                const Attribute * attr = &it_attributes->second;

                if( attr->required == true )
                {
                    continue;
                }

                for( pugi::xml_node::iterator
                    it_xml = _xml_node.begin(),
                    it_xml_end = _xml_node.end();
                    it_xml != it_xml_end;
                    ++it_xml )
                {
                    const pugi::xml_node & child = *it_xml;

                    const char * child_name = child.name();

                    if( member->name != child_name )
                    {
                        continue;
                    }

                    pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

                    if( xml_attr.empty() == true )
                    {
                        continue;
                    }

                    NoRequiredAttribute nra;
                    nra.name = member->name + "_" + attr->name;;
                    nra.id = attr->id;

                    _noRequiredAttributes.push_back( nra );

                    break;
                }
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    const std::string & Node::getName() const
    {
        return this->name;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string Node::getWriteName() const
    {
        std::string write_name;
        write_name += "Meta_" + this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string Node::getScope() const
    {
        std::string write_scope;

        if( this->node_scope == nullptr )
        {
            write_scope += this->getWriteName();
            return write_scope;
        }

        write_scope += this->node_scope->getScope();
        write_scope += "::";
        write_scope += this->getWriteName();

        return write_scope;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::getNoWrite() const
    {
        return noWrite;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::hasNode( std::string_view _type ) const
    {
        TMapNodes::const_iterator it_singles_found = singles.find( _type );

        if( it_singles_found != singles.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = singles.begin(),
            it_end = singles.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        TMapNodes::const_iterator it_includes_found = includes.find( _type );

        if( it_includes_found != includes.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = includes.begin(),
            it_end = includes.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        TMapNodes::const_iterator it_inheritances_found = inheritances.find( _type );

        if( it_inheritances_found != inheritances.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = inheritances.begin(),
            it_end = inheritances.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    const Node * Node::getNode( std::string_view _type ) const
    {
        TMapNodes::const_iterator it_singles_found = singles.find( _type );

        if( it_singles_found != singles.end() )
        {
            const Node * node = it_singles_found->second;

            return node;
        }

        for( TMapNodes::const_iterator
            it = singles.begin(),
            it_end = singles.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            const Node * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        TMapNodes::const_iterator it_includes_found = includes.find( _type );

        if( it_includes_found != includes.end() )
        {
            const Node * node = it_includes_found->second;

            return node;
        }

        for( TMapNodes::const_iterator
            it = includes.begin(),
            it_end = includes.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            const Node * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        TMapNodes::const_iterator it_inheritances_found = inheritances.find( _type );

        if( it_inheritances_found != inheritances.end() )
        {
            const Node * node = it_inheritances_found->second;

            return node;
        }

        for( TMapNodes::const_iterator
            it = inheritances.begin(),
            it_end = inheritances.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            const Node * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        return nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    Meta::Meta()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Meta::~Meta()
    {
        for( TMapNodes::const_iterator
            it = m_nodes.begin(),
            it_end = m_nodes.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            delete node;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void * Meta::operator new (size_t _size)
    {
        return METABUF_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta::operator delete (void * _ptr, size_t _size)
    {
        METABUF_UNUSED( _size );

        METABUF_FREE( _ptr, _size );
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t Meta::getVersion() const
    {
        return this->m_version;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string Meta::getWriteName() const
    {
        std::string write_name;
        write_name += "Meta_" + this->m_name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta::hasNode( std::string_view _type ) const
    {
        TMapNodes::const_iterator it_found = m_nodes.find( _type );

        if( it_found != m_nodes.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = m_nodes.begin(),
            it_end = m_nodes.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    const Node * Meta::getNode( std::string_view _type ) const
    {
        TMapNodes::const_iterator it_found = m_nodes.find( _type );

        if( it_found != m_nodes.end() )
        {
            const Node * found_node = it_found->second;

            return found_node;
        }

        for( TMapNodes::const_iterator
            it = m_nodes.begin(),
            it_end = m_nodes.end();
            it != it_end;
            ++it )
        {
            const Node * node = it->second;

            const Node * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        return nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    const TMapNodes & Meta::getNodes() const
    {
        return m_nodes;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta::addNode( NodeInterface * _node )
    {
        Node * node = static_cast<Node *>(_node);

        m_nodes.emplace( node->name, node );
    }
    //////////////////////////////////////////////////////////////////////////
    Protocol::Protocol()
        : m_enumerator( 0 )
        , m_version( 0 )
        , m_crc32( 0 )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Protocol::~Protocol()
    {
        this->clear_();
    }
    //////////////////////////////////////////////////////////////////////////
    void * Protocol::operator new (size_t _size)
    {
        return METABUF_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Protocol::operator delete (void * _ptr, size_t _size)
    {
        METABUF_UNUSED( _size );

        METABUF_FREE( _ptr, _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Protocol::finalize()
    {
        this->clear_();
    }
    //////////////////////////////////////////////////////////////////////////
    void Protocol::clear_()
    {
        for( TMapMetas::const_iterator
            it = m_metas.begin(),
            it_end = m_metas.end();
            it != it_end;
            ++it )
        {
            const Meta * meta = it->second;

            delete meta;
        }

        m_metas.clear();
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t Protocol::getVersion() const
    {
        return m_version;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t Protocol::getCrc32() const
    {
        return m_crc32;
    }
    //////////////////////////////////////////////////////////////////////////
    void Protocol::setVersion( uint32_t _version, uint32_t _crc32 )
    {
        this->clear_();

        m_types.clear();
        m_internals.clear();

        m_version = _version;
        m_crc32 = _crc32;
    }
    //////////////////////////////////////////////////////////////////////////
    TypeInterface * Protocol::addType( const std::string & _name, const std::string & _write, const std::string & _evict, bool _enumerator, bool _ncr, bool _template )
    {
        Type type;
        type.write = _write;
        type.evict = _evict;
        type.is_enumerator = _enumerator;
        type.is_ncr = _ncr;
        type.is_template = _template;

        std::pair<TMapTypes::iterator, bool> result = m_types.emplace( _name, std::move( type ) );
        Type * type_ptr = &result.first->second;

        return type_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    MetaInterface * Protocol::addMeta( const std::string & _name, uint32_t _version )
    {
        Meta * meta = new Meta();
        meta->m_name = _name;
        meta->m_version = _version;

        m_metas.emplace( _name, meta );

        return meta;
    }
    //////////////////////////////////////////////////////////////////////////
    NodeInterface * Protocol::addNode( uint32_t _id, const std::string & _name, const std::string & _generator, const std::string & _inheritance, uint32_t _enumerator, uint32_t _enumeratorNRA, bool _noWrite, const NodeInterface * _scope )
    {
        const Node * scope = static_cast<const Node *>(_scope);

        Node * node = new Node( _id, _name, _generator, _inheritance, _enumerator, _enumeratorNRA, _noWrite, scope );

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    void Protocol::addInternal( const std::string & _internal )
    {
        m_internals.emplace_back( _internal );
    }
    //////////////////////////////////////////////////////////////////////////
    std::string Protocol::getError() const
    {
        const std::string error = m_error.str();

        return error;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::readProtocol( const void * _buff, size_t _size )
    {
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load_buffer( _buff, _size );

        if( result == false )
        {
            m_error << "Protocol::readProtocol:" << result.description() << std::endl;

            return false;
        }

        pugi::xml_node root = doc.document_element();

        if( root.empty() == true )
        {
            return true;
        }

        pugi::xml_attribute xml_version = root.attribute( "Version" );

        const char * version_value = xml_version.value();

        if( sscanf( version_value, "%u", &m_version ) != 1 )
        {
            m_error << "Protocol::readProtocol: not set version!" << std::endl;

            return false;
        }

        for( pugi::xml_node::iterator
            it = root.begin(),
            it_end = root.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Type" ) == 0 )
            {
                if( this->readType_( element ) == false )
                {
                    return false;
                }
            }
        }

        for( pugi::xml_node::iterator
            it = root.begin(),
            it_end = root.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Enum" ) == 0 )
            {
                if( this->readEnum_( element ) == false )
                {
                    return false;
                }
            }
        }

        for( pugi::xml_node::iterator
            it = root.begin(),
            it_end = root.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Meta" ) == 0 )
            {
                if( this->readMeta_( element ) == false )
                {
                    return false;
                }
            }
        }

        XmlCRC32 crc32_writer;
        doc.save( crc32_writer );

        m_crc32 = crc32_writer.getCrc32();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::readEnum_( const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute Name = _xml_node.attribute( "Name" );
        pugi::xml_attribute TypeAttribute = _xml_node.attribute( "Type" );
        pugi::xml_attribute Template = _xml_node.attribute( "Template" );

        if( Name.empty() == true )
        {
            m_error << "Protocol::readEnum_: Name not set" << std::endl;

            return false;
        }

        const char * type_name = Name.value();
        const char * type_write = TypeAttribute.value();


        Type type;
        type.is_enumerator = true;
        type.write = (strlen( type_write ) == 0) ? type_name : type_write;
        type.evict = "uint32_t";

        if( Template.empty() == false )
        {
            const char * type_Template = Template.value();

            uint32_t type_Template_value;
            if( sscanf( type_Template, "%d", &type_Template_value ) != 1 )
            {
                m_error << "Protocol::readEnum_: Template invalid" << std::endl;

                return false;
            }

            type.is_template = (type_Template_value != 0);
        }
        else
        {
            type.is_template = false;
        }

        uint32_t enum_index = 0;

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            Enum e;
            e.write = element_name;

            pugi::xml_attribute EnumName = element.attribute( "Name" );

            if( EnumName.empty() == true )
            {
                e.name = element_name;
            }
            else
            {
                const char * enum_name_value = EnumName.value();

                e.name = enum_name_value;
            }

            pugi::xml_attribute EnumIndex = element.attribute( "Index" );

            if( EnumIndex.empty() == false )
            {
                const char * enum_index_value = EnumIndex.value();

                uint32_t value;
                if( sscanf( enum_index_value, "%u", &value ) != 1 )
                {
                    m_error << "Protocol::readEnum_: Enum '" << type_name << "' invalid index '" << enum_index_value << "'" << std::endl;

                    return false;
                }

                enum_index = value;
            }

            e.index = enum_index;
            ++enum_index;

            type.enumerators.push_back( e );
        }

        m_types.insert( std::make_pair( type_name, type ) );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::readType_( const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute Name = _xml_node.attribute( "Name" );
        pugi::xml_attribute TypeAttribute = _xml_node.attribute( "Type" );
        pugi::xml_attribute Evict = _xml_node.attribute( "Evict" );
        pugi::xml_attribute NCR = _xml_node.attribute( "NCR" );
        pugi::xml_attribute Template = _xml_node.attribute( "Template" );

        if( Name.empty() == true || Evict.empty() == true )
        {
            m_error << "Protocol::readType_: Name or Evict not set" << std::endl;

            return false;
        }

        const char * type_name = Name.value();
        const char * type_write = TypeAttribute.value();
        const char * type_evict = Evict.value();

        Type type;
        type.is_enumerator = false;
        type.write = (strlen( type_write ) == 0) ? type_name : type_write;
        type.evict = type_evict;

        if( NCR.empty() == false )
        {
            const char * type_NCR = NCR.value();

            uint32_t type_NCR_value;
            if( sscanf( type_NCR, "%d", &type_NCR_value ) != 1 )
            {
                m_error << "Protocol::readType_: NCR invalid" << std::endl;

                return false;
            }

            type.is_ncr = (type_NCR_value != 0);
        }
        else
        {
            type.is_ncr = false;
        }

        if( Template.empty() == false )
        {
            const char * type_Template = Template.value();

            uint32_t type_Template_value;
            if( sscanf( type_Template, "%d", &type_Template_value ) != 1 )
            {
                m_error << "Protocol::readType_: Template invalid" << std::endl;

                return false;
            }

            type.is_template = (type_Template_value != 0);
        }
        else
        {
            type.is_template = false;
        }

        m_types.insert( std::make_pair( type_name, type ) );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::readMeta_( const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute MetaName = _xml_node.attribute( "Name" );
        pugi::xml_attribute MetaVersion = _xml_node.attribute( "Version" );

        Meta * metaXml = new Meta();

        metaXml->m_name = MetaName.as_string();
        metaXml->m_version = MetaVersion.as_uint();

        m_metas.insert( std::make_pair( metaXml->m_name, metaXml ) );

        for( const pugi::xml_node & element : _xml_node )
        {
            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Node" ) == 0 )
            {
                if( this->readNode_( metaXml, nullptr, element ) == false )
                {
                    return false;
                }
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::readNode_( Meta * _meta, Node * _node, const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute NodeName = _xml_node.attribute( "Name" );
        pugi::xml_attribute NodeGenerator = _xml_node.attribute( "Generator" );
        pugi::xml_attribute NodeInheritance = _xml_node.attribute( "Inheritance" );
        pugi::xml_attribute NodeNoWrite = _xml_node.attribute( "NoWrite" );
        pugi::xml_attribute NodeSingle = _xml_node.attribute( "Single" );

        Node * nodeXml = new Node();

        if( NodeNoWrite.empty() == false )
        {
            nodeXml->noWrite = true;
        }

        if( _node == nullptr )
        {
            nodeXml->id = ++m_enumerator;
        }
        else
        {
            nodeXml->id = ++_node->enumerator;
        }

        nodeXml->enumerator = 0;
        nodeXml->enumeratorNRA = 0;
        nodeXml->node_inheritance = nullptr;
        nodeXml->node_scope = _node;
        nodeXml->name = NodeName.value();

        if( NodeGenerator.empty() == false )
        {
            nodeXml->generator = NodeGenerator.value();
        }

        if( NodeInheritance.empty() == false )
        {
            nodeXml->inheritance = NodeInheritance.value();

            nodeXml->node_inheritance = _node->getInheritances( nodeXml->inheritance );

            if( nodeXml->node_inheritance == nullptr )
            {
                m_error << "Protocol::readNode_: node " << NodeName.value() << " not found inheritance " << NodeInheritance.value() << std::endl;

                return false;
            }

            nodeXml->enumerator = nodeXml->node_inheritance->enumerator;
            nodeXml->enumeratorNRA = nodeXml->node_inheritance->enumeratorNRA;

            m_internals.emplace_back( nodeXml->name );
        }

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( strcmp( element_name, "Attribute" ) == 0 )
            {
                pugi::xml_attribute AttributeName = element.attribute( "Name" );
                pugi::xml_attribute AttributeType = element.attribute( "Type" );
                pugi::xml_attribute AttributeRequired = element.attribute( "Required" );
                pugi::xml_attribute AttributeDefault = element.attribute( "Default" );

                Attribute & attributeXml = nodeXml->attributes[AttributeName.value()];

                attributeXml.name = AttributeName.value();
                attributeXml.type = AttributeType.value();
                attributeXml.required = AttributeRequired.empty() == false;
                attributeXml.default_value = AttributeDefault.value();

                if( attributeXml.required == false )
                {
                    attributeXml.id = nodeXml->enumeratorNRA++;

                    if( attributeXml.id >= 32 )
                    {
                        m_error << "Protocol::readNode_: Attribute " << NodeName.value() << " id more 32" << std::endl;

                        return false;
                    }
                }
                else
                {
                    attributeXml.id = 0;
                }

                if( attributeXml.required == true && attributeXml.default_value.empty() == false )
                {
                    m_error << "Protocol::readNode_: Attribute " << NodeName.value() << " incorrect together 'Required' and 'Default' modify" << std::endl;

                    return false;
                }

                if( this->hasType( attributeXml.type ) == false )
                {
                    m_error << "Protocol::readNode_: Attribute " << NodeName.value() << " not found type " << AttributeType.value() << std::endl;

                    return false;
                }
            }
            else if( strcmp( element_name, "Member" ) == 0 )
            {
                pugi::xml_attribute MemberNode = element.attribute( "Node" );
                pugi::xml_attribute MemberName = element.attribute( "Name" );
                pugi::xml_attribute MemberType = element.attribute( "Type" );
                //pugi::xml_attribute Evict = element.attribute("Evict");
                pugi::xml_attribute MemberRequired = element.attribute( "Required" );
                pugi::xml_attribute AttributeDefault = element.attribute( "Default" );

                Member & memberXml = nodeXml->members[MemberNode.value()];

                memberXml.name = MemberNode.value();

                Attribute & attributeXml = memberXml.attributes[MemberName.value()];

                attributeXml.name = MemberName.value();
                attributeXml.type = MemberType.value();
                attributeXml.required = MemberRequired.empty() == false;
                attributeXml.default_value = AttributeDefault.value();

                if( attributeXml.required == false )
                {
                    attributeXml.id = nodeXml->enumeratorNRA++;

                    if( attributeXml.id >= 32 )
                    {
                        m_error << "Protocol::readNode_: Attribute " << NodeName.value() << " id more 32" << std::endl;

                        return false;
                    }
                }
                else
                {
                    attributeXml.id = 0;
                }

                if( attributeXml.required == true && attributeXml.default_value.empty() == false )
                {
                    m_error << "Protocol::readNode_: Attribute " << NodeName.value() << " incorrect together 'Required' and 'Default' modify" << std::endl;

                    return false;
                }

                if( this->hasType( attributeXml.type ) == false )
                {
                    m_error << "Protocol::readNode_: Member " << NodeName.value() << " not found type " << MemberType.value() << std::endl;

                    return false;
                }
            }
            else if( strcmp( element_name, "Children" ) == 0 )
            {
                pugi::xml_attribute ChildrenGroup = element.attribute( "Group" );
                pugi::xml_attribute ChildrenType = element.attribute( "Type" );

                const char * ChildrenGroupValue = ChildrenGroup.value();

                Children & childrenXml = nodeXml->children[ChildrenGroupValue];

                childrenXml.group = ChildrenGroup.value();
                childrenXml.type = ChildrenType.value();

                if( _meta->hasNode( childrenXml.type ) == false )
                {
                    m_error << "Protocol::readNode_: Children " << NodeName.value() << " not found type " << ChildrenType.value() << std::endl;

                    return false;
                }
            }
            else if( strcmp( element_name, "Node" ) == 0 )
            {
                if( this->readNode_( _meta, nodeXml, element ) == false )
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        if( _node == nullptr )
        {
            _meta->m_nodes.insert( std::make_pair( nodeXml->name, nodeXml ) );
        }
        else
        {
            if( NodeGenerator.empty() == false )
            {
                if( NodeSingle.empty() == false && strcmp( NodeSingle.value(), "1" ) == 0 )
                {
                    m_error << "Protocol::readNode_: node " << nodeXml->name << " inheritances no support single" << std::endl;

                    return false;
                }

                _node->inheritances.insert( std::make_pair( nodeXml->name, nodeXml ) );
            }
            else if( NodeInheritance.empty() == false )
            {
                if( NodeSingle.empty() == false && strcmp( NodeSingle.value(), "1" ) == 0 )
                {
                    m_error << "Protocol::readNode_: node " << nodeXml->name << " inheritance no support single" << std::endl;

                    return false;
                }

                _node->generators.insert( std::make_pair( nodeXml->name, nodeXml ) );
            }
            else
            {
                if( NodeSingle.empty() == false && strcmp( NodeSingle.value(), "1" ) == 0 )
                {
                    if( _node->singles.find( nodeXml->name ) != _node->singles.end() )
                    {
                        m_error << "Protocol::readNode_: node " << nodeXml->name << " have multiply single element " << NodeSingle.value() << std::endl;

                        return false;
                    }

                    _node->singles.insert( std::make_pair( nodeXml->name, nodeXml ) );
                }
                else
                {

                    _node->includes.insert( std::make_pair( nodeXml->name, nodeXml ) );
                }
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::hasType( std::string_view _name ) const
    {
        TMapTypes::const_iterator it_found = m_types.find( _name );

        if( it_found == m_types.end() )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::getType( std::string_view _name, Type & _type ) const
    {
        TMapTypes::const_iterator it_found = m_types.find( _name );

        if( it_found == m_types.end() )
        {
            return false;
        }

        _type = it_found->second;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    const TMapTypes & Protocol::getTypes() const
    {
        return m_types;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Protocol::hasMeta( std::string_view _type ) const
    {
        TMapMetas::const_iterator it_found = m_metas.find( _type );

        if( it_found == m_metas.end() )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    const Meta * Protocol::getMeta( std::string_view _type ) const
    {
        TMapMetas::const_iterator it_found = m_metas.find( _type );

        if( it_found == m_metas.end() )
        {
            return nullptr;
        }

        const Meta * found_node = it_found->second;

        return found_node;
    }
    //////////////////////////////////////////////////////////////////////////
    const TMapMetas & Protocol::getMetas() const
    {
        return m_metas;
    }
    //////////////////////////////////////////////////////////////////////////
    const TVectorInternalStrings & Protocol::getInternals() const
    {
        return m_internals;
    }
    //////////////////////////////////////////////////////////////////////////
}
