#   pragma once

#   include <metabuf/Metabuf.hpp>

#   include "Metatype.h"

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
        void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
        void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
            void attribute_Name( Menge::String & _value ) const
            {
                _value = this->Name;
            }
            
            template<class C, class M>
            void method_Name( C * _self, M _method ) const
            {
                (_self->*_method)( this->Name );
            }
            template<class C, class M, class A0>
            void method_Name( C * _self, M _method, const A0 & _a0 ) const
            {
                (_self->*_method)( this->Name, _a0 );
            }
            
            void attribute_Type( Menge::String & _value ) const
            {
                _value = this->Type;
            }
            
            template<class C, class M>
            void method_Type( C * _self, M _method ) const
            {
                (_self->*_method)( this->Type );
            }
            template<class C, class M, class A0>
            void method_Type( C * _self, M _method, const A0 & _a0 ) const
            {
                (_self->*_method)( this->Type, _a0 );
            }
            
        protected:
            void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::String Name;
            Menge::String Type;
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
            void setup_File_Path( Menge::String & _value )
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
            
            void setup_Folder_Path( Menge::String & _value )
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
            void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool File_Path_successful;
            Menge::String File_Path;
            bool Folder_Path_successful;
            Menge::String Folder_Path;
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
            void setup_File_Codec( Menge::String & _value )
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
            
            void setup_File_Path( Menge::String & _value )
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
            void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool File_Codec_successful;
            Menge::String File_Codec;
            bool File_MaxSize_successful;
            mt::vec2f File_MaxSize;
            bool File_Path_successful;
            Menge::String File_Path;
        };
        
    protected:
    protected:
    public:
        typedef std::vector<Meta_Resource *> TVectorMeta_Resource;
    
        const TVectorMeta_Resource & get_IncludesResource() const
        {
            return this->includes_Meta_Resource;
        }
    
    protected:
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
        void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
        void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
            void attribute_Path( Menge::WString & _value ) const
            {
                _value = this->Path;
            }
            
            template<class C, class M>
            void method_Path( C * _self, M _method ) const
            {
                (_self->*_method)( this->Path );
            }
            template<class C, class M, class A0>
            void method_Path( C * _self, M _method, const A0 & _a0 ) const
            {
                (_self->*_method)( this->Path, _a0 );
            }
            
        protected:
            void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
                void attribute_Name( Menge::ConstString & _value ) const
                {
                    _value = this->Name;
                }
                
                template<class C, class M>
                void method_Name( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Name );
                }
                template<class C, class M, class A0>
                void method_Name( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Name, _a0 );
                }
                
            protected:
                void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                Menge::ConstString Name;
            };
            
        protected:
        protected:
            Menge::WString Path;
        public:
            typedef std::vector<Meta_Resource> TVectorMeta_Resource;
        
            const TVectorMeta_Resource & get_IncludesResource() const
            {
                return this->includes_Meta_Resource;
            }
        
        protected:
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
            void attribute_Path( Menge::WString & _value ) const
            {
                _value = this->Path;
            }
            
            template<class C, class M>
            void method_Path( C * _self, M _method ) const
            {
                (_self->*_method)( this->Path );
            }
            template<class C, class M, class A0>
            void method_Path( C * _self, M _method, const A0 & _a0 ) const
            {
                (_self->*_method)( this->Path, _a0 );
            }
            
        protected:
            void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::WString Path;
        };
        
        class Meta_Texts
            : public Metabuf::Metadata
        { 
        public:
            Meta_Texts()
                : Metabuf::Metadata()
            {
            }
        public:
            void attribute_Path( Menge::WString & _value ) const
            {
                _value = this->Path;
            }
            
            template<class C, class M>
            void method_Path( C * _self, M _method ) const
            {
                (_self->*_method)( this->Path );
            }
            template<class C, class M, class A0>
            void method_Path( C * _self, M _method, const A0 & _a0 ) const
            {
                (_self->*_method)( this->Path, _a0 );
            }
            
        protected:
            void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
            class Meta_Text
                : public Metabuf::Metadata
            { 
            public:
                Meta_Text()
                    : Metabuf::Metadata()
                {
                }
            public:
                void attribute_File( Menge::WString & _value ) const
                {
                    _value = this->File;
                }
                
                template<class C, class M>
                void method_File( C * _self, M _method ) const
                {
                    (_self->*_method)( this->File );
                }
                template<class C, class M, class A0>
                void method_File( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->File, _a0 );
                }
                
                void attribute_Name( Menge::ConstString & _value ) const
                {
                    _value = this->Name;
                }
                
                template<class C, class M>
                void method_Name( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Name );
                }
                template<class C, class M, class A0>
                void method_Name( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Name, _a0 );
                }
                
            protected:
                void _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                void _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                Menge::WString File;
                Menge::ConstString Name;
            };
            
        protected:
        protected:
            Menge::WString Path;
        public:
            typedef std::vector<Meta_Text> TVectorMeta_Text;
        
            const TVectorMeta_Text & get_IncludesText() const
            {
                return this->includes_Meta_Text;
            }
        
        protected:
            TVectorMeta_Text includes_Meta_Text;
        };
        
    protected:
    protected:
    public:
        typedef std::vector<Meta_Resources> TVectorMeta_Resources;
    
        const TVectorMeta_Resources & get_IncludesResources() const
        {
            return this->includes_Meta_Resources;
        }
    
    protected:
        TVectorMeta_Resources includes_Meta_Resources;
    public:
        typedef std::vector<Meta_Scripts> TVectorMeta_Scripts;
    
        const TVectorMeta_Scripts & get_IncludesScripts() const
        {
            return this->includes_Meta_Scripts;
        }
    
    protected:
        TVectorMeta_Scripts includes_Meta_Scripts;
    public:
        typedef std::vector<Meta_Texts> TVectorMeta_Texts;
    
        const TVectorMeta_Texts & get_IncludesTexts() const
        {
            return this->includes_Meta_Texts;
        }
    
    protected:
        TVectorMeta_Texts includes_Meta_Texts;
    };
    
}
