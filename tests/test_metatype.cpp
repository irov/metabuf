#include "test_metatype.h"

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, bool & _value, void * _userData )
    {
        (void)_userData;

        uint8_t bool_value;
        ar.readPOD( bool_value );

        _value = !!bool_value;
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, uint8_t & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value );
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, uint16_t & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value );
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, uint32_t & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value );
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, float & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value );
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, std::string & _value, void * _userData )
    {
        Metabuf::Metacache * cache = static_cast<Metabuf::Metacache *>(_userData);

        uint32_t index;
        ar.readSize( index );

        _value = cache->strings[index];
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, Detail::Vec2f & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value.x );
        ar.readPOD( _value.y );
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, Detail::Vec3f & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value.x );
        ar.readPOD( _value.y );
        ar.readPOD( _value.z );
    }
    //////////////////////////////////////////////////////////////////////////
    void archive_read( Reader & ar, Detail::Rect & _value, void * _userData )
    {
        (void)_userData;

        ar.readPOD( _value.width );
        ar.readPOD( _value.height );
    }
    //////////////////////////////////////////////////////////////////////////
}