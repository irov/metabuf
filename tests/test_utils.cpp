#include "test_utils.h"

void * read_file( const char * _current, const char * _path, size_t * _size )
{
    char full_path[256];
    sprintf( full_path, "%s/%s"
        , _current
        , _path
    );

    FILE * f = ::fopen( full_path, "rb" );

    if( f == nullptr )
    {
        return nullptr;
    }

    fseek( f, 0, SEEK_END );
    long size = ftell( f );
    rewind( f );

    void * buffer = malloc( size );

    fread( buffer, 1, size, f );

    fclose( f );

    *_size = (size_t)size;

    return buffer;
}
//////////////////////////////////////////////////////////////////////////
FILE * write_file( const char * _current, const char * _path )
{
    char full_path[256];
    sprintf( full_path, "%s/%s"
        , _current
        , _path
    );

    FILE * f = fopen( full_path, "wb" );

    return f;
}