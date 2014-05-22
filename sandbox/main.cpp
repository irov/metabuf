#	include "XmlProtocol.hpp"
#	include "Xml2Metabuf.hpp"
#	include "Xml2Metacode.hpp"

#	include <stdio.h>

void convert( const char * _xml, const char * _bin, Metabuf::XmlProtocol * _protocol );

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

	FILE * file_metacode_hpp = fopen("Metacode.h", "wb");

	fwrite( header.c_str(), header.size(), 1, file_metacode_hpp );
	fclose( file_metacode_hpp );

    FILE * file_metacode_cpp = fopen("Metacode.cpp", "wb");

    fwrite( source.c_str(), source.size(), 1, file_metacode_cpp );
    fclose( file_metacode_cpp );

    //convert("DebugInfo.xml", "DebugInfo.bin", &xml_protocol );
    //convert("Pak.xml", "Pak.bin", &xml_protocol );
}
//////////////////////////////////////////////////////////////////////////
void convert( const char * _xml, const char * _bin, Metabuf::XmlProtocol * _protocol )
{
	FILE * file_test = fopen(_xml, "rb");

	long size_test;

	fseek(file_test, 0, SEEK_END);
	size_test = ftell(file_test);
	fseek(file_test, 0, SEEK_SET);

	char * buf_test = new char[size_test];

	fread( buf_test, 1, size_test, file_test );

	fclose( file_test );

	size_t write_size;
    
    

    Metabuf::Xml2Metabuf xml_metabuf(_protocol);
    xml_metabuf.initialize();

    unsigned char * write_buff = new unsigned char[size_test * 2];
	if( xml_metabuf.convert( write_buff, size_test * 2, buf_test, size_test, write_size ) == false )
    {
        printf("Error: %s\n"
            , xml_metabuf.getError().c_str()
            );

        return;
    }

    FILE * file_test_bin = fopen(_bin, "wb");

    fwrite( write_buff, write_size, 1, file_test_bin );
    fclose( file_test_bin );
}