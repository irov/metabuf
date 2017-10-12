#	include "XmlProtocol.hpp"
#	include "Xml2Metabuf.hpp"
#	include "Xml2Metacode.hpp"

#	include <stdio.h>

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


	Metabuf::XmlProtocol xml_protocol;

	FILE * file_protocol = fopen( path_protocol, "rb");

	long size;

	fseek(file_protocol, 0, SEEK_END);
	size = ftell(file_protocol);
	fseek(file_protocol, 0, SEEK_SET);
	
	char * buf = new char[size];

	fread( buf, 1, size, file_protocol );
	
	fclose( file_protocol );

	if( xml_protocol.readProtocol( buf, size ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error read protocol: %s"
			, error.c_str()
		);

		return 0;
	}
	
	Metabuf::Xml2Metacode xml_metacode(&xml_protocol);

	std::string header;
    std::string source;
	if( xml_metacode.generate( header, source ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error generate: %s"
			, error.c_str()
		);

		return 0;
	}

	const char * metacodeh_protocol = argv[2];
	
	FILE * file_metacode_hpp = fopen( metacodeh_protocol, "wb");

	if( file_metacode_hpp == nullptr )
	{
		printf( "error open 'header' file '%s'"
			, metacodeh_protocol
		);

		return 0;
	}

	fwrite( header.c_str(), header.size(), 1, file_metacode_hpp );
	fclose( file_metacode_hpp );

	const char * metacodecpp_protocol = argv[3];

    FILE * file_metacode_cpp = fopen( metacodecpp_protocol, "wb");

	if( file_metacode_cpp == NULL )
	{
		printf( "error open 'source' file '%s'"
			, metacodecpp_protocol
		);

		return 0;
	}

    fwrite( source.c_str(), source.size(), 1, file_metacode_cpp );
    fclose( file_metacode_cpp );
	
	printf( "done\n" );

	return 0;
}