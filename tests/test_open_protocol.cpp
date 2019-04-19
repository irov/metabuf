#include "../../src/xml2metabuf/XmlProtocol.hpp"
#include "../../src/xml2metabuf/Xml2Metacode.hpp"

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

	FILE * file_protocol = ::fopen( path_protocol, "rb" );

	if( file_protocol == nullptr )
	{
		printf( "invalid open protocol: %s"
			, path_protocol
		);

		return EXIT_FAILURE;
	}

	fseek( file_protocol, 0, SEEK_END );
	long size = ftell( file_protocol );
	fseek( file_protocol, 0, SEEK_SET );

	uint8_t * buf = (uint8_t *)malloc( size );

	fread( buf, 1, size, file_protocol );

	fclose( file_protocol );

	if( xml_protocol.readProtocol( buf, size ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error read protocol: %s"
			, error.c_str()
		);

		return EXIT_FAILURE;
	}

	free( buf );	

    return EXIT_SUCCESS;
}
