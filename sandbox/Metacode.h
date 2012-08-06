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
        bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
        bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::String Name;
            Menge::String Type;
        };
        
        class Meta_ResourceAnimation
            : public Meta_Resource
        { 
        public:
            Meta_ResourceAnimation()
                : Meta_Resource()
            {
            }
        public:
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
            class Meta_Sequence
                : public Metabuf::Metadata
            { 
            public:
                Meta_Sequence()
                    : Metabuf::Metadata()
                {
                }
            public:
                void attribute_Delay( float & _value ) const
                {
                    _value = this->Delay;
                }
                
                template<class C, class M>
                void method_Delay( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Delay );
                }
                template<class C, class M, class A0>
                void method_Delay( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Delay, _a0 );
                }
                
                void attribute_ResourceImageName( Menge::ConstString & _value ) const
                {
                    _value = this->ResourceImageName;
                }
                
                template<class C, class M>
                void method_ResourceImageName( C * _self, M _method ) const
                {
                    (_self->*_method)( this->ResourceImageName );
                }
                template<class C, class M, class A0>
                void method_ResourceImageName( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->ResourceImageName, _a0 );
                }
                
            protected:
                bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                float Delay;
                Menge::ConstString ResourceImageName;
            };
            
        protected:
        protected:
        public:
            typedef std::vector<Meta_Sequence> TVectorMeta_Sequence;
        
            const TVectorMeta_Sequence & get_IncludesSequence() const
            {
                return this->includes_Meta_Sequence;
            }
        
        protected:
            TVectorMeta_Sequence includes_Meta_Sequence;
        };
        
        class Meta_ResourceCursorICO
            : public Meta_Resource
        { 
        public:
            Meta_ResourceCursorICO()
                : Meta_Resource()
            {
            }
        public:
            void setup_File_Path( Menge::WString & _value )
            {
                _value = this->File_Path;
            }
            
            template<class C, class M>
            void setup_File_Path( C * _self, M _method )
            {
                (_self->*_method)( this->File_Path );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::WString File_Path;
        };
        
        class Meta_ResourceCursorSystem
            : public Meta_Resource
        { 
        public:
            Meta_ResourceCursorSystem()
                : Meta_Resource()
            {
            }
        public:
            void setup_File_Path( Menge::WString & _value )
            {
                _value = this->File_Path;
            }
            
            template<class C, class M>
            void setup_File_Path( C * _self, M _method )
            {
                (_self->*_method)( this->File_Path );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::WString File_Path;
        };
        
        class Meta_ResourceEmitterContainer
            : public Meta_Resource
        { 
        public:
            Meta_ResourceEmitterContainer()
                : Meta_Resource()
            {
            }
        public:
            void setup_File_Path( Menge::String & _value )
            {
                _value = this->File_Path;
            }
            
            template<class C, class M>
            void setup_File_Path( C * _self, M _method )
            {
                (_self->*_method)( this->File_Path );
            }
            
            void setup_Folder_Path( Menge::String & _value )
            {
                _value = this->Folder_Path;
            }
            
            template<class C, class M>
            void setup_Folder_Path( C * _self, M _method )
            {
                (_self->*_method)( this->Folder_Path );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::String File_Path;
            Menge::String Folder_Path;
        };
        
        class Meta_ResourceFont
            : public Meta_Resource
        { 
        public:
            Meta_ResourceFont()
                : Meta_Resource()
                , Color_Value_successful(false)
                , OutlineImage_Path_successful(false)
            {
            }
        public:
            void setup_Color_Value( Menge::ColourValue & _value )
            {
                if( Color_Value_successful == false )
                {
                    return;
                }
            
                _value = this->Color_Value;
            }
            
            template<class C, class M>
            void setup_Color_Value( C * _self, M _method )
            {
                if( Color_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->Color_Value );
            }
            
            void setup_Image_Path( Menge::WString & _value )
            {
                _value = this->Image_Path;
            }
            
            template<class C, class M>
            void setup_Image_Path( C * _self, M _method )
            {
                (_self->*_method)( this->Image_Path );
            }
            
            void setup_OutlineImage_Path( Menge::WString & _value )
            {
                if( OutlineImage_Path_successful == false )
                {
                    return;
                }
            
                _value = this->OutlineImage_Path;
            }
            
            template<class C, class M>
            void setup_OutlineImage_Path( C * _self, M _method )
            {
                if( OutlineImage_Path_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->OutlineImage_Path );
            }
            
            void setup_ResourceGlyph_Name( Menge::ConstString & _value )
            {
                _value = this->ResourceGlyph_Name;
            }
            
            template<class C, class M>
            void setup_ResourceGlyph_Name( C * _self, M _method )
            {
                (_self->*_method)( this->ResourceGlyph_Name );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool Color_Value_successful;
            Menge::ColourValue Color_Value;
            Menge::WString Image_Path;
            bool OutlineImage_Path_successful;
            Menge::WString OutlineImage_Path;
            Menge::ConstString ResourceGlyph_Name;
        };
        
        class Meta_ResourceGlyph
            : public Meta_Resource
        { 
        public:
            Meta_ResourceGlyph()
                : Meta_Resource()
            {
            }
        public:
            void setup_GlyphPath_Path( Menge::WString & _value )
            {
                _value = this->GlyphPath_Path;
            }
            
            template<class C, class M>
            void setup_GlyphPath_Path( C * _self, M _method )
            {
                (_self->*_method)( this->GlyphPath_Path );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::WString GlyphPath_Path;
        };
        
        class Meta_ResourceImageDefault
            : public Meta_Resource
        { 
        public:
            Meta_ResourceImageDefault()
                : Meta_Resource()
                , File_Codec_successful(false)
                , File_MaxSize_successful(false)
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
                _value = this->File_Path;
            }
            
            template<class C, class M>
            void setup_File_Path( C * _self, M _method )
            {
                (_self->*_method)( this->File_Path );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool File_Codec_successful;
            Menge::String File_Codec;
            bool File_MaxSize_successful;
            mt::vec2f File_MaxSize;
            Menge::String File_Path;
        };
        
        class Meta_ResourceImageSolid
            : public Meta_Resource
        { 
        public:
            Meta_ResourceImageSolid()
                : Meta_Resource()
            {
            }
        public:
            void setup_Color_Value( Menge::ColourValue & _value )
            {
                _value = this->Color_Value;
            }
            
            template<class C, class M>
            void setup_Color_Value( C * _self, M _method )
            {
                (_self->*_method)( this->Color_Value );
            }
            
            void setup_Size_Value( mt::vec2f & _value )
            {
                _value = this->Size_Value;
            }
            
            template<class C, class M>
            void setup_Size_Value( C * _self, M _method )
            {
                (_self->*_method)( this->Size_Value );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::ColourValue Color_Value;
            mt::vec2f Size_Value;
        };
        
        class Meta_ResourceInternalObject
            : public Meta_Resource
        { 
        public:
            Meta_ResourceInternalObject()
                : Meta_Resource()
            {
            }
        public:
            void setup_Internal_Group( Menge::ConstString & _value )
            {
                _value = this->Internal_Group;
            }
            
            template<class C, class M>
            void setup_Internal_Group( C * _self, M _method )
            {
                (_self->*_method)( this->Internal_Group );
            }
            
            void setup_Internal_Name( Menge::ConstString & _value )
            {
                _value = this->Internal_Name;
            }
            
            template<class C, class M>
            void setup_Internal_Name( C * _self, M _method )
            {
                (_self->*_method)( this->Internal_Name );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            Menge::ConstString Internal_Group;
            Menge::ConstString Internal_Name;
        };
        
        class Meta_ResourceMovie
            : public Meta_Resource
        { 
        public:
            Meta_ResourceMovie()
                : Meta_Resource()
                , Duration_Value_successful(false)
                , FrameDuration_Value_successful(false)
                , Height_Value_successful(false)
                , KeyFramesPackPath_Path_successful(false)
                , Width_Value_successful(false)
            {
            }
        public:
            void setup_Duration_Value( float & _value )
            {
                if( Duration_Value_successful == false )
                {
                    return;
                }
            
                _value = this->Duration_Value;
            }
            
            template<class C, class M>
            void setup_Duration_Value( C * _self, M _method )
            {
                if( Duration_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->Duration_Value );
            }
            
            void setup_FrameDuration_Value( float & _value )
            {
                if( FrameDuration_Value_successful == false )
                {
                    return;
                }
            
                _value = this->FrameDuration_Value;
            }
            
            template<class C, class M>
            void setup_FrameDuration_Value( C * _self, M _method )
            {
                if( FrameDuration_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->FrameDuration_Value );
            }
            
            void setup_Height_Value( float & _value )
            {
                if( Height_Value_successful == false )
                {
                    return;
                }
            
                _value = this->Height_Value;
            }
            
            template<class C, class M>
            void setup_Height_Value( C * _self, M _method )
            {
                if( Height_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->Height_Value );
            }
            
            void setup_KeyFramesPackPath_Path( Menge::WString & _value )
            {
                if( KeyFramesPackPath_Path_successful == false )
                {
                    return;
                }
            
                _value = this->KeyFramesPackPath_Path;
            }
            
            template<class C, class M>
            void setup_KeyFramesPackPath_Path( C * _self, M _method )
            {
                if( KeyFramesPackPath_Path_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->KeyFramesPackPath_Path );
            }
            
            void setup_Width_Value( float & _value )
            {
                if( Width_Value_successful == false )
                {
                    return;
                }
            
                _value = this->Width_Value;
            }
            
            template<class C, class M>
            void setup_Width_Value( C * _self, M _method )
            {
                if( Width_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->Width_Value );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
            class Meta_MovieCamera3D
                : public Metabuf::Metadata
            { 
            public:
                Meta_MovieCamera3D()
                    : Metabuf::Metadata()
                {
                }
            public:
                void attribute_CameraAspect( float & _value ) const
                {
                    _value = this->CameraAspect;
                }
                
                template<class C, class M>
                void method_CameraAspect( C * _self, M _method ) const
                {
                    (_self->*_method)( this->CameraAspect );
                }
                template<class C, class M, class A0>
                void method_CameraAspect( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->CameraAspect, _a0 );
                }
                
                void attribute_CameraFOV( float & _value ) const
                {
                    _value = this->CameraFOV;
                }
                
                template<class C, class M>
                void method_CameraFOV( C * _self, M _method ) const
                {
                    (_self->*_method)( this->CameraFOV );
                }
                template<class C, class M, class A0>
                void method_CameraFOV( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->CameraFOV, _a0 );
                }
                
                void attribute_CameraInterest( mt::vec3f & _value ) const
                {
                    _value = this->CameraInterest;
                }
                
                template<class C, class M>
                void method_CameraInterest( C * _self, M _method ) const
                {
                    (_self->*_method)( this->CameraInterest );
                }
                template<class C, class M, class A0>
                void method_CameraInterest( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->CameraInterest, _a0 );
                }
                
                void attribute_CameraPosition( mt::vec3f & _value ) const
                {
                    _value = this->CameraPosition;
                }
                
                template<class C, class M>
                void method_CameraPosition( C * _self, M _method ) const
                {
                    (_self->*_method)( this->CameraPosition );
                }
                template<class C, class M, class A0>
                void method_CameraPosition( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->CameraPosition, _a0 );
                }
                
                void attribute_Height( float & _value ) const
                {
                    _value = this->Height;
                }
                
                template<class C, class M>
                void method_Height( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Height );
                }
                template<class C, class M, class A0>
                void method_Height( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Height, _a0 );
                }
                
                void attribute_Width( float & _value ) const
                {
                    _value = this->Width;
                }
                
                template<class C, class M>
                void method_Width( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Width );
                }
                template<class C, class M, class A0>
                void method_Width( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Width, _a0 );
                }
                
            protected:
                bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                float CameraAspect;
                float CameraFOV;
                mt::vec3f CameraInterest;
                mt::vec3f CameraPosition;
                float Height;
                float Width;
            };
            
            class Meta_MovieLayer2D
                : public Metabuf::Metadata
            { 
            public:
                Meta_MovieLayer2D()
                    : Metabuf::Metadata()
                {
                }
            public:
                void attribute_BlendingMode( Menge::ConstString & _value ) const
                {
                    _value = this->BlendingMode;
                }
                
                template<class C, class M>
                void method_BlendingMode( C * _self, M _method ) const
                {
                    (_self->*_method)( this->BlendingMode );
                }
                template<class C, class M, class A0>
                void method_BlendingMode( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->BlendingMode, _a0 );
                }
                
                void attribute_In( float & _value ) const
                {
                    _value = this->In;
                }
                
                template<class C, class M>
                void method_In( C * _self, M _method ) const
                {
                    (_self->*_method)( this->In );
                }
                template<class C, class M, class A0>
                void method_In( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->In, _a0 );
                }
                
                void attribute_Index( size_t & _value ) const
                {
                    _value = this->Index;
                }
                
                template<class C, class M>
                void method_Index( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Index );
                }
                template<class C, class M, class A0>
                void method_Index( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Index, _a0 );
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
                
                void attribute_Out( float & _value ) const
                {
                    _value = this->Out;
                }
                
                template<class C, class M>
                void method_Out( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Out );
                }
                template<class C, class M, class A0>
                void method_Out( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Out, _a0 );
                }
                
                void attribute_Parent( size_t & _value ) const
                {
                    _value = this->Parent;
                }
                
                template<class C, class M>
                void method_Parent( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Parent );
                }
                template<class C, class M, class A0>
                void method_Parent( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Parent, _a0 );
                }
                
                void attribute_Source( Menge::ConstString & _value ) const
                {
                    _value = this->Source;
                }
                
                template<class C, class M>
                void method_Source( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Source );
                }
                template<class C, class M, class A0>
                void method_Source( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Source, _a0 );
                }
                
                void attribute_StartInterval( float & _value ) const
                {
                    _value = this->StartInterval;
                }
                
                template<class C, class M>
                void method_StartInterval( C * _self, M _method ) const
                {
                    (_self->*_method)( this->StartInterval );
                }
                template<class C, class M, class A0>
                void method_StartInterval( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->StartInterval, _a0 );
                }
                
            protected:
                bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                Menge::ConstString BlendingMode;
                float In;
                size_t Index;
                Menge::ConstString Name;
                float Out;
                size_t Parent;
                Menge::ConstString Source;
                float StartInterval;
            };
            
            class Meta_MovieLayer3D
                : public Metabuf::Metadata
            { 
            public:
                Meta_MovieLayer3D()
                    : Metabuf::Metadata()
                {
                }
            public:
                void attribute_BlendingMode( Menge::ConstString & _value ) const
                {
                    _value = this->BlendingMode;
                }
                
                template<class C, class M>
                void method_BlendingMode( C * _self, M _method ) const
                {
                    (_self->*_method)( this->BlendingMode );
                }
                template<class C, class M, class A0>
                void method_BlendingMode( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->BlendingMode, _a0 );
                }
                
                void attribute_In( float & _value ) const
                {
                    _value = this->In;
                }
                
                template<class C, class M>
                void method_In( C * _self, M _method ) const
                {
                    (_self->*_method)( this->In );
                }
                template<class C, class M, class A0>
                void method_In( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->In, _a0 );
                }
                
                void attribute_Index( size_t & _value ) const
                {
                    _value = this->Index;
                }
                
                template<class C, class M>
                void method_Index( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Index );
                }
                template<class C, class M, class A0>
                void method_Index( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Index, _a0 );
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
                
                void attribute_Out( float & _value ) const
                {
                    _value = this->Out;
                }
                
                template<class C, class M>
                void method_Out( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Out );
                }
                template<class C, class M, class A0>
                void method_Out( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Out, _a0 );
                }
                
                void attribute_Parent( size_t & _value ) const
                {
                    _value = this->Parent;
                }
                
                template<class C, class M>
                void method_Parent( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Parent );
                }
                template<class C, class M, class A0>
                void method_Parent( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Parent, _a0 );
                }
                
                void attribute_Source( Menge::ConstString & _value ) const
                {
                    _value = this->Source;
                }
                
                template<class C, class M>
                void method_Source( C * _self, M _method ) const
                {
                    (_self->*_method)( this->Source );
                }
                template<class C, class M, class A0>
                void method_Source( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->Source, _a0 );
                }
                
                void attribute_StartInterval( float & _value ) const
                {
                    _value = this->StartInterval;
                }
                
                template<class C, class M>
                void method_StartInterval( C * _self, M _method ) const
                {
                    (_self->*_method)( this->StartInterval );
                }
                template<class C, class M, class A0>
                void method_StartInterval( C * _self, M _method, const A0 & _a0 ) const
                {
                    (_self->*_method)( this->StartInterval, _a0 );
                }
                
            protected:
                bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                Menge::ConstString BlendingMode;
                float In;
                size_t Index;
                Menge::ConstString Name;
                float Out;
                size_t Parent;
                Menge::ConstString Source;
                float StartInterval;
            };
            
        protected:
        protected:
            bool Duration_Value_successful;
            float Duration_Value;
            bool FrameDuration_Value_successful;
            float FrameDuration_Value;
            bool Height_Value_successful;
            float Height_Value;
            bool KeyFramesPackPath_Path_successful;
            Menge::WString KeyFramesPackPath_Path;
            bool Width_Value_successful;
            float Width_Value;
        public:
            typedef std::vector<Meta_MovieCamera3D> TVectorMeta_MovieCamera3D;
        
            const TVectorMeta_MovieCamera3D & get_IncludesMovieCamera3D() const
            {
                return this->includes_Meta_MovieCamera3D;
            }
        
        protected:
            TVectorMeta_MovieCamera3D includes_Meta_MovieCamera3D;
        public:
            typedef std::vector<Meta_MovieLayer2D> TVectorMeta_MovieLayer2D;
        
            const TVectorMeta_MovieLayer2D & get_IncludesMovieLayer2D() const
            {
                return this->includes_Meta_MovieLayer2D;
            }
        
        protected:
            TVectorMeta_MovieLayer2D includes_Meta_MovieLayer2D;
        public:
            typedef std::vector<Meta_MovieLayer3D> TVectorMeta_MovieLayer3D;
        
            const TVectorMeta_MovieLayer3D & get_IncludesMovieLayer3D() const
            {
                return this->includes_Meta_MovieLayer3D;
            }
        
        protected:
            TVectorMeta_MovieLayer3D includes_Meta_MovieLayer3D;
        };
        
        class Meta_ResourcePlaylist
            : public Meta_Resource
        { 
        public:
            Meta_ResourcePlaylist()
                : Meta_Resource()
                , Loop_Value_successful(false)
                , Shuffle_Value_successful(false)
            {
            }
        public:
            void setup_Loop_Value( bool & _value )
            {
                if( Loop_Value_successful == false )
                {
                    return;
                }
            
                _value = this->Loop_Value;
            }
            
            template<class C, class M>
            void setup_Loop_Value( C * _self, M _method )
            {
                if( Loop_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->Loop_Value );
            }
            
            void setup_Shuffle_Value( bool & _value )
            {
                if( Shuffle_Value_successful == false )
                {
                    return;
                }
            
                _value = this->Shuffle_Value;
            }
            
            template<class C, class M>
            void setup_Shuffle_Value( C * _self, M _method )
            {
                if( Shuffle_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->Shuffle_Value );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
            class Meta_Tracks
                : public Metabuf::Metadata
            { 
            public:
                Meta_Tracks()
                    : Metabuf::Metadata()
                {
                }
            public:
            protected:
                bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
                class Meta_Track
                    : public Metabuf::Metadata
                { 
                public:
                    Meta_Track()
                        : Metabuf::Metadata()
                        , Codec_successful(false)
                    {
                    }
                public:
                    void attribute_Codec( Menge::ConstString & _value ) const
                    {
                        if( Codec_successful == false )
                        {
                            return;
                        }
                    
                        _value = this->Codec;
                    }
                    
                    template<class C, class M>
                    void method_Codec( C * _self, M _method ) const
                    {
                        if( Codec_successful == false )
                        {
                            return;
                        }
                    
                        (_self->*_method)( this->Codec );
                    }
                    template<class C, class M, class A0>
                    void method_Codec( C * _self, M _method, const A0 & _a0 ) const
                    {
                        if( Codec_successful == false )
                        {
                            return;
                        }
                    
                        (_self->*_method)( this->Codec, _a0 );
                    }
                    
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
                    
                protected:
                    bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                    bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
                public:
                    Metadata * generateMetadata( size_t _id ) override;
                    
                protected:
                protected:
                    bool Codec_successful;
                    Menge::ConstString Codec;
                    Menge::WString File;
                };
                
            protected:
            protected:
            public:
                typedef std::vector<Meta_Track> TVectorMeta_Track;
            
                const TVectorMeta_Track & get_IncludesTrack() const
                {
                    return this->includes_Meta_Track;
                }
            
            protected:
                TVectorMeta_Track includes_Meta_Track;
            };
            
        protected:
        protected:
            bool Loop_Value_successful;
            bool Loop_Value;
            bool Shuffle_Value_successful;
            bool Shuffle_Value;
        public:
            typedef std::vector<Meta_Tracks> TVectorMeta_Tracks;
        
            const TVectorMeta_Tracks & get_IncludesTracks() const
            {
                return this->includes_Meta_Tracks;
            }
        
        protected:
            TVectorMeta_Tracks includes_Meta_Tracks;
        };
        
        class Meta_ResourceSound
            : public Meta_Resource
        { 
        public:
            Meta_ResourceSound()
                : Meta_Resource()
                , DefaultVolume_Value_successful(false)
                , File_Codec_successful(false)
                , File_Converter_successful(false)
                , IsStreamable_Value_successful(false)
            {
            }
        public:
            void setup_DefaultVolume_Value( float & _value )
            {
                if( DefaultVolume_Value_successful == false )
                {
                    return;
                }
            
                _value = this->DefaultVolume_Value;
            }
            
            template<class C, class M>
            void setup_DefaultVolume_Value( C * _self, M _method )
            {
                if( DefaultVolume_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->DefaultVolume_Value );
            }
            
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
            
            void setup_File_Converter( Menge::ConstString & _value )
            {
                if( File_Converter_successful == false )
                {
                    return;
                }
            
                _value = this->File_Converter;
            }
            
            template<class C, class M>
            void setup_File_Converter( C * _self, M _method )
            {
                if( File_Converter_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->File_Converter );
            }
            
            void setup_File_Path( Menge::WString & _value )
            {
                _value = this->File_Path;
            }
            
            template<class C, class M>
            void setup_File_Path( C * _self, M _method )
            {
                (_self->*_method)( this->File_Path );
            }
            
            void setup_IsStreamable_Value( bool & _value )
            {
                if( IsStreamable_Value_successful == false )
                {
                    return;
                }
            
                _value = this->IsStreamable_Value;
            }
            
            template<class C, class M>
            void setup_IsStreamable_Value( C * _self, M _method )
            {
                if( IsStreamable_Value_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->IsStreamable_Value );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool DefaultVolume_Value_successful;
            float DefaultVolume_Value;
            bool File_Codec_successful;
            Menge::ConstString File_Codec;
            bool File_Converter_successful;
            Menge::ConstString File_Converter;
            Menge::WString File_Path;
            bool IsStreamable_Value_successful;
            bool IsStreamable_Value;
        };
        
        class Meta_ResourceVideo
            : public Meta_Resource
        { 
        public:
            Meta_ResourceVideo()
                : Meta_Resource()
                , File_Alpha_successful(false)
                , File_Codec_successful(false)
            {
            }
        public:
            void setup_File_Alpha( bool & _value )
            {
                if( File_Alpha_successful == false )
                {
                    return;
                }
            
                _value = this->File_Alpha;
            }
            
            template<class C, class M>
            void setup_File_Alpha( C * _self, M _method )
            {
                if( File_Alpha_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->File_Alpha );
            }
            
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
            
            void setup_File_Path( Menge::WString & _value )
            {
                _value = this->File_Path;
            }
            
            template<class C, class M>
            void setup_File_Path( C * _self, M _method )
            {
                (_self->*_method)( this->File_Path );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool File_Alpha_successful;
            bool File_Alpha;
            bool File_Codec_successful;
            Menge::ConstString File_Codec;
            Menge::WString File_Path;
        };
        
        class Meta_ResourceWindow
            : public Meta_Resource
        { 
        public:
            Meta_ResourceWindow()
                : Meta_Resource()
                , WindowBackground_ResourceImageName_successful(false)
                , WindowBottom_Offset_successful(false)
                , WindowBottom_ResourceImageName_successful(false)
                , WindowLeft_Offset_successful(false)
                , WindowLeft_ResourceImageName_successful(false)
                , WindowLeftBottom_Offset_successful(false)
                , WindowLeftBottom_ResourceImageName_successful(false)
                , WindowLeftTop_Offset_successful(false)
                , WindowLeftTop_ResourceImageName_successful(false)
                , WindowRight_Offset_successful(false)
                , WindowRight_ResourceImageName_successful(false)
                , WindowRightBottom_Offset_successful(false)
                , WindowRightBottom_ResourceImageName_successful(false)
                , WindowRightTop_Offset_successful(false)
                , WindowRightTop_ResourceImageName_successful(false)
                , WindowTop_Offset_successful(false)
                , WindowTop_ResourceImageName_successful(false)
            {
            }
        public:
            void setup_WindowBackground_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowBackground_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowBackground_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowBackground_ResourceImageName( C * _self, M _method )
            {
                if( WindowBackground_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowBackground_ResourceImageName );
            }
            
            void setup_WindowBottom_Offset( mt::vec2f & _value )
            {
                if( WindowBottom_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowBottom_Offset;
            }
            
            template<class C, class M>
            void setup_WindowBottom_Offset( C * _self, M _method )
            {
                if( WindowBottom_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowBottom_Offset );
            }
            
            void setup_WindowBottom_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowBottom_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowBottom_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowBottom_ResourceImageName( C * _self, M _method )
            {
                if( WindowBottom_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowBottom_ResourceImageName );
            }
            
            void setup_WindowLeft_Offset( mt::vec2f & _value )
            {
                if( WindowLeft_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowLeft_Offset;
            }
            
            template<class C, class M>
            void setup_WindowLeft_Offset( C * _self, M _method )
            {
                if( WindowLeft_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowLeft_Offset );
            }
            
            void setup_WindowLeft_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowLeft_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowLeft_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowLeft_ResourceImageName( C * _self, M _method )
            {
                if( WindowLeft_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowLeft_ResourceImageName );
            }
            
            void setup_WindowLeftBottom_Offset( mt::vec2f & _value )
            {
                if( WindowLeftBottom_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowLeftBottom_Offset;
            }
            
            template<class C, class M>
            void setup_WindowLeftBottom_Offset( C * _self, M _method )
            {
                if( WindowLeftBottom_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowLeftBottom_Offset );
            }
            
            void setup_WindowLeftBottom_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowLeftBottom_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowLeftBottom_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowLeftBottom_ResourceImageName( C * _self, M _method )
            {
                if( WindowLeftBottom_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowLeftBottom_ResourceImageName );
            }
            
            void setup_WindowLeftTop_Offset( mt::vec2f & _value )
            {
                if( WindowLeftTop_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowLeftTop_Offset;
            }
            
            template<class C, class M>
            void setup_WindowLeftTop_Offset( C * _self, M _method )
            {
                if( WindowLeftTop_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowLeftTop_Offset );
            }
            
            void setup_WindowLeftTop_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowLeftTop_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowLeftTop_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowLeftTop_ResourceImageName( C * _self, M _method )
            {
                if( WindowLeftTop_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowLeftTop_ResourceImageName );
            }
            
            void setup_WindowRight_Offset( mt::vec2f & _value )
            {
                if( WindowRight_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowRight_Offset;
            }
            
            template<class C, class M>
            void setup_WindowRight_Offset( C * _self, M _method )
            {
                if( WindowRight_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowRight_Offset );
            }
            
            void setup_WindowRight_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowRight_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowRight_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowRight_ResourceImageName( C * _self, M _method )
            {
                if( WindowRight_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowRight_ResourceImageName );
            }
            
            void setup_WindowRightBottom_Offset( mt::vec2f & _value )
            {
                if( WindowRightBottom_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowRightBottom_Offset;
            }
            
            template<class C, class M>
            void setup_WindowRightBottom_Offset( C * _self, M _method )
            {
                if( WindowRightBottom_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowRightBottom_Offset );
            }
            
            void setup_WindowRightBottom_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowRightBottom_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowRightBottom_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowRightBottom_ResourceImageName( C * _self, M _method )
            {
                if( WindowRightBottom_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowRightBottom_ResourceImageName );
            }
            
            void setup_WindowRightTop_Offset( mt::vec2f & _value )
            {
                if( WindowRightTop_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowRightTop_Offset;
            }
            
            template<class C, class M>
            void setup_WindowRightTop_Offset( C * _self, M _method )
            {
                if( WindowRightTop_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowRightTop_Offset );
            }
            
            void setup_WindowRightTop_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowRightTop_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowRightTop_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowRightTop_ResourceImageName( C * _self, M _method )
            {
                if( WindowRightTop_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowRightTop_ResourceImageName );
            }
            
            void setup_WindowTop_Offset( mt::vec2f & _value )
            {
                if( WindowTop_Offset_successful == false )
                {
                    return;
                }
            
                _value = this->WindowTop_Offset;
            }
            
            template<class C, class M>
            void setup_WindowTop_Offset( C * _self, M _method )
            {
                if( WindowTop_Offset_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowTop_Offset );
            }
            
            void setup_WindowTop_ResourceImageName( Menge::ConstString & _value )
            {
                if( WindowTop_ResourceImageName_successful == false )
                {
                    return;
                }
            
                _value = this->WindowTop_ResourceImageName;
            }
            
            template<class C, class M>
            void setup_WindowTop_ResourceImageName( C * _self, M _method )
            {
                if( WindowTop_ResourceImageName_successful == false )
                {
                    return;
                }
            
                (_self->*_method)( this->WindowTop_ResourceImageName );
            }
            
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
        public:
            Metadata * generateMetadata( size_t _id ) override;
            
        protected:
        protected:
            bool WindowBackground_ResourceImageName_successful;
            Menge::ConstString WindowBackground_ResourceImageName;
            bool WindowBottom_Offset_successful;
            mt::vec2f WindowBottom_Offset;
            bool WindowBottom_ResourceImageName_successful;
            Menge::ConstString WindowBottom_ResourceImageName;
            bool WindowLeft_Offset_successful;
            mt::vec2f WindowLeft_Offset;
            bool WindowLeft_ResourceImageName_successful;
            Menge::ConstString WindowLeft_ResourceImageName;
            bool WindowLeftBottom_Offset_successful;
            mt::vec2f WindowLeftBottom_Offset;
            bool WindowLeftBottom_ResourceImageName_successful;
            Menge::ConstString WindowLeftBottom_ResourceImageName;
            bool WindowLeftTop_Offset_successful;
            mt::vec2f WindowLeftTop_Offset;
            bool WindowLeftTop_ResourceImageName_successful;
            Menge::ConstString WindowLeftTop_ResourceImageName;
            bool WindowRight_Offset_successful;
            mt::vec2f WindowRight_Offset;
            bool WindowRight_ResourceImageName_successful;
            Menge::ConstString WindowRight_ResourceImageName;
            bool WindowRightBottom_Offset_successful;
            mt::vec2f WindowRightBottom_Offset;
            bool WindowRightBottom_ResourceImageName_successful;
            Menge::ConstString WindowRightBottom_ResourceImageName;
            bool WindowRightTop_Offset_successful;
            mt::vec2f WindowRightTop_Offset;
            bool WindowRightTop_ResourceImageName_successful;
            Menge::ConstString WindowRightTop_ResourceImageName;
            bool WindowTop_Offset_successful;
            mt::vec2f WindowTop_Offset;
            bool WindowTop_ResourceImageName_successful;
            Menge::ConstString WindowTop_ResourceImageName;
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
        bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
        bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
                bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                Menge::WString Path;
            };
            
        protected:
        protected:
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
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
        protected:
            bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
            bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
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
                bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;
                bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators ) override;
            public:
                Metadata * generateMetadata( size_t _id ) override;
                
            protected:
            protected:
                Menge::WString Path;
            };
            
        protected:
        protected:
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
