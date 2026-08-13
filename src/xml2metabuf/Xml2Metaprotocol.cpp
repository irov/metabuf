#include "Xml2Metaprotocol.hpp"

#include <map>
#include <sstream>
#include <vector>

namespace Metabuf
{
    namespace Detail
    {
        typedef std::vector<const XmlNode *> VectorNodes;
        typedef std::map<const XmlNode *, size_t> MapNodeIndices;

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
        static void collectNode( const XmlNode * _node, VectorNodes & _nodes, MapNodeIndices & _indices )
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
        static void writeAttribute( std::stringstream & _ss, const char * _owner, const XmlAttribute & _attribute )
        {
            _ss << "        " << _owner << ".emplace( "
                << quote( _attribute.name )
                << ", Metabuf::XmlAttribute{"
                << _attribute.id << "U, "
                << quote( _attribute.name ) << ", "
                << quote( _attribute.type ) << ", "
                << (_attribute.required == true ? "true" : "false") << ", "
                << quote( _attribute.default_value )
                << "} );" << std::endl;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    Xml2Metaprotocol::Xml2Metaprotocol( const XmlProtocol * _protocol )
        : m_protocol( _protocol )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metaprotocol::generate( std::string & _header, std::string & _source ) const
    {
        std::stringstream header;
        header << "#pragma once" << std::endl;
        header << std::endl;
        header << "#include \"xml2metabuf/XmlProtocol.hpp\"" << std::endl;
        header << std::endl;
        header << "namespace Metacode" << std::endl;
        header << "{" << std::endl;
        header << "    void initializeMetaprotocol( Metabuf::XmlProtocol * _protocol );" << std::endl;
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
        source << "#include <utility>" << std::endl;
        source << std::endl;
        source << "namespace Metacode" << std::endl;
        source << "{" << std::endl;
        source << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        source << "    void initializeMetaprotocol( Metabuf::XmlProtocol * _protocol )" << std::endl;
        source << "    {" << std::endl;
        source << "        Metabuf::TMapMetas metas;" << std::endl;
        source << "        Metabuf::TMapTypes types;" << std::endl;
        source << "        Metabuf::TVectorInternalStrings internals;" << std::endl;
        source << std::endl;

        const TMapTypes & types = m_protocol->getTypes();

        for( TMapTypes::const_iterator
            it_type = types.begin(),
            it_type_end = types.end();
            it_type != it_type_end;
            ++it_type )
        {
            const XmlType & type = it_type->second;
            source << "        types.emplace( " << Detail::quote( it_type->first ) << ", Metabuf::XmlType{{";

            for( XmlType::TVectorEnumerators::const_iterator
                it_enum = type.enumerators.begin(),
                it_enum_end = type.enumerators.end();
                it_enum != it_enum_end;
                ++it_enum )
            {
                if( it_enum != type.enumerators.begin() )
                {
                    source << ", ";
                }

                source << "Metabuf::XmlEnum{" << Detail::quote( it_enum->name ) << ", " << Detail::quote( it_enum->write ) << ", " << it_enum->index << "U}";
            }

            source << "}, " << Detail::quote( type.write ) << ", " << Detail::quote( type.evict ) << ", "
                << (type.is_enumerator == true ? "true" : "false") << ", "
                << (type.is_ncr == true ? "true" : "false") << ", "
                << (type.is_template == true ? "true" : "false") << "} );" << std::endl;
        }

        source << std::endl;

        size_t meta_index = 0;
        for( TMapMetas::const_iterator
            it_meta = metas.begin(),
            it_meta_end = metas.end();
            it_meta != it_meta_end;
            ++it_meta, ++meta_index )
        {
            const XmlMeta * meta = it_meta->second;
            source << "        Metabuf::XmlMeta * meta_" << meta_index << " = new Metabuf::XmlMeta();" << std::endl;
            source << "        meta_" << meta_index << "->m_name = " << Detail::quote( meta->m_name ) << ";" << std::endl;
            source << "        meta_" << meta_index << "->m_version = " << meta->m_version << "U;" << std::endl;
        }

        source << std::endl;

        for( size_t node_index = 0; node_index != nodes.size(); ++node_index )
        {
            const XmlNode * node = nodes[node_index];

            source << "        Metabuf::XmlNode * node_" << node_index << " = new Metabuf::XmlNode( "
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
            const XmlNode * node = nodes[node_index];

            if( node->node_inheritance != nullptr )
            {
                source << "        node_" << node_index << "->node_inheritance = node_" << node_indices[node->node_inheritance] << ";" << std::endl;
            }

            for( TMapAttributes::const_iterator
                it_attribute = node->attributes.begin(),
                it_attribute_end = node->attributes.end();
                it_attribute != it_attribute_end;
                ++it_attribute )
            {
                std::stringstream owner;
                owner << "node_" << node_index << "->attributes";
                Detail::writeAttribute( source, owner.str().c_str(), it_attribute->second );
            }

            for( TMapMembers::const_iterator
                it_member = node->members.begin(),
                it_member_end = node->members.end();
                it_member != it_member_end;
                ++it_member )
            {
                source << "        node_" << node_index << "->members[" << Detail::quote( it_member->first ) << "].name = " << Detail::quote( it_member->second.name ) << ";" << std::endl;

                for( TMapAttributes::const_iterator
                    it_attribute = it_member->second.attributes.begin(),
                    it_attribute_end = it_member->second.attributes.end();
                    it_attribute != it_attribute_end;
                    ++it_attribute )
                {
                    std::stringstream owner;
                    owner << "node_" << node_index << "->members[" << Detail::quote( it_member->first ) << "].attributes";
                    Detail::writeAttribute( source, owner.str().c_str(), it_attribute->second );
                }
            }

            for( TMapChildren::const_iterator
                it_children = node->children.begin(),
                it_children_end = node->children.end();
                it_children != it_children_end;
                ++it_children )
            {
                source << "        node_" << node_index << "->children[" << Detail::quote( it_children->first ) << "].group = " << Detail::quote( it_children->second.group ) << ";" << std::endl;
                source << "        node_" << node_index << "->children[" << Detail::quote( it_children->first ) << "].type = " << Detail::quote( it_children->second.type ) << ";" << std::endl;
            }

            const TMapNodes * maps[] = {&node->singles, &node->includes, &node->inheritances, &node->generators};
            const char * names[] = {"singles", "includes", "inheritances", "generators"};

            for( size_t map_index = 0; map_index != 4; ++map_index )
            {
                for( TMapNodes::const_iterator
                    it_child = maps[map_index]->begin(),
                    it_child_end = maps[map_index]->end();
                    it_child != it_child_end;
                    ++it_child )
                {
                    source << "        node_" << node_index << "->" << names[map_index] << ".emplace( " << Detail::quote( it_child->first ) << ", node_" << node_indices[it_child->second] << " );" << std::endl;
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
                source << "        meta_" << meta_index << "->m_nodes.emplace( " << Detail::quote( it_node->first ) << ", node_" << node_indices[it_node->second] << " );" << std::endl;
            }

            source << "        metas.emplace( " << Detail::quote( it_meta->first ) << ", meta_" << meta_index << " );" << std::endl;
        }

        const TVectorInternalStrings & internals = m_protocol->getInternals();

        for( TVectorInternalStrings::const_iterator
            it_internal = internals.begin(),
            it_internal_end = internals.end();
            it_internal != it_internal_end;
            ++it_internal )
        {
            source << "        internals.emplace_back( " << Detail::quote( *it_internal ) << " );" << std::endl;
        }

        source << std::endl;
        source << "        _protocol->initialize( " << m_protocol->getVersion() << "U, " << m_protocol->getCrc32() << "U, std::move( metas ), std::move( types ), std::move( internals ) );" << std::endl;
        source << "    }" << std::endl;
        source << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        source << "}" << std::endl;

        _source = source.str();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
