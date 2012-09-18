#   pragma once

#   include <metabuf/Metabuf.hpp>

#   include "Metatype.h"

#   include <vector>
#   include <algorithm>

namespace Metacode
{
    bool readHeader( const char * _buff, size_t _size, size_t & _read, size_t & _readVersion, size_t & _needVersion );

}
