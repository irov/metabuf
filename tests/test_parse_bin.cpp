#include "test_metacode.h"

#include <string>
#include <vector>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

const char * path_example_bin = "Example.bin";

//////////////////////////////////////////////////////////////////////////
void * _metabuf_malloc( size_t _size )
{
    void * p = ::malloc( _size );

    return p;
}
//////////////////////////////////////////////////////////////////////////
void _metabuf_free( void * _ptr )
{
    ::free( _ptr );
}
//////////////////////////////////////////////////////////////////////////
int main( int argc, char * argv[] )
{
    METABUF_UNUSED( argc );
    METABUF_UNUSED( argv );

    char full_path_example_bin[256];
    sprintf( full_path_example_bin, "%s/%s"
        , argv[1]
        , path_example_bin
    );

    FILE * file_example_bin = ::fopen( full_path_example_bin, "rb" );

    if( file_example_bin == nullptr )
    {
        printf( "invalid open example: %s"
            , full_path_example_bin
        );

        return EXIT_FAILURE;
    }

    uint8_t header_buff[Metacode::header_size];
    fread( header_buff, Metacode::header_size, 1, file_example_bin );

    size_t header_read = 0;
    uint32_t readVersion;
    uint32_t needVersion;
    uint32_t readProtocol;
    uint32_t needProtocol;
    uint32_t metaMetaVersion = Metacode::Meta_Data::getVersion();
    uint32_t needMetaVersion;

    Metabuf::HeaderError result = Metacode::readHeader( header_buff, Metacode::header_size, header_read, readVersion, needVersion, readProtocol, needProtocol, metaMetaVersion, needMetaVersion );

    if( result != Metabuf::HEADER_SUCCESSFUL )
    {
        printf( "invalid header bin file '%s'"
            , path_example_bin
        );

        return EXIT_FAILURE;
    }

    uint32_t bin_size;
    fread( &bin_size, sizeof( bin_size ), 1, file_example_bin );

    uint8_t * example_bin_buffer = (uint8_t *)malloc( bin_size );

    fread( example_bin_buffer, bin_size, 1, file_example_bin );

    fclose( file_example_bin );

    size_t read_size = 0;

    uint32_t stringCount;
    if( Metacode::readStrings( example_bin_buffer, bin_size, read_size, stringCount ) == false )
    {
        printf( "error read strings bin file '%s'"
            , path_example_bin
        );

        return EXIT_FAILURE;
    }

    Metabuf::Metacache metacache;

    metacache.strings.resize( stringCount );

    for( std::string & s : metacache.strings )
    {
        uint32_t stringSize;
        int64_t stringHash;
        const char * str = Metacode::readString( example_bin_buffer, bin_size, read_size, stringSize, stringHash );

        s.assign( str, stringSize );
    }

    Metacode::Meta_Data::Meta_DataBlock meta_DataBlock;
    if( meta_DataBlock.parse( example_bin_buffer, bin_size, read_size, (void *)&metacache ) == false )
    {
        printf( "error parse root bin file '%s'"
            , path_example_bin
        );

        return EXIT_FAILURE;
    }

    metacache.strings.clear();

    free( example_bin_buffer );

    assert( meta_DataBlock.get_Name() == "Test" );

    const Metacode::Meta_Data::Meta_DataBlock::VectorMeta_Resource & meta_DataBlock_Resource = meta_DataBlock.get_Includes_Resource();

    for( const Metacode::Meta_Data::Meta_DataBlock::Meta_Resource * meta_Resource : meta_DataBlock_Resource )
    {
        assert( meta_Resource->get_Name() == "Battleground" );
        assert( meta_Resource->get_Type() == "ResourceImageDefault" );

        if( meta_Resource->get_Type() == "ResourceImageDefault" )
        {
            const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceImageDefault * meta_ResourceImageDefault = static_cast<const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceImageDefault *>(meta_Resource);
            METABUF_UNUSED( meta_ResourceImageDefault );

            assert( meta_ResourceImageDefault->get_File_Path() == "Art/Battleground.png" );
            assert( meta_ResourceImageDefault->get_File_MaxSize() == Detail::Rect( {867, 1536} ) );
        }
        else
        {
            assert( !"invalid DataBlock resource type" );

            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
