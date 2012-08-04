#	include "XmlProtocol.hpp"
#	include "Xml2Metabuf.hpp"
#	include "Xml2Metacode.hpp"

#	include <stdio.h>

void main()
{
	Metabuf::XmlProtocol xml_protocol;

	FILE * file_protocol = fopen("protocol.xml", "rb");

	long size;

	fseek(file_protocol, 0, SEEK_END);
	size = ftell(file_protocol);
	fseek(file_protocol, 0, SEEK_SET);
	
	char * buf = new char[size];

	fread( buf, 1, size, file_protocol );
	
	fclose( file_protocol );

	xml_protocol.readProtocol( buf, size );
	
	Metabuf::Xml2Metacode xml_metacode(&xml_protocol);

	std::string header;
    std::string source;
	xml_metacode.generate( header, source );

	FILE * file_metacode_hpp = fopen("Metacode.hpp", "wb");

	fwrite( header.c_str(), header.size(), 1, file_metacode_hpp );
	fclose( file_metacode_hpp );

    FILE * file_metacode_cpp = fopen("Metacode.cpp", "wb");

    fwrite( source.c_str(), source.size(), 1, file_metacode_cpp );
    fclose( file_metacode_cpp );

	FILE * file_test = fopen("101_Fork.xml", "rb");

	long size_test;

	fseek(file_test, 0, SEEK_END);
	size_test = ftell(file_test);
	fseek(file_test, 0, SEEK_SET);

	char * buf_test = new char[size_test];

	fread( buf_test, 1, size_test, file_test );

	fclose( file_test );

	size_t write_size;
    
    char * write_buff = new char[size_test];

    Metabuf::Xml2Metabuf xml_metabuf(write_buff, &xml_protocol);

    xml_metabuf.initialize();
	xml_metabuf.convert( buf_test, size_test, write_size );

    FILE * file_test_bin = fopen("101_Fork.bin", "wb");

    fwrite( write_buff, write_size, 1, file_test_bin );
    fclose( file_test_bin );
}