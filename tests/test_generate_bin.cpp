#include "metaconverter/Protocol.hpp"
#include "metaconverter/Converter.hpp"
#include "xml2metabuf/XmlConverter.hpp"

#include "test_utils.h"
#include "test_metacode.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <vector>

const char * path_example_xml = "Example.xml";
const char * path_example_bin = "Example.bin";
const char * path_protocol = "Protocol.xml";

int main( int argc, char *argv[] )
{
    METABUF_UNUSED( argc );
    METABUF_UNUSED( argv );

    size_t file_protocol_size;
    void * file_protocol_buffer = read_file( argv[1], path_protocol, &file_protocol_size );

    if( file_protocol_buffer == nullptr )
    {
        printf( "invalid open protocol: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    Metabuf::Protocol protocol;

    if( protocol.readProtocol( file_protocol_buffer, file_protocol_size ) == false )
    {
        printf( "error read protocol: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    free( file_protocol_buffer );

    uint32_t protocol_version = Metacode::get_metacode_protocol_version();

    if( protocol_version != protocol.getVersion() )
    {
        printf( "invalid protocol version: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    uint32_t protocol_crc32 = Metacode::get_metacode_protocol_crc32();

    if( protocol_crc32 != protocol.getCrc32() )
    {
        printf( "invalid protocol crc32: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    const Metabuf::Meta * meta = protocol.getMeta( "Data" );

    if( meta == nullptr )
    {
        printf( "error protocol don't have meta Data: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    const Metabuf::Node * node = meta->getNode( "DataBlock" );

    if( node == nullptr )
    {
        printf( "error protocol meta Data don't have node DataBlock: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    std::vector<uint8_t> metabuf_header_buffer;
    std::string header_error;

    if( Metabuf::makeHeader( &protocol, meta, metabuf_header_buffer, header_error ) == false )
    {
        printf( "invalid make header: %s error: %s"
            , path_protocol
            , header_error.c_str()
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

    std::vector<uint8_t> example_bin_buffer;
    std::string convert_error;

    if( Metabuf::convertXml( &protocol, example_xml_buffer, file_example_xml_size, meta, node, example_bin_buffer, convert_error ) == false )
    {
        printf( "invalid convert example: %s\nerror: %s"
            , path_example_xml
            , convert_error.c_str()
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

    fwrite( metabuf_header_buffer.data(), metabuf_header_buffer.size(), 1, file_example_bin );

    uint32_t write_example_bin_size = (uint32_t)example_bin_buffer.size();
    fwrite( &write_example_bin_size, sizeof( write_example_bin_size ), 1, file_example_bin );

    fwrite( example_bin_buffer.data(), example_bin_buffer.size(), 1, file_example_bin );

    fclose( file_example_bin );

    return EXIT_SUCCESS;
}
