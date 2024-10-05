#pragma once

#include "test_metatype.h"

#include "metabuf/Metadata.hpp"

namespace Metacode
{
    const size_t header_size = 20;

    typedef uint32_t enum_t;

    uint32_t get_metacode_magic();
    uint32_t get_metacode_version();
    uint32_t get_metacode_protocol_version();
    uint32_t get_metacode_protocol_crc32();

    const char * getHeaderErrorMessage( Metabuf::HeaderError _error );

    Metabuf::HeaderError readHeader( const void * _buff, size_t _size, size_t & _read, uint32_t & _readVersion, uint32_t & _needVersion, uint32_t & _readProtocol, uint32_t & _needProtocol, uint32_t _metaVersion, uint32_t & _readMetaVersion );

    uint32_t getInternalStringsCount();
    const char * getInternalString( uint32_t _index, uint32_t & _stringSize, int64_t & _stringHash );
    bool readStrings( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringCount );
    const char * readString( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringSize, int64_t & _stringHash );

    namespace Meta_Data
    {
        uint32_t getVersion();
    
        class Meta_DataBlock
            : public Metabuf::Metaparse
        { 
        public:
            Meta_DataBlock();
            ~Meta_DataBlock();
        
        public:
            template<class C, class M>
            void getm_Name( C _self, M _method ) const
            {
                (_self->*_method)( this->m_Name );
            }
            
            const std::string & get_Name() const
            {
                return this->m_Name;
            }
            
        public:
            bool parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData ) override;
        
