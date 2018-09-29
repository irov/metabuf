#include "Metacode.h"

#include "metabuf/ArchiveReader.hpp"

void main()
{
    FILE * file_test = fopen( "../sandbox/Pak.bin", "rb" );

    long size;

    fseek( file_test, 0, SEEK_END );
    size = ftell( file_test );
    fseek( file_test, 0, SEEK_SET );

    char * buf = new char[size];

    fread( buf, 1, size, file_test );

    fclose( file_test );

    Metacode::Meta_Pak * datablock = new Metacode::Meta_Pak();

    size_t read_size = 0;
    datablock->parse( buf, size, read_size );
}