#include "metaconverter/Protocol.hpp"
#include "xml2metabuf/XmlConverter.hpp"

#include "pugixml.hpp"

#include <stdio.h>

#include <string>
#include <vector>

static void * read_file( const char * _file, size_t * _size )
{
    FILE * file_protocol = fopen( _file, "rb" );

    if( file_protocol == nullptr )
    {
        return nullptr;
    }

    fseek( file_protocol, 0, SEEK_END );
    long size = ftell( file_protocol );
    fseek( file_protocol, 0, SEEK_SET );

    uint8_t * buf = new uint8_t[size];

    fread( buf, 1, size, file_protocol );

    fclose( file_protocol );

    *_size = (size_t)size;

    return (void *)buf;
}

int main( int argc, char *argv[] )
{
    if( argc != 4 )
    {
        printf( "invalid args count! '%d' need 4"
            , argc
        );

        return 0;
    }

    const char * path_protocol = argv[1];

    Metabuf::Protocol protocol;

    size_t protocol_size;
    void * protocol_buf = read_file( path_protocol, &protocol_size );

    if( protocol_buf == nullptr )
    {
        printf( "invalid open protocol: %s"
            , path_protocol
        );

        return 0;
    }

    if( protocol.readProtocol( protocol_buf, protocol_size ) == false )
    {
        std::string error = protocol.getError();

        printf( "error read protocol '%s': %s"
            , path_protocol
            , error.c_str()
        );

        return 0;
    }

    const char * path_xml = argv[2];

    size_t xml_size;
    void * xml_buf = read_file( path_xml, &xml_size );

    if( xml_buf == nullptr )
    {
        printf( "invalid open xml: %s"
            , path_xml
        );

        return 0;
    }

    pugi::xml_document document;
    pugi::xml_parse_result parse_result = document.load_buffer( xml_buf, xml_size );

    if( parse_result == false )
    {
        printf( "error parse xml '%s': %s"
            , path_xml
            , parse_result.description()
        );

        return 0;
    }

    const pugi::xml_node root = document.document_element();
    const Metabuf::Meta * meta = nullptr;
    const Metabuf::Node * node = nullptr;

    for( const Metabuf::TMapMetas::value_type & value : protocol.getMetas() )
    {
        const Metabuf::Node * test_node = value.second->getNode( root.name() );

        if( test_node != nullptr )
        {
            meta = value.second;
            node = test_node;

            break;
        }
    }

    if( meta == nullptr )
    {
        printf( "error find protocol meta for '%s'"
            , root.name()
        );

        return 0;
    }

    std::vector<uint8_t> bin;
    std::string error;

    if( Metabuf::convertXml( &protocol, xml_buf, xml_size, meta, node, bin, error ) == false )
    {

        printf( "error convert metabuf '%s': %s"
            , path_xml
            , error.c_str()
        );

        return 0;
    }

    const char * path_bin = argv[3];

    FILE * file_bin = fopen( path_bin, "wb" );

    if( file_bin == nullptr )
    {
        printf( "invalid open bin '%s'"
            , path_bin
        );

        return 0;
    }

    fwrite( bin.data(), 1, bin.size(), file_bin );
    fclose( file_bin );

    printf( "done\n" );

    return 0;
}
