#include "test_metacode.h"

namespace Metacode
{
    //////////////////////////////////////////////////////////////////////////
    uint32_t get_metacode_magic()
    {
        return 3133062829u;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t get_metacode_version()
    {
        return 9;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t get_metacode_protocol_version()
    {
        return 1;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t get_metacode_protocol_crc32()
    {
        return 4064837494; 
    }
    //////////////////////////////////////////////////////////////////////////
    const char * getHeaderErrorMessage( Metabuf::HeaderError _error )
    {
        switch( _error )
        {
        case Metabuf::HEADER_SUCCESSFUL: return "Successful";
        case Metabuf::HEADER_INVALID_MAGIC: return "invalid magic header";
        case Metabuf::HEADER_INVALID_VERSION: return "invalid version";
        case Metabuf::HEADER_INVALID_PROTOCOL_VERSION: return "invalid protocol version";
        case Metabuf::HEADER_INVALID_PROTOCOL_CRC32: return "invalid protocol crc32";
        case Metabuf::HEADER_INVALID_METAVERSION: return "invalid meta version";
        default: return "invalid error";
        }
    }
    //////////////////////////////////////////////////////////////////////////
    Metabuf::HeaderError readHeader( const void * _buff, size_t _size, size_t & _read, uint32_t & _readVersion, uint32_t & _needVersion, uint32_t & _readProtocol, uint32_t & _needProtocol, uint32_t _metaVersion, uint32_t & _readMetaVersion )
    {
        uint32_t metacode_magic = get_metacode_magic();
        uint32_t metacode_version = get_metacode_version();
        uint32_t metacode_protocol_version = get_metacode_protocol_version();
        uint32_t metacode_protocol_crc32 = get_metacode_protocol_crc32();

        Metabuf::Reader ar(_buff, _size, _read);

        uint32_t head;
        ar.readPOD( head );

        if( head != metacode_magic )
        {
            return Metabuf::HEADER_INVALID_MAGIC;
        }

        uint32_t read_version;
        ar.readPOD( read_version );

        uint32_t read_protocol_version;
        ar.readPOD( read_protocol_version );

        uint32_t read_protocol_crc32;
        ar.readPOD( read_protocol_crc32 );

        uint32_t read_meta_version;
        ar.readPOD( read_meta_version );

        _readVersion = read_version;
        _needVersion = metacode_version;
        _readProtocol = read_protocol_version;
        _needProtocol = metacode_protocol_version;
        _readMetaVersion = read_meta_version;

        if( read_version != metacode_version )
        {
            return Metabuf::HEADER_INVALID_VERSION;
        }

        if( read_protocol_version != metacode_protocol_version )
        {
            return Metabuf::HEADER_INVALID_PROTOCOL_VERSION;
        }

        if( read_protocol_crc32 != metacode_protocol_crc32 )
        {
            return Metabuf::HEADER_INVALID_PROTOCOL_CRC32;
        }

        if( read_meta_version != _metaVersion )
        {
            return Metabuf::HEADER_INVALID_METAVERSION;
        }

        return Metabuf::HEADER_SUCCESSFUL;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t getInternalStringsCount()
    {
        return 1;
    }
    //////////////////////////////////////////////////////////////////////////
    const char * getInternalString( uint32_t _index, uint32_t & _stringSize, int64_t & _stringHash )
    {
        struct internal_t
        {
            uint32_t size;
            const char * str;
            uint64_t hash;
        };

        const internal_t internals[] = {
            {20, "ResourceImageDefault", 3470757930260756242UL},
        };

        const internal_t & internal = internals[_index];
        _stringSize = internal.size;
        _stringHash = internal.hash;

        return internal.str;
    }
    //////////////////////////////////////////////////////////////////////////
    bool readStrings( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringCount )
    {
        Metabuf::Reader ar(_buff, _size, _read);

        uint32_t count;
        ar.readPOD( count );

        _stringCount = count;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    const char * readString( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringSize, int64_t & _stringHash )
    {
        Metabuf::Reader ar(_buff, _size, _read);

        uint32_t size;
        ar.readSize( size );

        int64_t hash;
        ar.readPOD( hash );

        const char * value = ar.current_buff<char>();
        ar.skip( size );

        _stringSize = size;
        _stringHash = hash;

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    namespace Meta_Data
    { 
        uint32_t getVersion()
        {
            return 1;
        }
    
        //////////////////////////////////////////////////////////////////////////
        //cppcheck-suppress uninitMemberVar
        Meta_DataBlock::Meta_DataBlock()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        Meta_DataBlock::~Meta_DataBlock()
        {
            for( VectorMeta_Resource::const_iterator
                it = includes_Meta_Resource.begin(),
                it_end = includes_Meta_Resource.end();
            it != it_end;
            ++it )
            {
                delete *it;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        bool Meta_DataBlock::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _userData );
            Meta_DataBlock::_parseData(_buff, _size, _read, _userData); 
        
            uint32_t includeCount;
            Metabuf::readSize( _buff, _size, _read, includeCount );
        
            if( includeCount != 0 )
            {
                includes_Meta_Include.resize( includeCount );
        
                for( Meta_DataBlock::Meta_Include & metadata : includes_Meta_Include )
                {
                    metadata.parse( _buff, _size, _read, _userData );
                }
            }
        
            uint32_t generatorTypeCount;
            Metabuf::readSize( _buff, _size, _read, generatorTypeCount );
        
            for( uint32_t i = 0; i != generatorTypeCount; ++i )
            {
                uint32_t generatorCount;
                Metabuf::readSize( _buff, _size, _read, generatorCount );
        
                uint32_t id;
                Metabuf::readSize( _buff, _size, _read, id );
        
                this->_preparationIncludes( id, generatorCount );
        
                for( uint32_t j = 0; j != generatorCount; ++j )
                {
                    uint32_t generator_id;
                    Metabuf::readSize( _buff, _size, _read, generator_id );
        
                    this->_parseGenerators( _buff, _size, _read, generator_id, _userData );
                }
            }
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            Metabuf::read( _buff, _size, _read, _userData, this->m_Name );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::_preparationIncludes( uint32_t _id, uint32_t _count )
        {
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _count );
            switch( _id )
            {
            case 1:
                {
                    includes_Meta_Include.reserve( _count );
                }break;
            case 2:
                {
                    includes_Meta_Resource.reserve( _count );
                }break;
            default:
                break;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::_parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _userData );
            switch( _id )
            {
            case 1:
                {
                    includes_Meta_Include.emplace_back( Meta_DataBlock::Meta_Include() );
                    Meta_DataBlock::Meta_Include & metadata = includes_Meta_Include.back();
        
                    metadata.parse( _buff, _size, _read, _userData );
                }break;
            default:
                break;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::_parseGenerators( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )
        {
            switch( _id )
            {
            case 3:
                {
                    Meta_DataBlock::Meta_ResourceImageDefault * metadata = new Meta_DataBlock::Meta_ResourceImageDefault ();
                    metadata->parse( _buff, _size, _read, _userData );
        
                    includes_Meta_Resource.push_back(metadata);
                }break;
            default:
                break;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        //cppcheck-suppress uninitMemberVar
        Meta_DataBlock::Meta_Include::Meta_Include()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        bool Meta_DataBlock::Meta_Include::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _userData );
            Meta_DataBlock::Meta_Include::_parseData(_buff, _size, _read, _userData); 
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Include::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            Metabuf::read( _buff, _size, _read, _userData, this->m_Path );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Include::_preparationIncludes( uint32_t _id, uint32_t _count )
        {
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _count );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Include::_parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _userData );
        }
        //////////////////////////////////////////////////////////////////////////
        //cppcheck-suppress uninitMemberVar
        Meta_DataBlock::Meta_Resource::Meta_Resource()
            : m_flagNoRequiredAttribute(0)
        {
        }
        //////////////////////////////////////////////////////////////////////////
        Meta_DataBlock::Meta_Resource::~Meta_Resource()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        bool Meta_DataBlock::Meta_Resource::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _userData );
            Meta_DataBlock::Meta_Resource::_parseData(_buff, _size, _read, _userData); 
        
            Metabuf::readSize( _buff, _size, _read, m_flagNoRequiredAttribute );
        
            if( m_flagNoRequiredAttribute != 0 )
            {
                this->_parseArguments( _buff, _size, _read, _userData );
            }
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Resource::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            Metabuf::read( _buff, _size, _read, _userData, this->m_Name );
            Metabuf::read( _buff, _size, _read, _userData, this->m_Type );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Resource::_parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            if( (m_flagNoRequiredAttribute & EMETA_Precompile) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_Precompile );
            }
        
            if( (m_flagNoRequiredAttribute & EMETA_Unique) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_Unique );
            }
        
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Resource::_preparationIncludes( uint32_t _id, uint32_t _count )
        {
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _count );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Resource::_parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _userData );
        }
        //////////////////////////////////////////////////////////////////////////
        //cppcheck-suppress uninitMemberVar
        Meta_DataBlock::Meta_ResourceImageDefault::Meta_ResourceImageDefault()
            : Meta_Resource()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        bool Meta_DataBlock::Meta_ResourceImageDefault::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _userData );
            Meta_DataBlock::Meta_ResourceImageDefault::_parseData(_buff, _size, _read, _userData); 
        
            Metabuf::readSize( _buff, _size, _read, m_flagNoRequiredAttribute );
        
            if( m_flagNoRequiredAttribute != 0 )
            {
                this->_parseArguments( _buff, _size, _read, _userData );
            }
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_ResourceImageDefault::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            Meta_DataBlock::Meta_Resource::_parseData( _buff, _size, _read, _userData );
        
            Metabuf::read( _buff, _size, _read, _userData, this->m_File_MaxSize );
            Metabuf::read( _buff, _size, _read, _userData, this->m_File_Path );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_ResourceImageDefault::_parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _userData );
        
            if( (m_flagNoRequiredAttribute & EMETA_File_Alpha) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_File_Alpha );
            }
        
            if( (m_flagNoRequiredAttribute & EMETA_File_Codec) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_File_Codec );
            }
        
            if( (m_flagNoRequiredAttribute & EMETA_File_Converter) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_File_Converter );
            }
        
            if( (m_flagNoRequiredAttribute & EMETA_File_Offset) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_File_Offset );
            }
        
            if( (m_flagNoRequiredAttribute & EMETA_File_Premultiply) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_File_Premultiply );
            }
        
            if( (m_flagNoRequiredAttribute & EMETA_File_Size) != 0 )
            {
                Metabuf::read( _buff, _size, _read, _userData, this->m_File_Size );
            }
        
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_ResourceImageDefault::_preparationIncludes( uint32_t _id, uint32_t _count )
        {
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _count );
            Meta_DataBlock::Meta_Resource::_preparationIncludes( _id, _count );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_ResourceImageDefault::_parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )
        {
            METABUF_UNUSED( _buff );
            METABUF_UNUSED( _size );
            METABUF_UNUSED( _read );
            METABUF_UNUSED( _id );
            METABUF_UNUSED( _userData );
            Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _id, _userData );
        
        }
    } 
}
