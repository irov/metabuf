#include "../src/metabufconverter/Converter.hpp"
#include "../src/xml2metabuf/XmlConverter.hpp"
#include "../src/xml2metabuf/XmlProtocol.hpp"
#include "../src/json2metabuf/JsonConverter.hpp"

#include "test_metacode.h"
#include "Metaprotocol.h"
#include "test_utils.h"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace
{
    bool parseRawDataBlock( const std::vector<uint8_t> & _raw, Metacode::Meta_Data::Meta_DataBlock & _metadata )
    {
        size_t read = 0;
        uint32_t stringCount;

        if( Metacode::readStrings( _raw.data(), _raw.size(), read, stringCount ) == false )
        {
            return false;
        }

        Metabuf::Metacache metacache;
        const uint32_t internalStringsCount = Metacode::getInternalStringsCount();
        metacache.strings.resize( internalStringsCount + stringCount );

        for( uint32_t index = 0; index != internalStringsCount + stringCount; ++index )
        {
            uint32_t stringSize;
            const char * value;

            if( index < internalStringsCount )
            {
                value = Metacode::getInternalString( index, stringSize );
            }
            else
            {
                value = Metacode::readString( _raw.data(), _raw.size(), read, stringSize );
            }

            metacache.strings[index].assign( value, stringSize );
        }

        if( _metadata.parse( _raw.data(), _raw.size(), read, &metacache ) == false )
        {
            return false;
        }

        if( read != _raw.size() )
        {
            return false;
        }

        return true;
    }

    bool validateDataBlock( const Metacode::Meta_Data::Meta_DataBlock & _metadata )
    {
        if( _metadata.get_Name() != "Test" )
        {
            return false;
        }

        const Metacode::Meta_Data::Meta_DataBlock::VectorMeta_Resource & resources = _metadata.get_Includes_Resource();

        if( resources.size() != 1 )
        {
            return false;
        }

        const Metacode::Meta_Data::Meta_DataBlock::Meta_Resource * resource = resources.front();

        if( resource->get_Name() != "Battleground" || resource->get_Type() != "ResourceImageDefault" )
        {
            return false;
        }

        const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceImageDefault * image = static_cast<const Metacode::Meta_Data::Meta_DataBlock::Meta_ResourceImageDefault *>(resource);

        if( image->get_File_Path() != "Art/Battleground.png" )
        {
            return false;
        }

        if( (image->get_File_MaxSize() == Detail::Rect( {867, 1536} )) == false )
        {
            return false;
        }

        return true;
    }
}

