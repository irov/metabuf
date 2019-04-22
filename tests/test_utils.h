#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void * read_file( const char * _current, const char * _path, size_t * _size );
FILE * write_file( const char * _current, const char * _path );