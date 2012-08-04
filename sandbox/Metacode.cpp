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
        case 2:
            {
                return new Meta_Resource();
                break;
            }
        case 4:
            {
                return new Meta_ResourceEmitterContainer();
                break;
            }
        case 3:
            {
                return new Meta_ResourceImageDefault();
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
    void Meta_Resource::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Name );
                break;
            }
        case 2:
            {
                this->read( _buff, _size, _read, this->Type );
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Resource::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceEmitterContainer::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_Resource::_parseArguments( _buff, _size, _read, _id );
    
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                this->File_Path_successful = true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->Folder_Path );
                this->Folder_Path_successful = true;
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceEmitterContainer::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators );
    
        switch( _includes )
        {
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceImageDefault::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_Resource::_parseArguments( _buff, _size, _read, _id );
    
        switch( _id )
        {
        case 4:
            {
                this->read( _buff, _size, _read, this->File_Codec );
                this->File_Codec_successful = true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->File_MaxSize );
                this->File_MaxSize_successful = true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                this->File_Path_successful = true;
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceImageDefault::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators );
    
        switch( _includes )
        {
        }
    }
    
}
