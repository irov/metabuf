#   pragma once

#   include <Metabuf.hpp>

namespace Menge
{
    class DataBlock
        : public Metabuf::Metadata
    { 
    public:
        DataBlock()
            : Metabuf::Metadata()
        {
        }
    public:
    protected:
        void _parse( char * _buff, size_t _size, size_t & _read, size_t _id ) override
        {
            switch( _id )
            {
            }
        }
        
    protected:
    } 
    
    class Resource
        : public Metabuf::Metadata
    { 
    public:
        Resource()
            : Metabuf::Metadata()
        {
        }
    public:
        void setup_Name( Menge::ConstString & _value )
        {
            _value = this->Name;
        }
        
        template<class C, class M>
        void setup_Name( C * _self, M _method )
        {
            (_self->*_method)( this->Name );
        }
        
        void setup_Type( Menge::ConstString & _value )
        {
            _value = this->Type;
        }
        
        template<class C, class M>
        void setup_Type( C * _self, M _method )
        {
            (_self->*_method)( this->Type );
        }
        
    protected:
        void _parse( char * _buff, size_t _size, size_t & _read, size_t _id ) override
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
        
    protected:
        Menge::ConstString Name
        Menge::ConstString Type
    } 
    
    class ResourceEmitterContainer
        : public Resource
    { 
    public:
        ResourceEmitterContainer()
            : Resource()
            , File_Path_successful(false)
            , Folder_Path_successful(false)
        {
        }
    public:
        void setup_File_Path( Menge::WString & _value )
        {
            if( File_Path_successful == false )
            {
                return;
            }
        
            _value = this->File_Path;
        }
        
        template<class C, class M>
        void setup_File_Path( C * _self, M _method )
        {
            if( File_Path_successful == false )
            {
                return;
            }
        
            (_self->*_method)( this->File_Path );
        }
        
        void setup_Folder_Path( Menge::WString & _value )
        {
            if( Folder_Path_successful == false )
            {
                return;
            }
        
            _value = this->Folder_Path;
        }
        
        template<class C, class M>
        void setup_Folder_Path( C * _self, M _method )
        {
            if( Folder_Path_successful == false )
            {
                return;
            }
        
            (_self->*_method)( this->Folder_Path );
        }
        
    protected:
        void _parse( char * _buff, size_t _size, size_t & _read, size_t _id ) override
        {
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
        
    protected:
        bool File_Path_successful;
        Menge::WString File_Path;
        bool Folder_Path_successful;
        Menge::WString Folder_Path;
    } 
    
    class ResourceImageDefault
        : public Resource
    { 
    public:
        ResourceImageDefault()
            : Resource()
            , File_Codec_successful(false)
            , File_MaxSize_successful(false)
            , File_Path_successful(false)
        {
        }
    public:
        void setup_File_Codec( Menge::ConstString & _value )
        {
            if( File_Codec_successful == false )
            {
                return;
            }
        
            _value = this->File_Codec;
        }
        
        template<class C, class M>
        void setup_File_Codec( C * _self, M _method )
        {
            if( File_Codec_successful == false )
            {
                return;
            }
        
            (_self->*_method)( this->File_Codec );
        }
        
        void setup_File_MaxSize( mt::vec2f & _value )
        {
            if( File_MaxSize_successful == false )
            {
                return;
            }
        
            _value = this->File_MaxSize;
        }
        
        template<class C, class M>
        void setup_File_MaxSize( C * _self, M _method )
        {
            if( File_MaxSize_successful == false )
            {
                return;
            }
        
            (_self->*_method)( this->File_MaxSize );
        }
        
        void setup_File_Path( Menge::WString & _value )
        {
            if( File_Path_successful == false )
            {
                return;
            }
        
            _value = this->File_Path;
        }
        
        template<class C, class M>
        void setup_File_Path( C * _self, M _method )
        {
            if( File_Path_successful == false )
            {
                return;
            }
        
            (_self->*_method)( this->File_Path );
        }
        
    protected:
        void _parse( char * _buff, size_t _size, size_t & _read, size_t _id ) override
        {
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
        
    protected:
        bool File_Codec_successful;
        Menge::ConstString File_Codec;
        bool File_MaxSize_successful;
        mt::vec2f File_MaxSize;
        bool File_Path_successful;
        Menge::WString File_Path;
    } 
    
}