int main( int argc, char * argv[] )
{
    METABUF_UNUSED( argc );

    Metacode::Meta_Data::Meta_DataBlock metadata;

    if( std::strcmp( metadata.getMetaName(), "Data" ) != 0 ||
        std::strcmp( metadata.getNodeName(), "DataBlock" ) != 0 ||
        metadata.getMetaVersion() != Metacode::Meta_Data::getVersion() )
    {
        return EXIT_FAILURE;
    }

    size_t protocol_size;
    void * protocol_buffer = read_file( argv[1], "Protocol.xml", &protocol_size );

    Metabuf::XmlProtocol protocol;

    if( protocol_buffer == nullptr || protocol.readProtocol( protocol_buffer, protocol_size ) == false )
    {
        return EXIT_FAILURE;
    }

    free( protocol_buffer );

    size_t xml_size;
    void * xml_buffer = read_file( argv[1], "Example.xml", &xml_size );
    size_t json_size;
    void * json_buffer = read_file( argv[1], "Example.json", &json_size );

    std::vector<uint8_t> xml_raw;
    std::vector<uint8_t> json_raw;
    std::string error;

    if( Metabuf::convertXml( &protocol, xml_buffer, xml_size, "Data", "DataBlock", xml_raw, error ) == false )
    {
        return EXIT_FAILURE;
    }

    if( Metabuf::convertJson( &protocol, json_buffer, json_size, "Data", "DataBlock", json_raw, error ) == false )
    {
        return EXIT_FAILURE;
    }

    if( xml_raw != json_raw )
    {
        return EXIT_FAILURE;
    }

    std::vector<uint8_t> generated_xml_raw;
    std::vector<uint8_t> generated_json_raw;
    std::string generated_error;
    Metabuf::XmlProtocol generated_protocol;
    Metacode::initializeMetaprotocol( &generated_protocol );

    std::vector<uint8_t> generated_header;

    if( Metabuf::makeHeader( &generated_protocol, "Data", generated_header, generated_error ) == false )
    {
        return EXIT_FAILURE;
    }

    if( generated_header.size() != sizeof( uint32_t ) * 5 )
    {
        return EXIT_FAILURE;
    }

    if( Metabuf::convertXml( &generated_protocol, xml_buffer, xml_size, "Data", "DataBlock", generated_xml_raw, generated_error ) == false )
    {
        return EXIT_FAILURE;
    }

    if( Metabuf::convertJson( &generated_protocol, json_buffer, json_size, "Data", "DataBlock", generated_json_raw, generated_error ) == false )
    {
        return EXIT_FAILURE;
    }

    if( generated_xml_raw != xml_raw || generated_json_raw != json_raw )
    {
        return EXIT_FAILURE;
    }

    free( xml_buffer );
    free( json_buffer );

    const char invalid_complex_object[] = "{\"Name\":\"Test\",\"Resource\":{\"Name\":\"Battleground\",\"Type\":\"ResourceImageDefault\",\"File\":{\"Path\":\"Art/Battleground.png\",\"MaxSize\":{\"x\":867,\"y\":1536}}}}";
    std::vector<uint8_t> invalid_raw;

    if( Metabuf::convertJson( &protocol, invalid_complex_object, sizeof( invalid_complex_object ) - 1, "Data", "DataBlock", invalid_raw, error ) == true )
    {
        return EXIT_FAILURE;
    }

    if( error.find( "invalid complex value" ) == std::string::npos )
    {
        return EXIT_FAILURE;
    }

    Metacode::Meta_Data::Meta_DataBlock xml_metadata;
    Metacode::Meta_Data::Meta_DataBlock json_metadata;

    if( parseRawDataBlock( xml_raw, xml_metadata ) == false )
    {
        return EXIT_FAILURE;
    }

    if( parseRawDataBlock( json_raw, json_metadata ) == false )
    {
        return EXIT_FAILURE;
    }

    if( validateDataBlock( xml_metadata ) == false )
    {
        return EXIT_FAILURE;
    }

    if( validateDataBlock( json_metadata ) == false )
    {
        return EXIT_FAILURE;
    }

    const char invalid_required[] = "{\"Include\":{}}";

    if( Metabuf::convertJson( &protocol, invalid_required, sizeof( invalid_required ) - 1, "Data", "DataBlock", invalid_raw, error ) == true )
    {
        return EXIT_FAILURE;
    }

    if( error.find( "required" ) == std::string::npos )
    {
        return EXIT_FAILURE;
    }

    size_t validation_protocol_size;
    void * validation_protocol_buffer = read_file( argv[1], "ValidationProtocol.xml", &validation_protocol_size );
    Metabuf::XmlProtocol validation_protocol;

    if( validation_protocol_buffer == nullptr || validation_protocol.readProtocol( validation_protocol_buffer, validation_protocol_size ) == false )
    {
        return EXIT_FAILURE;
    }

    free( validation_protocol_buffer );

    const char default_xml[] = "<Root Name=\"Test\"/>";
    const char default_json[] = "{\"Name\":\"Test\"}";
    std::vector<uint8_t> default_xml_raw;
    std::vector<uint8_t> default_json_raw;

    if( Metabuf::convertXml( &validation_protocol, default_xml, sizeof( default_xml ) - 1, "Data", "Root", default_xml_raw, error ) == false )
    {
        return EXIT_FAILURE;
    }

    if( Metabuf::convertJson( &validation_protocol, default_json, sizeof( default_json ) - 1, "Data", "Root", default_json_raw, error ) == false )
    {
        return EXIT_FAILURE;
    }

    if( default_xml_raw != default_json_raw )
    {
        return EXIT_FAILURE;
    }

    const char invalid_enum[] = "{\"Name\":\"Test\",\"Mode\":\"Unknown\"}";

    if( Metabuf::convertJson( &validation_protocol, invalid_enum, sizeof( invalid_enum ) - 1, "Data", "Root", invalid_raw, error ) == true )
    {
        return EXIT_FAILURE;
    }

    if( error.find( "enumerate" ) == std::string::npos )
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
