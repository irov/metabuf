#	pragma once

#	include "XmlProtocol.hpp"

#	include <pugixml.hpp>

#	include <string>
#	include <sstream>

namespace Metabuf
{
	class Xml2Metacode
	{
	public:
		Xml2Metacode( XmlProtocol * _protocol );

	public:
		bool generate( std::string & _header, std::string & _source );
        std::string getError();

    protected:
        bool generateHeader( std::string & _header );        

	protected:		
		bool writeHeaderNode_( const XmlNode * _node );
				
    protected:
        bool writeHeaderConstructor_( const XmlNode * _node );
		bool writeHeaderAttributeReader_( const XmlNode * _node );
		bool writeHeaderAttributeSetup_( const XmlNode * _node );
        bool writeHeaderIncludesDefinition_( const XmlNode * _node );        
		bool writeHeaderAttribute_( const XmlNode * _node );
		bool writeHeaderIncludes_( const XmlNode * _node );
        bool writeHeaderIncludesPreparation_( const XmlNode * _node );
		bool writeHeaderIncludesReader_( const XmlNode * _node );
        bool writeHeaderGeneratorsReader_( const XmlNode * _node );

    protected:
        bool generateSource( std::string & _header );

    protected:
        bool writeSourceNode_( const XmlNode * _node );

    protected:
        bool writeSourceAttributeReader_( const XmlNode * _node );
        bool writeSourceIncludesPreparation_( const XmlNode * _node );
        bool writeSourceIncludesReader_( const XmlNode * _node );
        bool writeSourceGeneratorsReader_( const XmlNode * _node );
        bool writeSourceIncludesDefinition_( const XmlNode * _node );

	protected:
		std::stringstream & write();

	protected:
		std::stringstream m_out;
		size_t m_indent;

		XmlProtocol * m_protocol;

		std::stringstream m_error;
	};
}