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
        bool generateHeader( std::stringstream & _ss );        

	protected:		
		bool writeHeaderNode_( std::stringstream & _ss, const XmlNode * _node );
				
    protected:
        bool writeHeaderConstructor_( std::stringstream & _ss, const XmlNode * _node );
		bool writeHeaderAttributeReader_( std::stringstream & _ss, const XmlNode * _node );
		bool writeHeaderAttributeSetup_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderIncludesDefinition_( std::stringstream & _ss, const XmlNode * _node );        
		bool writeHeaderAttribute_( std::stringstream & _ss, const XmlNode * _node );
		bool writeHeaderIncludes_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node );
		bool writeHeaderIncludesReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeHeaderGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node );

    protected:
        bool generateSource( std::stringstream & _ss );

    protected:
        bool writeSourceNode_( std::stringstream & _ss, const XmlNode * _node );

    protected:
		bool writeSourceConstructor_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceAttributeReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceIncludesReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node );
        bool writeSourceIncludesDefinition_( std::stringstream & _ss, const XmlNode * _node );

	protected:
		std::stringstream & write( std::stringstream & _ss );

	protected:
		std::stringstream m_out;
		unsigned int m_indent;

		XmlProtocol * m_protocol;

		std::stringstream m_error;
	};
}