        protected:
            void _parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );
            void _preparationIncludes( uint32_t _id, uint32_t _count );
            void _parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );
            void _parseGenerators( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );
        
        public:
            class Meta_Include
                : public Metabuf::Metaparse
            { 
            public:
                Meta_Include();
            
            public:
                template<class C, class M>
                void getm_Path( C _self, M _method ) const
                {
                    (_self->*_method)( this->m_Path );
                }
                
                const std::string & get_Path() const
                {
                    return this->m_Path;
                }
                
            public:
                bool parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData ) override;
            
            protected:
                void _parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );
                void _preparationIncludes( uint32_t _id, uint32_t _count );
                void _parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );
            
            public:
            protected:
                std::string m_Path;
            };
            
            class Meta_Resource
                : public Metabuf::Metaparse
            { 
            public:
                Meta_Resource();
                virtual ~Meta_Resource();
            
            protected:
                enum NoRequiredAttribute
                {
                    EMETA_Precompile = (1 <<1),
                    EMETA_Unique = (1 <<0),
                };
                
                uint32_t m_flagNoRequiredAttribute;
                
            public:
                template<class C, class M>
                void getm_Name( C _self, M _method ) const
                {
                    (_self->*_method)( this->m_Name );
                }
                
                const std::string & get_Name() const
                {
                    return this->m_Name;
                }
                
                bool has_Precompile() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_Precompile) != 0;
                }
                
                bool getd_Precompile( bool _default ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Precompile) == 0 )
                    {
                        return _default;
                    }
                
                    return this->m_Precompile;
                }
                
                bool get_Precompile( bool * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Precompile) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_Precompile;
                
                    return true;
                }
                
                template<class C, class M>
                bool getm_Precompile( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Precompile) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_Precompile );
                
                    return true;
                }
                
                bool getd_Precompile( bool * _value, const bool & _default ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Precompile) == 0 )
                    {
                        *_value = _default;
                
                        return false;
                    }
                
                    *_value = this->m_Precompile;
                
                    return true;
                }
                template<class C, class M>
                void getm_Type( C _self, M _method ) const
                {
                    (_self->*_method)( this->m_Type );
                }
                
                const std::string & get_Type() const
                {
                    return this->m_Type;
                }
                
                bool has_Unique() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_Unique) != 0;
                }
                
                bool getd_Unique( bool _default ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Unique) == 0 )
                    {
                        return _default;
                    }
                
                    return this->m_Unique;
                }
                
                bool get_Unique( bool * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Unique) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_Unique;
                
                    return true;
                }
                
                template<class C, class M>
                bool getm_Unique( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Unique) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_Unique );
                
                    return true;
                }
                
                bool getd_Unique( bool * _value, const bool & _default ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_Unique) == 0 )
                    {
                        *_value = _default;
                
                        return false;
                    }
                
                    *_value = this->m_Unique;
                
                    return true;
                }
            public:
                bool parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData ) override;
            
            protected:
                void _parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );
                void _parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );
                void _preparationIncludes( uint32_t _id, uint32_t _count );
                void _parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );
            
            public:
            protected:
                std::string m_Name;
                bool m_Precompile;
                std::string m_Type;
                bool m_Unique;
            };
            
            class Meta_ResourceImageDefault
                : public Meta_Resource
            { 
            public:
                Meta_ResourceImageDefault();
            
            protected:
                enum NoRequiredAttribute
                {
                    EMETA_File_Alpha = (1 <<4),
                    EMETA_File_Codec = (1 <<2),
                    EMETA_File_Converter = (1 <<3),
                    EMETA_File_Offset = (1 <<7),
                    EMETA_File_Premultiply = (1 <<5),
                    EMETA_File_Size = (1 <<6),
                };
                
            public:
                bool has_File_Alpha() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_File_Alpha) != 0;
                }
                
                template<class C, class M>
                bool getm_File_Alpha( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Alpha) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_File_Alpha );
                
                    return true;
                }
                
                bool get_File_Alpha( bool * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Alpha) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_File_Alpha;
                
                    return true;
                }
                
                bool has_File_Codec() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_File_Codec) != 0;
                }
                
                template<class C, class M>
                bool getm_File_Codec( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Codec) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_File_Codec );
                
                    return true;
                }
                
                bool get_File_Codec( std::string * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Codec) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_File_Codec;
                
                    return true;
                }
                
                bool has_File_Converter() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_File_Converter) != 0;
                }
                
                template<class C, class M>
                bool getm_File_Converter( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Converter) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_File_Converter );
                
                    return true;
                }
                
                bool get_File_Converter( std::string * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Converter) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_File_Converter;
                
                    return true;
                }
                
                template<class C, class M>
                void getm_File_MaxSize( C _self, M _method ) const
                {
                    (_self->*_method)( this->m_File_MaxSize );
                }
                
                const Detail::Rect & get_File_MaxSize() const
                {
                    return this->m_File_MaxSize;
                }
                
                bool has_File_Offset() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_File_Offset) != 0;
                }
                
                template<class C, class M>
                bool getm_File_Offset( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Offset) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_File_Offset );
                
                    return true;
                }
                
                bool get_File_Offset( Detail::Rect * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Offset) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_File_Offset;
                
                    return true;
                }
                
                template<class C, class M>
                void getm_File_Path( C _self, M _method ) const
                {
                    (_self->*_method)( this->m_File_Path );
                }
                
                const std::string & get_File_Path() const
                {
                    return this->m_File_Path;
                }
                
                bool has_File_Premultiply() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_File_Premultiply) != 0;
                }
                
                template<class C, class M>
                bool getm_File_Premultiply( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Premultiply) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_File_Premultiply );
                
                    return true;
                }
                
                bool get_File_Premultiply( bool * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Premultiply) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_File_Premultiply;
                
                    return true;
                }
                
                bool has_File_Size() const
                {
                    return (m_flagNoRequiredAttribute & EMETA_File_Size) != 0;
                }
                
                template<class C, class M>
                bool getm_File_Size( C _self, M _method ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Size) == 0 )
                    {
                        return false;
                    }
                
                    (_self->*_method)( this->m_File_Size );
                
                    return true;
                }
                
                bool get_File_Size( Detail::Rect * _value ) const
                {
                    if( (m_flagNoRequiredAttribute & EMETA_File_Size) == 0 )
                    {
                        return false;
                    }
                
                    *_value = this->m_File_Size;
                
                    return true;
                }
                
            public:
                bool parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData ) override;
            
            protected:
                void _parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );
                void _parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );
                void _preparationIncludes( uint32_t _id, uint32_t _count );
                void _parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );
            
            public:
            protected:
                bool m_File_Alpha;
                std::string m_File_Codec;
                std::string m_File_Converter;
                Detail::Rect m_File_MaxSize;
                Detail::Rect m_File_Offset;
                std::string m_File_Path;
                bool m_File_Premultiply;
                Detail::Rect m_File_Size;
            };
            
        protected:
            std::string m_Name;
        public:
            typedef Metabuf::Vector<Meta_Include> VectorMeta_Include;
        
            const VectorMeta_Include & get_Includes_Include() const
            {
                return this->includes_Meta_Include;
            }
        
        protected:
            VectorMeta_Include includes_Meta_Include;
        public:
            typedef Metabuf::Vector<Meta_Resource *> VectorMeta_Resource;
        
            const VectorMeta_Resource & get_Includes_Resource() const
            {
                return this->includes_Meta_Resource;
            }
        
        protected:
            VectorMeta_Resource includes_Meta_Resource;
        };
        
    }
}
