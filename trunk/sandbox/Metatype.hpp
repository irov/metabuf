#   pragma once

#   include <string>

#   include <../../Mengine/src/Utils/Math/vec2.h>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    inline void operator >> ( ArchiveReader & ar, std::string & _value )
    {
        size_t size;
        ar.readSize( size );

        if( size == 0 )
        {
            return;
        }

        _value.resize( size + 1 );

        //Archive::value_type * buff = reinterpret_cast<Archive::value_type *>(str);
        ar.readBuffer( &_value[0], size );
    }
    //////////////////////////////////////////////////////////////////////////
    inline void operator >> ( ArchiveReader & ar, std::wstring & _value )
    {
        size_t size;
        ar.readSize( size );

        if( size == 0 )
        {
            return;
        }

        _value.resize( size );

        //Archive::value_type * buff = reinterpret_cast<Archive::value_type *>(str);
        ar.readBuffer( (char *)&_value[0], size * sizeof(wchar_t) );
    }
    //////////////////////////////////////////////////////////////////////////
    inline void operator >> ( ArchiveReader & ar, mt::vec2f & _value )
    {
        ar.readPOD( _value.x );
        ar.readPOD( _value.y );
    }
}