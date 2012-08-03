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

    protected:
        bool generateHeader( std::string & _header );        

	protected:		
		bool writeHeaderNode_( const XmlNode * _node );
				
    protected:
        bool writeHeaderConstructor_( const XmlNode * _node );
		bool writeHeaderAttributeReader_( const XmlNode * _node );
		bool writeHeaderAttributeSetup_( const XmlNode * _node );
		bool writeHeaderAttribute_( const XmlNode * _node );
		bool writeHeaderIncludes_( const XmlNode * _node );
		bool writeHeaderIncludesReader_( const XmlNode * _node );

    protected:
        bool generateSource( std::string & _header );

    protected:
        bool writeSourceNode_( const XmlNode * _node );

    protected:
        bool writeSourceAttributeReader_( const XmlNode * _node );
        bool writeSourceIncludesReader_( const XmlNode * _node );

	protected:
		std::stringstream & write();

	protected:
		std::stringstream m_out;
		size_t m_indent;

		XmlProtocol * m_protocol;

		std::string m_error;
	};
}