#	include "XmlProtocol.hpp"
#	include "Xml2Metabuf.hpp"

#	include <stdio.h>

static void * read_file( const char * _file, size_t * _size )
{
	FILE * file_protocol = fopen( _file, "rb" );

	if( file_protocol == nullptr )
	{
		return nullptr;
	}

	fseek( file_protocol, 0, SEEK_END );
	long size = ftell( file_protocol );
	fseek( file_protocol, 0, SEEK_SET );

	uint8_t * buf = new uint8_t[size];

	fread( buf, 1, size, file_protocol );

	fclose( file_protocol );

	*_size = (size_t)size;

	return (void *)buf;
}

auto f = []( auto a )
{
    printf( "%d", a );
};

template<class T>
void ff( T t, auto b )
{

}

int main( int argc, char *argv[] )
{
    f( 1 );

    ff<int>( 1, 2 );

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

	if( protocol_buf == nullptr )
	{
		printf( "invalid open protocol: %s"
			, path_protocol
		);

		return 0;
	}

	if( xml_protocol.readProtocol( protocol_buf, protocol_size ) == false )
	{
		std::string error = xml_protocol.getError();

		printf( "error read protocol '%s': %s"
			, path_protocol
			, error.c_str()
		);

		return 0;
	}
	
	Metabuf::Xml2Metabuf xml_metabuf(&xml_protocol);

	xml_metabuf.initialize();

	const char * path_xml = argv[2];

	size_t xml_size;
	void * xml_buf = read_file( path_xml, &xml_size );

	if( xml_buf == nullptr )
	{
		printf( "invalid open xml: %s"
			, path_xml
		);

		return 0;
	}

	size_t try_bin_size = xml_size * 2;
	uint8_t * bin_buf = new uint8_t[try_bin_size];
	
	size_t bin_size;
	if( xml_metabuf.convert( bin_buf, try_bin_size, xml_buf, xml_size, bin_size ) == false )
	{
		std::string error = xml_metabuf.getError();

		printf( "error convert metabuf '%s': %s"
			, path_xml
			, error.c_str()
		);

		return 0;
	}

	const char * path_bin = argv[3];

	FILE * file_bin = fopen( path_bin, "wb" );

	if( file_bin == nullptr )
	{
		printf( "invalid open bin '%s'"
			, path_bin
		);

		return 0;
	}

	fwrite( bin_buf, 1, bin_size, file_bin );
	fclose( file_bin );
		
	printf( "done\n" );

	return 0;
}