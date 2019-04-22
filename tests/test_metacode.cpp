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
        return 7;
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
            : Metabuf::Metadata()
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
            this->_parseData( _buff, _size, _read, _userData );
        
            uint32_t includeCount;
            this->readSize( _buff, _size, _read, includeCount );
        
            if( includeCount != 0 )
            {
                includes_Meta_Include.reserve( includeCount );
        
                for( uint32_t j = 0; j != includeCount; ++j )
                {
                    includes_Meta_Include.emplace_back( Meta_DataBlock::Meta_Include() );
                    Meta_DataBlock::Meta_Include & metadata = includes_Meta_Include.back();
        
                    metadata.parse( _buff, _size, _read, _userData );
                }
            }
        
            uint32_t generatorTypeCount;
            this->readSize( _buff, _size, _read, generatorTypeCount );
        
            for( uint32_t i = 0; i != generatorTypeCount; ++i )
            {
                uint32_t generatorCount;
                this->readSize( _buff, _size, _read, generatorCount );
        
                uint32_t id;
                this->readSize( _buff, _size, _read, id );
        
                this->_preparationIncludes( id, generatorCount );
        
                for( uint32_t j = 0; j != generatorCount; ++j )
                {
                    uint32_t generator_id;
                    this->readSize( _buff, _size, _read, generator_id );
        
                    this->_parseGenerators( _buff, _size, _read, generator_id, _userData );
                }
            }
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            this->read( _buff, _size, _read, _userData, this->m_Name );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::_preparationIncludes( uint32_t _id, uint32_t _count )
        {
            switch( _id )
            {
            case 2:
                {
                    includes_Meta_Include.reserve( _count );
                }break;
            case 3:
                {
                    includes_Meta_Resource.reserve( _count );
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
            case 4:
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
            : Metabuf::Metadata()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        bool Meta_DataBlock::Meta_Include::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            this->_parseData( _buff, _size, _read, _userData );
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Include::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            this->read( _buff, _size, _read, _userData, this->m_Path );
        }
        //////////////////////////////////////////////////////////////////////////
        //cppcheck-suppress uninitMemberVar
        Meta_DataBlock::Meta_Resource::Meta_Resource()
            : Metabuf::Metadata()
            , m_Precompile_successful(false)
            , m_Unique_successful(false)
        {
        }
        //////////////////////////////////////////////////////////////////////////
        Meta_DataBlock::Meta_Resource::~Meta_Resource()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        bool Meta_DataBlock::Meta_Resource::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            this->_parseData( _buff, _size, _read, _userData );
        
            uint32_t attributeCount;
            this->readSize( _buff, _size, _read, attributeCount );
        
            for( uint32_t i = 0; i != attributeCount; ++i )
            {
                uint32_t id;
                this->readSize( _buff, _size, _read, id );
        
                this->_parseArguments( _buff, _size, _read, id, _userData );
            }
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Resource::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            this->read( _buff, _size, _read, _userData, this->m_Name );
            this->read( _buff, _size, _read, _userData, this->m_Type );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_Resource::_parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )
        {
            switch( _id )
            {
            case 4:
                {
                    this->read( _buff, _size, _read, _userData, this->m_Precompile );
        
                    this->m_Precompile_successful = true;
                }break;
            case 3:
                {
                    this->read( _buff, _size, _read, _userData, this->m_Unique );
        
                    this->m_Unique_successful = true;
                }break;
            default:
                break;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        //cppcheck-suppress uninitMemberVar
        Meta_DataBlock::Meta_ResourceImageDefault::Meta_ResourceImageDefault()
            : Meta_Resource()
            , m_File_Alpha_successful(false)
            , m_File_Codec_successful(false)
            , m_File_Converter_successful(false)
            , m_File_Offset_successful(false)
            , m_File_Premultiply_successful(false)
            , m_File_Size_successful(false)
        {
        }
        //////////////////////////////////////////////////////////////////////////
        bool Meta_DataBlock::Meta_ResourceImageDefault::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            this->_parseData( _buff, _size, _read, _userData );
        
            uint32_t attributeCount;
            this->readSize( _buff, _size, _read, attributeCount );
        
            for( uint32_t i = 0; i != attributeCount; ++i )
            {
                uint32_t id;
                this->readSize( _buff, _size, _read, id );
        
                this->_parseArguments( _buff, _size, _read, id, _userData );
            }
        
            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_ResourceImageDefault::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )
        {
            Meta_DataBlock::Meta_Resource::_parseData( _buff, _size, _read, _userData );
        
            this->read( _buff, _size, _read, _userData, this->m_File_MaxSize );
            this->read( _buff, _size, _read, _userData, this->m_File_Path );
        }
        //////////////////////////////////////////////////////////////////////////
        void Meta_DataBlock::Meta_ResourceImageDefault::_parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )
        {
            Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id, _userData );
        
            switch( _id )
            {
            case 8:
                {
                    this->read( _buff, _size, _read, _userData, this->m_File_Alpha );
        
                    this->m_File_Alpha_successful = true;
                }break;
            case 6:
                {
                    this->read( _buff, _size, _read, _userData, this->m_File_Codec );
        
                    this->m_File_Codec_successful = true;
                }break;
            case 7:
                {
                    this->read( _buff, _size, _read, _userData, this->m_File_Converter );
        
                    this->m_File_Converter_successful = true;
                }break;
            case 12:
                {
                    this->read( _buff, _size, _read, _userData, this->m_File_Offset );
        
                    this->m_File_Offset_successful = true;
                }break;
            case 9:
                {
                    this->read( _buff, _size, _read, _userData, this->m_File_Premultiply );
        
                    this->m_File_Premultiply_successful = true;
                }break;
            case 11:
                {
                    this->read( _buff, _size, _read, _userData, this->m_File_Size );
        
                    this->m_File_Size_successful = true;
                }break;
            default:
                break;
            }
        }
    } 
}
