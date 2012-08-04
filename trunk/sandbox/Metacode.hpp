#   pragma once

#   include <Metabuf.hpp>

#   include "Metatype.hpp"

#   include <vector>

namespace Metacode
{
    class Meta_DataBlock
        : public Metabuf::Metadata
    { 
    public:
        Meta_DataBlock()
            : Metabuf::Metadata()
        {
        }
    public:
    protected:
        void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
        void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
    protected:
    protected:
        typedef std::vector<class Meta_Resource *> TVectorMeta_Resource;
        TVectorMeta_Resource includes_Meta_Resource;
    };
    
    class Meta_Pak
        : public Metabuf::Metadata
    { 
    public:
        Meta_Pak()
            : Metabuf::Metadata()
        {
        }
    public:
    protected:
        void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
        void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
    protected:
    protected:
        typedef std::vector<class Meta_Resources *> TVectorMeta_Resources;
        TVectorMeta_Resources includes_Meta_Resources;
        typedef std::vector<class Meta_Scripts *> TVectorMeta_Scripts;
        TVectorMeta_Scripts includes_Meta_Scripts;
    };
    
}
