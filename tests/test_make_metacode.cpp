#include "../../src/xml2metabuf/XmlProtocol.hpp"
#include "../../src/xml2metabuf/Xml2Metacode.hpp"

#include "Metautils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char * path_metacode_h = "Metacode.h";
const char * path_metacode_cpp = "Metacode.cpp";
const char * path_protocol = "Protocol.xml";

int main( int argc, char *argv[] )
{
    (void)( argc );
	(void)( argv );

	Metabuf::XmlProtocol xml_protocol;

	size_t file_protocol_size;
	void * file_protocol_buffer = read_file( argv[1], path_protocol, &file_protocol_size );

	if( file_protocol_buffer == nullptr )
	{
		printf( "invalid open protocol: %s"
			, path_protocol
		);

		return EXIT_FAILURE;
	}

	if( xml_protocol.readProtocol( file_protocol_buffer, file_protocol_size ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error read protocol: %s"
			, error.c_str()
		);

		return EXIT_FAILURE;
	}

	free( file_protocol_buffer );

	Metabuf::Xml2Metacode xml_metacode( &xml_protocol );

	std::string header;
	std::string source;
	if( xml_metacode.generate( header, source ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error generate: %s"
			, error.c_str()
		);

		return EXIT_FAILURE;
	}	

	FILE * file_metacode_h = fopen( path_metacode_h, "wb" );

	if( file_metacode_h == nullptr )
	{
		printf( "error open 'header' file '%s'"
			, path_metacode_h
		);

		return EXIT_FAILURE;
	}

	fwrite( header.c_str(), header.size(), 1, file_metacode_h );
	fclose( file_metacode_h );

	FILE * file_metacode_cpp = fopen( path_metacode_cpp, "wb" );

	if( file_metacode_cpp == nullptr )
	{
		printf( "error open 'source' file '%s'"
			, path_metacode_cpp
		);

		return EXIT_FAILURE;
	}

	fwrite( source.c_str(), source.size(), 1, file_metacode_cpp );
	fclose( file_metacode_cpp );

    return EXIT_SUCCESS;
}
