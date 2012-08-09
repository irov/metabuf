#   pragma once

#   include <string>

#   include <../../../src/Utils/Math/vec2.h>

namespace Menge
{
    typedef std::string String;
    typedef std::wstring WString;

    typedef std::string ConstString;
}

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    inline void operator >> ( ArchiveReader & ar, Menge::String & _value )
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
    inline void operator >> ( ArchiveReader & ar, Menge::WString & _value )
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