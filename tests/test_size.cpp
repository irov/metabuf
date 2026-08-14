#include "metabuf/Reader.hpp"

#include "xml2metabuf/Xml2Metabuf.hpp"

#include <stdlib.h>
#include <stdint.h>

namespace
{
    class SizeWriter
        : public Metabuf::Xml2Metabuf
    {
    public:
        SizeWriter()
            : Metabuf::Xml2Metabuf( nullptr, nullptr )
        {
        }

    public:
        bool test( uint32_t _value, const uint8_t * _expected, size_t _size )
        {
            m_buff.clear();

            this->writeSize( _value );

            if( m_buff.size() != _size )
            {
                return false;
            }

            for( size_t index = 0; index != _size; ++index )
            {
                if( m_buff[index] != _expected[index] )
                {
                    return false;
                }
            }

            return true;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    bool testReadSize( const uint8_t * _buffer, size_t _capacity, uint32_t _expected )
    {
        size_t read = 0;
        Metabuf::Reader reader( _buffer, _capacity, read );

        uint32_t value;
        reader.readSize( value );

        if( value != _expected )
        {
            return false;
        }

        if( read != _capacity )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////
int main()
{
    const uint8_t size_253[] = {253};
    const uint8_t size_254[] = {254, 254, 0};
    const uint8_t size_65535[] = {254, 255, 255};
    const uint8_t size_65536[] = {255, 0, 0, 1, 0};
    const uint8_t size_4294967295[] = {255, 255, 255, 255, 255};

    SizeWriter writer;

    if( writer.test( 253, size_253, sizeof( size_253 ) ) == false ||
        writer.test( 254, size_254, sizeof( size_254 ) ) == false ||
        writer.test( 65535, size_65535, sizeof( size_65535 ) ) == false ||
        writer.test( 65536, size_65536, sizeof( size_65536 ) ) == false ||
        writer.test( 4294967295U, size_4294967295, sizeof( size_4294967295 ) ) == false )
    {
        return EXIT_FAILURE;
    }

    if( testReadSize( size_253, sizeof( size_253 ), 253 ) == false ||
        testReadSize( size_254, sizeof( size_254 ), 254 ) == false ||
        testReadSize( size_65535, sizeof( size_65535 ), 65535 ) == false ||
        testReadSize( size_65536, sizeof( size_65536 ), 65536 ) == false ||
        testReadSize( size_4294967295, sizeof( size_4294967295 ), 4294967295U ) == false )
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
