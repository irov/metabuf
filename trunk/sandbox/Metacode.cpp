#   include "Metacode.h"

namespace Metacode
{
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::generateMetadata( size_t _id )
    {
        switch( _id )
        {
        case 3:
            {
                return new Meta_ResourceEmitterContainer();
                break;
            }
        case 2:
            {
                return new Meta_ResourceImageDefault();
                break;
            }
        }
    
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 1:
            {
                Metabuf::Metadata * metadata = this->generateMetadata( _generators );
    
                Meta_DataBlock::Meta_Resource * metadata2 = static_cast<Meta_DataBlock::Meta_Resource *>(metadata);
                metadata2->parseNode( _buff, _size, _read );
    
                includes_Meta_Resource.push_back(metadata2);
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_Resource::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::Meta_Resource::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
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
    void Meta_DataBlock::Meta_Resource::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceEmitterContainer::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::Meta_ResourceEmitterContainer::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id );
    
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
    void Meta_DataBlock::Meta_ResourceEmitterContainer::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators );
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceImageDefault::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::Meta_ResourceImageDefault::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id );
    
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
    void Meta_DataBlock::Meta_ResourceImageDefault::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators );
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 2:
            {
                Meta_Pak::Meta_Resources metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Resources.push_back(metadata);
                break;
            }
        case 1:
            {
                Meta_Pak::Meta_Scripts metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Scripts.push_back(metadata);
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Resources::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::Meta_Resources::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Path );
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::Meta_Resources::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 2:
            {
                Meta_Pak::Meta_Resources::Meta_Resource metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Resource.push_back(metadata);
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Resources::Meta_Resource::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::Meta_Resources::Meta_Resource::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Name );
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::Meta_Resources::Meta_Resource::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Scripts::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::Meta_Scripts::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Path );
                break;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Pak::Meta_Scripts::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
    }
    
}
