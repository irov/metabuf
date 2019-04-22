#include "../../src/xml2metabuf/XmlProtocol.hpp"
#include "../../src/xml2metabuf/Xml2Metacode.hpp"

#include "test_utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char * path_metacode_h = "test_metacode.h";
const char * path_metacode_cpp = "test_metacode.cpp";
const char * path_protocol = "Protocol.xml";

int main( int argc, char *argv[] )
{
    (void)( argc );
	(void)( argv );

	Metabuf::XmlProtocol xml_protocol;

	size_t file_protocol_size;
	void * file_protocol_buffer = read_file( "d:/Project/Mengine/dependencies/metabuf/tests", path_protocol, &file_protocol_size );

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

	Metabuf::Xml2Settings xml_settings;
	xml_settings.metacode_h = "test_metacode.h";
	xml_settings.metatype_h = "test_metatype.h";

	std::string header;
	std::string source;
	if( xml_metacode.generate( header, source, xml_settings ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error generate: %s"
			, error.c_str()
		);

		return EXIT_FAILURE;
	}	

	FILE * file_metacode_h = write_file( "d:/Project/Mengine/dependencies/metabuf/tests", path_metacode_h );

	if( file_metacode_h == nullptr )
	{
		printf( "error open 'header' file '%s'"
			, path_metacode_h
		);

		return EXIT_FAILURE;
	}

	fwrite( header.c_str(), header.size(), 1, file_metacode_h );
	fclose( file_metacode_h );

	FILE * file_metacode_cpp = write_file( "d:/Project/Mengine/dependencies/metabuf/tests", path_metacode_cpp );

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
