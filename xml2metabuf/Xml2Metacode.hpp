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
		bool generate( std::string & _out );

	protected:
		bool writeConstructor_( const XmlNode * _node );
		bool writeNode_( const XmlNode * _node );
				
		bool writeNodeReader_( const XmlNode * _node );
		bool writeNodeSetup_( const XmlNode * _node );
		bool writeNodeMember_( const XmlNode * _node );

	protected:
		std::stringstream & write();

	protected:
		std::stringstream m_out;
		size_t m_indent;

		XmlProtocol * m_protocol;

		std::string m_error;
	};
}