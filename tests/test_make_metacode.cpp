#include "metaconverter/Protocol.hpp"
#include "../../src/xml2metabuf/Xml2Metacode.hpp"
#include "../../src/xml2metabuf/Xml2Metaprotocol.hpp"

#include "test_utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char * path_metacode_h = "test_metacode.h";
const char * path_metacode_cpp = "test_metacode.cpp";
const char * path_metaprotocol_h = "Metaprotocol.h";
const char * path_metaprotocol_cpp = "Metaprotocol.cpp";
const char * path_protocol = "Protocol.xml";

namespace
{
    bool writeOutput( const char * _base, const char * _path, const std::string & _content )
    {
        FILE * file = write_file( _base, _path );

        if( file == nullptr )
        {
            return false;
        }

        const bool successful = fwrite( _content.data(), _content.size(), 1, file ) == 1;
        fclose( file );

        return successful;
    }
}

int main( int argc, char *argv[] )
{
    METABUF_UNUSED( argc );
    METABUF_UNUSED( argv );

    Metabuf::Protocol protocol;

    size_t file_protocol_size;
    void * file_protocol_buffer = read_file( argv[1], path_protocol, &file_protocol_size );

    if( file_protocol_buffer == nullptr )
    {
        printf( "invalid open protocol: %s"
            , path_protocol
        );

        return EXIT_FAILURE;
    }

    if( protocol.readProtocol( file_protocol_buffer, file_protocol_size ) == false )
    {
        std::string error = protocol.getError();

        printf( "error read protocol: %s"
            , error.c_str()
        );

        return EXIT_FAILURE;
    }

    free( file_protocol_buffer );

    Metabuf::Xml2Metacode xml_metacode( &protocol );

    Metabuf::Xml2Settings xml_settings;
    xml_settings.metacode_h = "test_metacode.h";
    xml_settings.metatype_h = "test_metatype.h";

    std::string header;
    std::string source;
    if( xml_metacode.generate( header, source, xml_settings ) == false )
    {
        std::string error = protocol.getError();

        printf( "error generate: %s"
            , error.c_str()
        );

        return EXIT_FAILURE;
    }

    Metabuf::Xml2Metaprotocol xml_metaprotocol( &protocol );

    std::string protocol_header;
    std::string protocol_source;

    if( xml_metaprotocol.generate( protocol_header, protocol_source ) == false )
    {
        return EXIT_FAILURE;
    }

    if( writeOutput( argv[1], path_metacode_h, header ) == false ||
        writeOutput( argv[1], path_metacode_cpp, source ) == false ||
        writeOutput( argv[1], path_metaprotocol_h, protocol_header ) == false ||
        writeOutput( argv[1], path_metaprotocol_cpp, protocol_source ) == false )
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
