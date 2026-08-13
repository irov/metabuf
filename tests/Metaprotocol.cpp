#include "Metaprotocol.h"

#include <utility>

namespace Metacode
{
    //////////////////////////////////////////////////////////////////////////
    void initializeMetaprotocol( Metabuf::XmlProtocol * _protocol )
    {
        Metabuf::TMapMetas metas;
        Metabuf::TMapTypes types;
        Metabuf::TVectorInternalStrings internals;

        types.emplace( "Detail::Rect", Metabuf::XmlType{{}, "Detail::Rect", "uint32_t2", false, false, false} );
        types.emplace( "bool", Metabuf::XmlType{{}, "bool", "bool", false, true, false} );
        types.emplace( "float", Metabuf::XmlType{{}, "float", "float", false, true, false} );
        types.emplace( "std::string", Metabuf::XmlType{{}, "std::string", "string", false, false, false} );
        types.emplace( "uint32_t", Metabuf::XmlType{{}, "uint32_t", "uint32_t", false, true, false} );

        Metabuf::XmlMeta * meta_0 = new Metabuf::XmlMeta();
        meta_0->m_name = "Data";
        meta_0->m_version = 1U;

        Metabuf::XmlNode * node_0 = new Metabuf::XmlNode( 1U, "DataBlock", "", "", 3U, 0U, false, nullptr );
        Metabuf::XmlNode * node_1 = new Metabuf::XmlNode( 1U, "Include", "", "", 0U, 0U, false, node_0 );
        Metabuf::XmlNode * node_2 = new Metabuf::XmlNode( 2U, "Resource", "Type", "", 0U, 2U, false, node_0 );
        Metabuf::XmlNode * node_3 = new Metabuf::XmlNode( 3U, "ResourceImageDefault", "", "Resource", 0U, 8U, false, node_0 );

        node_0->attributes.emplace( "Name", Metabuf::XmlAttribute{0U, "Name", "std::string", true, ""} );
        node_0->includes.emplace( "Include", node_1 );
        node_0->inheritances.emplace( "Resource", node_2 );
        node_0->generators.emplace( "ResourceImageDefault", node_3 );

        node_1->attributes.emplace( "Path", Metabuf::XmlAttribute{0U, "Path", "std::string", true, ""} );

        node_2->attributes.emplace( "Name", Metabuf::XmlAttribute{0U, "Name", "std::string", true, ""} );
        node_2->attributes.emplace( "Precompile", Metabuf::XmlAttribute{1U, "Precompile", "bool", false, ""} );
        node_2->attributes.emplace( "Type", Metabuf::XmlAttribute{0U, "Type", "std::string", true, ""} );
        node_2->attributes.emplace( "Unique", Metabuf::XmlAttribute{0U, "Unique", "bool", false, ""} );

        node_3->node_inheritance = node_2;
        node_3->members["File"].name = "File";
        node_3->members["File"].attributes.emplace( "Alpha", Metabuf::XmlAttribute{4U, "Alpha", "bool", false, ""} );
        node_3->members["File"].attributes.emplace( "Codec", Metabuf::XmlAttribute{2U, "Codec", "std::string", false, ""} );
        node_3->members["File"].attributes.emplace( "Converter", Metabuf::XmlAttribute{3U, "Converter", "std::string", false, ""} );
        node_3->members["File"].attributes.emplace( "MaxSize", Metabuf::XmlAttribute{0U, "MaxSize", "Detail::Rect", true, ""} );
        node_3->members["File"].attributes.emplace( "Offset", Metabuf::XmlAttribute{7U, "Offset", "Detail::Rect", false, ""} );
        node_3->members["File"].attributes.emplace( "Path", Metabuf::XmlAttribute{0U, "Path", "std::string", true, ""} );
        node_3->members["File"].attributes.emplace( "Premultiply", Metabuf::XmlAttribute{5U, "Premultiply", "bool", false, ""} );
        node_3->members["File"].attributes.emplace( "Size", Metabuf::XmlAttribute{6U, "Size", "Detail::Rect", false, ""} );

        meta_0->m_nodes.emplace( "DataBlock", node_0 );
        metas.emplace( "Data", meta_0 );
        internals.emplace_back( "ResourceImageDefault" );

        _protocol->initialize( 1U, 4064837494U, std::move( metas ), std::move( types ), std::move( internals ) );
    }
    //////////////////////////////////////////////////////////////////////////
}
