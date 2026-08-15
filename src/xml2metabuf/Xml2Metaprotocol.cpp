#include "Xml2Metaprotocol.hpp"

#include <map>
#include <sstream>
#include <vector>

namespace Metabuf
{
    namespace Detail
    {
        typedef std::vector<const Node *> VectorNodes;
        typedef std::map<const Node *, size_t> MapNodeIndices;

        //////////////////////////////////////////////////////////////////////////
        static std::string quote( const std::string & _value )
        {
            std::stringstream ss;
            ss << '"';

            for( std::string::const_iterator
                it = _value.begin(),
                it_end = _value.end();
                it != it_end;
                ++it )
            {
                switch( *it )
                {
                case '\\': ss << "\\\\"; break;
                case '"': ss << "\\\""; break;
                case '\n': ss << "\\n"; break;
                case '\r': ss << "\\r"; break;
                case '\t': ss << "\\t"; break;
                default: ss << *it; break;
                }
            }

            ss << '"';

            const std::string value = ss.str();

            return value;
        }
        //////////////////////////////////////////////////////////////////////////
        static void collectNode( const Node * _node, VectorNodes & _nodes, MapNodeIndices & _indices )
        {
            _indices.emplace( _node, _nodes.size() );
            _nodes.emplace_back( _node );

            const TMapNodes * maps[] = {
                &_node->singles,
                &_node->includes,
                &_node->inheritances,
                &_node->generators
            };

            for( const TMapNodes * map : maps )
            {
                for( TMapNodes::const_iterator
                    it = map->begin(),
                    it_end = map->end();
                    it != it_end;
                    ++it )
                {
                    collectNode( it->second, _nodes, _indices );
                }
            }
        }
        //////////////////////////////////////////////////////////////////////////
        static void writeNodeAttribute( std::stringstream & _ss, size_t _nodeIndex, const Attribute & _attribute )
        {
            _ss << "        node_" << _nodeIndex << "->addAttribute( "
                << _attribute.id << "U, "
                << quote( _attribute.name ) << ", "
                << quote( _attribute.type ) << ", "
                << (_attribute.required == true ? "true" : "false") << ", "
                << quote( _attribute.default_value )
                << " );" << std::endl;
        }
        //////////////////////////////////////////////////////////////////////////
        static void writeNodeMemberAttribute( std::stringstream & _ss, size_t _nodeIndex, const std::string & _member, const Attribute & _attribute )
        {
            _ss << "        node_" << _nodeIndex << "->addMemberAttribute( "
                << quote( _member ) << ", "
                << _attribute.id << "U, "
                << quote( _attribute.name ) << ", "
                << quote( _attribute.type ) << ", "
                << (_attribute.required == true ? "true" : "false") << ", "
                << quote( _attribute.default_value )
                << " );" << std::endl;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    Xml2Metaprotocol::Xml2Metaprotocol( const Protocol * _protocol )
        : m_protocol( _protocol )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metaprotocol::generate( std::string & _header, std::string & _source ) const
    {
        std::stringstream header;
        header << "#pragma once" << std::endl;
        header << std::endl;
        header << "#include \"metabuf/Metaconvert.hpp\"" << std::endl;
        header << std::endl;
        header << "namespace Metacode" << std::endl;
        header << "{" << std::endl;
        header << "    class MetaprotocolGenerator" << std::endl;
        header << "        : public Metabuf::ProtocolGenerator" << std::endl;
        header << "    {" << std::endl;
        header << "    public:" << std::endl;
        header << "        void generate( Metabuf::ProtocolInterface * _protocol ) const override;" << std::endl;
        header << "    };" << std::endl;
        header << "}" << std::endl;

        _header = header.str();

        Detail::VectorNodes nodes;
        Detail::MapNodeIndices node_indices;
        const TMapMetas & metas = m_protocol->getMetas();

        for( TMapMetas::const_iterator
            it_meta = metas.begin(),
            it_meta_end = metas.end();
            it_meta != it_meta_end;
            ++it_meta )
        {
            const TMapNodes & meta_nodes = it_meta->second->getNodes();

            for( TMapNodes::const_iterator
                it_node = meta_nodes.begin(),
                it_node_end = meta_nodes.end();
                it_node != it_node_end;
                ++it_node )
            {
                Detail::collectNode( it_node->second, nodes, node_indices );
            }
        }

        std::stringstream source;
        source << "#include \"Metaprotocol.h\"" << std::endl;
        source << std::endl;
        source << "namespace Metacode" << std::endl;
        source << "{" << std::endl;
        source << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        source << "    void MetaprotocolGenerator::generate( Metabuf::ProtocolInterface * _protocol ) const" << std::endl;
        source << "    {" << std::endl;
        source << "        _protocol->setVersion( " << m_protocol->getVersion() << "U, " << m_protocol->getCrc32() << "U );" << std::endl;
        source << std::endl;

        const TMapTypes & types = m_protocol->getTypes();

        size_t type_index = 0;
        for( TMapTypes::const_iterator
            it_type = types.begin(),
            it_type_end = types.end();
            it_type != it_type_end;
            ++it_type, ++type_index )
        {
            const Type & type = it_type->second;

            if( type.enumerators.empty() == false )
            {
                source << "        Metabuf::TypeInterface * type_" << type_index << " = ";
            }
            else
            {
                source << "        ";
            }

            source << "_protocol->addType( "
                << Detail::quote( it_type->first ) << ", "
                << Detail::quote( type.write ) << ", "
                << Detail::quote( type.evict ) << ", "
                << (type.is_enumerator == true ? "true" : "false") << ", "
                << (type.is_ncr == true ? "true" : "false") << ", "
                << (type.is_template == true ? "true" : "false") << " );" << std::endl;

            for( Type::TVectorEnumerators::const_iterator
                it_enum = type.enumerators.begin(),
                it_enum_end = type.enumerators.end();
                it_enum != it_enum_end;
                ++it_enum )
            {
                source << "        type_" << type_index << "->addEnumerator( "
                    << Detail::quote( it_enum->name ) << ", "
                    << Detail::quote( it_enum->write ) << ", "
                    << it_enum->index << "U );" << std::endl;
            }
        }

        source << std::endl;

        size_t meta_index = 0;
        for( TMapMetas::const_iterator
            it_meta = metas.begin(),
            it_meta_end = metas.end();
            it_meta != it_meta_end;
            ++it_meta, ++meta_index )
        {
            const Meta * meta = it_meta->second;
            source << "        Metabuf::MetaInterface * meta_" << meta_index << " = _protocol->addMeta( "
                << Detail::quote( meta->m_name ) << ", "
                << meta->m_version << "U );" << std::endl;
        }

        source << std::endl;

        for( size_t node_index = 0; node_index != nodes.size(); ++node_index )
        {
            const Node * node = nodes[node_index];

            source << "        Metabuf::NodeInterface * node_" << node_index << " = _protocol->addNode( "
                << node->id << "U, "
                << Detail::quote( node->name ) << ", "
                << Detail::quote( node->generator ) << ", "
                << Detail::quote( node->inheritance ) << ", "
                << node->enumerator << "U, "
                << node->enumeratorNRA << "U, "
                << (node->noWrite == true ? "true" : "false") << ", ";

            if( node->node_scope == nullptr )
            {
                source << "nullptr";
            }
            else
            {
                source << "node_" << node_indices[node->node_scope];
            }

            source << " );" << std::endl;
        }

        source << std::endl;

        for( size_t node_index = 0; node_index != nodes.size(); ++node_index )
        {
            const Node * node = nodes[node_index];

            if( node->node_inheritance != nullptr )
            {
                source << "        node_" << node_index << "->setInheritance( node_" << node_indices[node->node_inheritance] << " );" << std::endl;
            }

            for( TMapAttributes::const_iterator
                it_attribute = node->attributes.begin(),
                it_attribute_end = node->attributes.end();
                it_attribute != it_attribute_end;
                ++it_attribute )
            {
                Detail::writeNodeAttribute( source, node_index, it_attribute->second );
            }

            for( TMapMembers::const_iterator
                it_member = node->members.begin(),
                it_member_end = node->members.end();
                it_member != it_member_end;
                ++it_member )
            {
                for( TMapAttributes::const_iterator
                    it_attribute = it_member->second.attributes.begin(),
                    it_attribute_end = it_member->second.attributes.end();
                    it_attribute != it_attribute_end;
                    ++it_attribute )
                {
                    Detail::writeNodeMemberAttribute( source, node_index, it_member->first, it_attribute->second );
                }
            }

            for( TMapChildren::const_iterator
                it_children = node->children.begin(),
                it_children_end = node->children.end();
                it_children != it_children_end;
                ++it_children )
            {
                source << "        node_" << node_index << "->addChildren( "
                    << Detail::quote( it_children->first ) << ", "
                    << Detail::quote( it_children->second.group ) << ", "
                    << Detail::quote( it_children->second.type ) << " );" << std::endl;
            }

            const TMapNodes * maps[] = {&node->singles, &node->includes, &node->inheritances, &node->generators};
            const char * functions[] = {"addSingle", "addInclude", "addInheritance", "addGenerator"};

            for( size_t map_index = 0; map_index != 4; ++map_index )
            {
                for( TMapNodes::const_iterator
                    it_child = maps[map_index]->begin(),
                    it_child_end = maps[map_index]->end();
                    it_child != it_child_end;
                    ++it_child )
                {
                    source << "        node_" << node_index << "->" << functions[map_index] << "( node_" << node_indices[it_child->second] << " );" << std::endl;
                }
            }

            source << std::endl;
        }

        meta_index = 0;
        for( TMapMetas::const_iterator
            it_meta = metas.begin(),
            it_meta_end = metas.end();
            it_meta != it_meta_end;
            ++it_meta, ++meta_index )
        {
            const TMapNodes & meta_nodes = it_meta->second->getNodes();

            for( TMapNodes::const_iterator
                it_node = meta_nodes.begin(),
                it_node_end = meta_nodes.end();
                it_node != it_node_end;
                ++it_node )
            {
                source << "        meta_" << meta_index << "->addNode( node_" << node_indices[it_node->second] << " );" << std::endl;
            }
        }

        const TVectorInternalStrings & internals = m_protocol->getInternals();

        for( TVectorInternalStrings::const_iterator
            it_internal = internals.begin(),
            it_internal_end = internals.end();
            it_internal != it_internal_end;
            ++it_internal )
        {
            source << "        _protocol->addInternal( " << Detail::quote( *it_internal ) << " );" << std::endl;
        }

        source << "    }" << std::endl;
        source << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        source << "}" << std::endl;

        _source = source.str();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
