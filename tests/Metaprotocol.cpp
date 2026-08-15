#include "Metaprotocol.h"

namespace Metacode
{
    //////////////////////////////////////////////////////////////////////////
    void MetaprotocolGenerator::generate( Metabuf::ProtocolInterface * _protocol ) const
    {
        _protocol->setVersion( 1U, 4064837494U );

        _protocol->addType( "Detail::Rect", "Detail::Rect", "uint32_t2", false, false, false );
        _protocol->addType( "bool", "bool", "bool", false, true, false );
        _protocol->addType( "float", "float", "float", false, true, false );
        _protocol->addType( "std::string", "std::string", "string", false, false, false );
        _protocol->addType( "uint32_t", "uint32_t", "uint32_t", false, true, false );

        Metabuf::MetaInterface * meta_0 = _protocol->addMeta( "Data", 1U );

        Metabuf::NodeInterface * node_0 = _protocol->addNode( 1U, "DataBlock", "", "", 3U, 0U, false, nullptr );
        Metabuf::NodeInterface * node_1 = _protocol->addNode( 1U, "Include", "", "", 0U, 0U, false, node_0 );
        Metabuf::NodeInterface * node_2 = _protocol->addNode( 2U, "Resource", "Type", "", 0U, 2U, false, node_0 );
        Metabuf::NodeInterface * node_3 = _protocol->addNode( 3U, "ResourceImageDefault", "", "Resource", 0U, 8U, false, node_0 );

        node_0->addAttribute( 0U, "Name", "std::string", true, "" );
        node_0->addInclude( node_1 );
        node_0->addInheritance( node_2 );
        node_0->addGenerator( node_3 );

        node_1->addAttribute( 0U, "Path", "std::string", true, "" );

        node_2->addAttribute( 0U, "Name", "std::string", true, "" );
        node_2->addAttribute( 1U, "Precompile", "bool", false, "" );
        node_2->addAttribute( 0U, "Type", "std::string", true, "" );
        node_2->addAttribute( 0U, "Unique", "bool", false, "" );

        node_3->setInheritance( node_2 );
        node_3->addMemberAttribute( "File", 4U, "Alpha", "bool", false, "" );
        node_3->addMemberAttribute( "File", 2U, "Codec", "std::string", false, "" );
        node_3->addMemberAttribute( "File", 3U, "Converter", "std::string", false, "" );
        node_3->addMemberAttribute( "File", 0U, "MaxSize", "Detail::Rect", true, "" );
        node_3->addMemberAttribute( "File", 7U, "Offset", "Detail::Rect", false, "" );
        node_3->addMemberAttribute( "File", 0U, "Path", "std::string", true, "" );
        node_3->addMemberAttribute( "File", 5U, "Premultiply", "bool", false, "" );
        node_3->addMemberAttribute( "File", 6U, "Size", "Detail::Rect", false, "" );

        meta_0->addNode( node_0 );
        _protocol->addInternal( "ResourceImageDefault" );
    }
    //////////////////////////////////////////////////////////////////////////
}
