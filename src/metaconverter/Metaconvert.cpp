#include "metaconverter/Metaconvert.hpp"

#include "metaconverter/Converter.hpp"
#include "metaconverter/Protocol.hpp"

#include "config/Metaconfig.hpp"

#include <sstream>

namespace Metabuf
{
    Metaconvert::Metaconvert()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Metaconvert::~Metaconvert()
    {
        delete m_protocol;
    }
    //////////////////////////////////////////////////////////////////////////
    void * Metaconvert::operator new (size_t _size)
    {
        return METABUF_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Metaconvert::operator delete (void * _ptr, size_t _size)
    {
        METABUF_UNUSED( _size );

        METABUF_FREE( _ptr, _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Metaconvert::generate( const ProtocolGenerator * _generator )
    {
        Protocol * protocol = new Protocol();
        _generator->generate( protocol );

        m_protocol = protocol;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t Metaconvert::getProtocolVersion() const
    {
        uint32_t version = m_protocol->getVersion();

        return version;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t Metaconvert::getProtocolCrc32() const
    {
        uint32_t crc32 = m_protocol->getCrc32();

        return crc32;
    }
    //////////////////////////////////////////////////////////////////////////
    const MetaInterface * Metaconvert::getMeta( std::string_view _name ) const
    {
        const MetaInterface * meta = m_protocol->getMeta( _name );

        return meta;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Metaconvert::validateProtocol( const void * _buffer, size_t _size, std::string & _error ) const
    {
        Protocol protocol;

        if( protocol.readProtocol( _buffer, _size ) == false )
        {
            _error = protocol.getError();

            return false;
        }

        const uint32_t version = protocol.getVersion();
        const uint32_t crc32 = protocol.getCrc32();
        const uint32_t expectedVersion = m_protocol->getVersion();
        const uint32_t expectedCrc32 = m_protocol->getCrc32();

        if( version != expectedVersion || crc32 != expectedCrc32 )
        {
            std::stringstream ss;
            ss << "protocol mismatch version [" << version << ':' << expectedVersion << "] crc32 [" << crc32 << ':' << expectedCrc32 << ']';

            _error = ss.str();

            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Metaconvert::makeHeader( const MetaInterface * _meta, OutputAdapter & _output, std::string & _error ) const
    {
        if( ::Metabuf::makeHeader( m_protocol, _meta, _output, _error ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    METABUF_EXPORT void destroyMetaconvert( MetaconvertInterface * _metaconvert )
    {
        if( _metaconvert == nullptr )
        {
            return;
        }

        delete _metaconvert;
    }
    //////////////////////////////////////////////////////////////////////////
}
