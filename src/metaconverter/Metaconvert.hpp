#pragma once

#include "metabuf/Metaconvert.hpp"

namespace Metabuf
{
    class Metaconvert
        : public MetaconvertInterface
    {
    public:
        Metaconvert();
        ~Metaconvert() override;

    public:
        void * operator new (size_t _size);
        void operator delete (void * _ptr, size_t _size);

    public:
        void generate( const ProtocolGenerator * _generator );

    public:
        uint32_t getProtocolVersion() const override;
        uint32_t getProtocolCrc32() const override;
        const MetaInterface * getMeta( std::string_view _name ) const override;
        bool validateProtocol( const void * _buffer, size_t _size, std::string & _error ) const override;

    public:
        bool makeHeader( const MetaInterface * _meta, OutputAdapter & _output, std::string & _error ) const override;

    protected:
        ProtocolInterface * m_protocol = nullptr;
    };
    //////////////////////////////////////////////////////////////////////////
}
