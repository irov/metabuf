#pragma once

#include "pugixml.hpp"

namespace Metabuf
{
    class XmlCRC32
        : public pugi::xml_writer
    {
    public:
        XmlCRC32();
        ~XmlCRC32() override;

    public:
        uint32_t getCrc32() const;

    protected:
        void write( const void * data, size_t size ) override;

    protected:
        uint32_t m_crc32;
    };
}