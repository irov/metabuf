#	include "XmlProtocol.hpp"
#	include "Xml2Metabuf.hpp"

#	include <stdio.h>

static void * read_file( const char * _file, size_t * _size )
{
	FILE * file_protocol = fopen( _file, "rb" );

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
	
	Metabuf::XmlProtocol xml_protocol;

	size_t protocol_size;
	void * protocol_buf = read_file( path_protocol, &protocol_size );

	if( xml_protocol.readProtocol( protocol_buf, protocol_size ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error read protocol: %s"
			, error.c_str()
		);

		return 0;
	}
	
	Metabuf::Xml2Metabuf xml_metabuf(&xml_protocol);

	xml_metabuf.initialize();

	const char * path_xml = argv[2];

	size_t xml_size;
	void * xml_buf = read_file( path_xml, &xml_size );

	size_t try_bin_size = xml_size * 2;
	uint8_t * bin_buf = new uint8_t[try_bin_size];
	
	size_t bin_size;
	if( xml_metabuf.convert( bin_buf, try_bin_size, xml_buf, xml_size, bin_size ) == false )
	{
		std::string error = xml_metabuf.getError();

		printf( "error convert metabuf: %s"
			, error.c_str()
		);

		return 0;
	}

	const char * path_bin = argv[3];

	FILE * file_bin = fopen( path_bin, "wb" );
	fwrite( bin_buf, 1, bin_size, file_bin );
	fclose( file_bin );
		
	printf( "done\n" );

	return 0;
}