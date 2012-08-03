#   include "Metatype.hpp"
#   include "Metacode.hpp"

namespace Metacode
{
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_DataBlock::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 2:
            {
                Meta_Resource * metadata = new Meta_Resource();
                metadata->parse( _buff, _size, _read );
    
                includes_Meta_Resource.push_back(metadata);
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Resource::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->Name );
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->Type );
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_Resource::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceEmitterContainer::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_Resource::_parseArguments( _buff, _size, _read, _id );
    
        switch( _id )
        {
        case 10:
            {
                this->read( _buff, _size, _read, this->File_Path );
                this->File_Path_successful = true;
            }
        case 11:
            {
                this->read( _buff, _size, _read, this->Folder_Path );
                this->Folder_Path_successful = true;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceEmitterContainer::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_Resource::_parseIncludes( _buff, _size, _read, _id );
    
        switch( _id )
        {
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceImageDefault::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_Resource::_parseArguments( _buff, _size, _read, _id );
    
        switch( _id )
        {
        case 7:
            {
                this->read( _buff, _size, _read, this->File_Codec );
                this->File_Codec_successful = true;
            }
        case 8:
            {
                this->read( _buff, _size, _read, this->File_MaxSize );
                this->File_MaxSize_successful = true;
            }
        case 6:
            {
                this->read( _buff, _size, _read, this->File_Path );
                this->File_Path_successful = true;
            }
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    void Meta_ResourceImageDefault::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        Meta_Resource::_parseIncludes( _buff, _size, _read, _id );
    
        switch( _id )
        {
        }
    }
    
}
