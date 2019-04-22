#include "../../src/xml2metabuf/XmlProtocol.hpp"
#include "../../src/xml2metabuf/Xml2Metacode.hpp"

#include "test_utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

    return EXIT_SUCCESS;
}
