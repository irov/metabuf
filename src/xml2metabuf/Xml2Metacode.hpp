#pragma once

#include "config/Metaconfig.hpp"

#include "XmlProtocol.hpp"

#include "pugixml.hpp"

#include <string>
#include <sstream>

namespace Metabuf
{
    struct Xml2Settings
    {
        std::string metacode_h;
        std::string metatype_h;
    };

    class Xml2Metacode
    {
    public:
        Xml2Metacode( const XmlProtocol * _protocol );
        ~Xml2Metacode();

    public:
        bool generate( std::string & _header, std::string & _source, const Xml2Settings & _settings );
        std::string getError() const;

    protected:
        bool generateHeader( std::stringstream & _ss );

    protected:
        bool writeHeaderMeta_( std::stringstream & _ss, const XmlMeta * _meta );
        bool writeHeaderNode_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node, bool _root );

    protected:
        bool writeHeaderConstructor_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderParse_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderDataReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderAttributeReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderAttributeSetup_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderIncludesDefinition_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node );
        bool writeHeaderAttributeNoRequired_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderAttribute_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderSingles_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderSinglesReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderIncludes_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderIncludesReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderChildren_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node );
        bool writeHeaderChildrenPreparation_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderChildrenReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node );

    protected:
        bool generateSource( std::stringstream & _ss );

    protected:
        bool writeSourceMeta_( std::stringstream & _ss, const XmlMeta * _meta );
        bool writeSourceNode_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node, bool _root );

    protected:
        bool writeSourceConstructor_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node, bool _root );
        bool writeSourceParse_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceDataReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceAttributeReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceSinglesReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceIncludesReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceChildrenPreparation_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node );
        bool writeSourceChildrenReader_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node );
        bool writeSourceGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceIncludesDefinition_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node );

    protected:
        bool hasNodeDataSize_( const XmlNode * _node, bool _hierarchy );
        bool hasNodeAttributeSize_( const XmlNode * _node, bool _hierarchy );

    protected:
        std::stringstream & write( std::stringstream & _ss );

    protected:
        const XmlProtocol * m_protocol;

        Xml2Settings m_settings;

        std::stringstream m_out;
        uint32_t m_indent;

        std::stringstream m_error;
    };
}