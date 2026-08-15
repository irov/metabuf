#include "metaconverter/Protocol.hpp"
#include "Xml2Metacode.hpp"
#include "Xml2Metaprotocol.hpp"

#include <stdlib.h>
#include <stdio.h>

namespace
{
    bool writeOutput( const char * _path, const std::string & _content )
    {
        FILE * file = fopen( _path, "wb" );

        if( file == nullptr )
        {
            printf( "error open output file '%s'", _path );

            return false;
        }

        const bool successful = fwrite( _content.data(), _content.size(), 1, file ) == 1;
        fclose( file );

        return successful;
    }
}

int main( int argc, char *argv[] )
{
    if( argc != 6 )
    {
        printf( "invalid args count! '%d' need 6"
            , argc
        );

        return EXIT_FAILURE;
    }

    const char * path_protocol = argv[1];
    FILE * file_protocol = fopen( path_protocol, "rb" );

    if( file_protocol == nullptr )
    {
        printf( "error open protocol file '%s'", path_protocol );

        return EXIT_FAILURE;
    }

    if( fseek( file_protocol, 0, SEEK_END ) != 0 )
    {
        fclose( file_protocol );

        return EXIT_FAILURE;
    }

    const long size = ftell( file_protocol );

    if( size < 0 || fseek( file_protocol, 0, SEEK_SET ) != 0 )
    {
        fclose( file_protocol );

        return EXIT_FAILURE;
    }

    char * buf = new char[size];

    if( fread( buf, 1, size, file_protocol ) != static_cast<size_t>(size) )
    {
        delete [] buf;
        fclose( file_protocol );

        return EXIT_FAILURE;
    }

    fclose( file_protocol );

    Metabuf::Protocol protocol;

    if( protocol.readProtocol( buf, size ) == false )
    {
        delete [] buf;

        std::string error = protocol.getError();

        printf( "error read protocol: %s"
            , error.c_str()
        );

        return EXIT_FAILURE;
    }

    delete [] buf;

    Metabuf::Xml2Metacode xml_metacode( &protocol );

    Metabuf::Xml2Settings xml_settings;
    xml_settings.metacode_h = "Metacode.h";
    xml_settings.metatype_h = "Metatype.h";

    std::string header;
    std::string source;
    if( xml_metacode.generate( header, source, xml_settings ) == false )
    {
        std::string error = xml_metacode.getError();

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

    if( writeOutput( argv[2], header ) == false ||
        writeOutput( argv[3], source ) == false ||
        writeOutput( argv[4], protocol_header ) == false ||
        writeOutput( argv[5], protocol_source ) == false )
    {
        return EXIT_FAILURE;
    }

    printf( "done\n" );

    return EXIT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
