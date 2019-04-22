#pragma once

#include "metabuf/Metadata.hpp"

#include <string>
#include <vector>

namespace Detail
{
    struct Vec2f
    {
        float x;
        float y;
    };

    struct Vec3f
    {
        float x;
        float y;
        float z;
    };

    struct Rect
    {
        uint32_t width;
        uint32_t height;
    };

    inline bool operator == ( const Rect & l, const Rect & r )
    {
        return l.width == r.width && l.height == r.height;
    }
}

namespace Metabuf
{
    struct Metacache
    {
        std::vector<std::string> strings;
    };

    void archive_read( Reader & ar, bool & _value, void * _userData );
    void archive_read( Reader & ar, uint8_t & _value, void * _userData );
    void archive_read( Reader & ar, uint16_t & _value, void * _userData );
    void archive_read( Reader & ar, uint32_t & _value, void * _userData );
    void archive_read( Reader & ar, float & _value, void * _userData );
    void archive_read( Reader & ar, std::string & _value, void * _userData );
    void archive_read( Reader & ar, Detail::Vec2f & _value, void * _userData );
    void archive_read( Reader & ar, Detail::Vec3f & _value, void * _userData );
    void archive_read( Reader & ar, Detail::Rect & _value, void * _userData );
}