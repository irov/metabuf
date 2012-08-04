#   include "Metacode.hpp"

namespace Metacode
{
    //////////////////////////////////////////////////////////////////////////
    static Metabuf::Metadata * s_generatorMetadata( size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                return new Meta_DataBlock();
                break;
            }
        case 5:
            {
                return new Meta_Pak();
                break;
            }
        }
    
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 2:
            {
                Metabuf::Metadata * metadata = s_generatorMetadata( _generators );
    
                Meta_Resource * metadata2 = static_cast<Meta_Resource *>(metadata);
                metadata2->parseNode( _buff, _size, _read );
    
                includes_Meta_Resource.push_back(metadata2);
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 7:
            {
                Metabuf::Metadata * metadata = s_generatorMetadata( _generators );
    
                Meta_Resources * metadata2 = static_cast<Meta_Resources *>(metadata);
                metadata2->parseNode( _buff, _size, _read );
    
                includes_Meta_Resources.push_back(metadata2);
                break;
            }
        case 6:
            {
                Metabuf::Metadata * metadata = s_generatorMetadata( _generators );
    
                Meta_Scripts * metadata2 = static_cast<Meta_Scripts *>(metadata);
                metadata2->parseNode( _buff, _size, _read );
    
                includes_Meta_Scripts.push_back(metadata2);
                break;
            }
        }
    }
    
}
