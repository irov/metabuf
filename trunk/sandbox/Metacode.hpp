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
    public:
        Metadata * generateMetadata( size_t _id ) override;
        
        class Meta_Resource
            : public Metabuf::Metadata
        { 
        public:
            Meta_Resource()
                : Metabuf::Metadata()
            {
            }
        public:
            void setup_Name( std::string & _value )
            {
                _value = this->Name;
            }
            
            template<class C, class M>
            void setup_Name( C * _self, M _method )
            {
                (_self->*_method)( this->Name );
            }
            
            void setup_Type( std::string & _value )
            {
                _value = this->Type;
            }
            
            template<class C, class M>
            void setup_Type( C * _self, M _method )
            {
                (_self->*_method)( this->Type );
            }
            
        protected:
            void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            std::string Name;
            std::string Type;
        };
        
        class Meta_ResourceEmitterContainer
            : public Meta_Resource
        { 
        public:
            Meta_ResourceEmitterContainer()
                : Meta_Resource()
                , File_Path_successful(false)
                , Folder_Path_successful(false)
            {
            }
        public:
            void setup_File_Path( std::string & _value )
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
            
            void setup_Folder_Path( std::string & _value )
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
            void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool File_Path_successful;
            std::string File_Path;
            bool Folder_Path_successful;
            std::string Folder_Path;
        };
        
        class Meta_ResourceImageDefault
            : public Meta_Resource
        { 
        public:
            Meta_ResourceImageDefault()
                : Meta_Resource()
                , File_Codec_successful(false)
                , File_MaxSize_successful(false)
                , File_Path_successful(false)
            {
            }
        public:
            void setup_File_Codec( std::string & _value )
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
            
            void setup_File_Path( std::string & _value )
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
            void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool File_Codec_successful;
            std::string File_Codec;
            bool File_MaxSize_successful;
            mt::vec2f File_MaxSize;
            bool File_Path_successful;
            std::string File_Path;
        };
        
    protected:
    protected:
        typedef std::vector<Meta_Resource *> TVectorMeta_Resource;
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
    public:
        Metadata * generateMetadata( size_t _id ) override;
        
        class Meta_Resources
            : public Metabuf::Metadata
        { 
        public:
            Meta_Resources()
                : Metabuf::Metadata()
            {
            }
        public:
            void setup_Path( std::wstring & _value )
            {
                _value = this->Path;
            }
            
            template<class C, class M>
            void setup_Path( C * _self, M _method )
            {
                (_self->*_method)( this->Path );
            }
            
        protected:
            void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
            class Meta_Resource
                : public Metabuf::Metadata
            { 
            public:
                Meta_Resource()
                    : Metabuf::Metadata()
                {
                }
            public:
                void setup_Name( std::wstring & _value )
                {
                    _value = this->Name;
                }
                
                template<class C, class M>
                void setup_Name( C * _self, M _method )
                {
                    (_self->*_method)( this->Name );
                }
                
            protected:
                void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                std::wstring Name;
            };
            
        protected:
        protected:
            std::wstring Path;
            typedef std::vector<Meta_Resource> TVectorMeta_Resource;
            TVectorMeta_Resource includes_Meta_Resource;
        };
        
        class Meta_Scripts
            : public Metabuf::Metadata
        { 
        public:
            Meta_Scripts()
                : Metabuf::Metadata()
            {
            }
        public:
            void setup_Path( std::wstring & _value )
            {
                _value = this->Path;
            }
            
            template<class C, class M>
            void setup_Path( C * _self, M _method )
            {
                (_self->*_method)( this->Path );
            }
            
        protected:
            void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            std::wstring Path;
        };
        
    protected:
    protected:
        typedef std::vector<Meta_Resources> TVectorMeta_Resources;
        TVectorMeta_Resources includes_Meta_Resources;
        typedef std::vector<Meta_Scripts> TVectorMeta_Scripts;
        TVectorMeta_Scripts includes_Meta_Scripts;
    };
    
}
