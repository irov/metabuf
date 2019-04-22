#include "../../src/xml2metabuf/XmlProtocol.hpp"
#include "../../src/xml2metabuf/Xml2Metabuf.hpp"

#include "test_utils.h"
#include "test_metacode.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char * path_example_xml = "Example.xml";
const char * path_example_bin = "Example.bin";
const char * path_protocol = "Protocol.xml";

int main( int argc, char *argv[] )
{
    (void)(argc);
    (void)(argv);

    size_t file_protocol_size;
    void * file_protocol_buffer = read_file( argv[1], path_protocol, &file_protocol_size );

    if( file_protocol_buffer == nullptr )
    {
        printf( "invalid open protocol: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    Metabuf::XmlProtocol xml_protocol;

    if( xml_protocol.readProtocol( file_protocol_buffer, file_protocol_size ) == false )
    {
        printf( "error read protocol: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    free( file_protocol_buffer );

    uint32_t protocol_version = Metacode::get_metacode_protocol_version();

    if( protocol_version != xml_protocol.getVersion() )
    {
        printf( "invalid protocol version: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    uint32_t protocol_crc32 = Metacode::get_metacode_protocol_crc32();

    if( protocol_crc32 != xml_protocol.getCrc32() )
    {
        printf( "invalid protocol crc32: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    const Metabuf::XmlMeta * xml_meta = xml_protocol.getMeta( "Data" );

    if( xml_meta == nullptr )
    {
        printf( "error protocol don't have meta Data: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    uint32_t xml_meta_version = xml_meta->getVersion();

    Metabuf::Xml2Metabuf xml_metabuf( &xml_protocol, xml_meta );

    xml_metabuf.initialize();

    uint8_t metabuf_header_buffer[Metacode::header_size];

    size_t header_size;
    if( xml_metabuf.header( metabuf_header_buffer, Metacode::header_size, xml_meta_version, &header_size ) == false )
    {
        printf( "invalid make header: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    size_t file_example_xml_size;
    void * example_xml_buffer = read_file( argv[1], path_example_xml, &file_example_xml_size );

    if( example_xml_buffer == nullptr )
    {
        printf( "invalid open protocol: %s"
            , path_example_xml
        );

        return EXIT_FAILURE;
    }

    uint8_t * example_bin_buffer = (uint8_t *)malloc( file_example_xml_size * 2 );

    size_t example_bin_size;
    if( xml_metabuf.convert( example_bin_buffer, file_example_xml_size * 2, example_xml_buffer, file_example_xml_size, &example_bin_size ) == false )
    {
        printf( "invalid convert example: %s\nerror: %s"
            , path_example_xml
            , xml_metabuf.getError().c_str()
        );

        return EXIT_FAILURE;
    }

    free( example_xml_buffer );

    FILE * file_example_bin = write_file( argv[1], path_example_bin );

    if( file_example_bin == nullptr )
    {
        printf( "error open 'header' file '%s'"
            , path_example_bin
        );

        return EXIT_FAILURE;
    }

    fwrite( metabuf_header_buffer, Metacode::header_size, 1, file_example_bin );

    uint32_t write_example_bin_size = (uint32_t)example_bin_size;
    fwrite( &write_example_bin_size, sizeof( write_example_bin_size ), 1, file_example_bin );

    fwrite( example_bin_buffer, example_bin_size, 1, file_example_bin );

    fclose( file_example_bin );

    free( example_bin_buffer );

    return EXIT_SUCCESS;
}
