#	pragma once

#	include <pugixml.hpp>

#	include <string>

namespace Metabuf
{
	class XmlProtocol;

	class Xml2Metabuf
	{
	public:
		Xml2Metabuf( char * _out, XmlProtocol * _protocol );

	public:
		bool convert( const void * _buff, size_t _size, size_t & _write );

	protected:
		bool writeNode_( const pugi::xml_node & root );

	protected:
		char * m_out;
		size_t m_write;

		XmlProtocol * m_protocol;

		std::string m_error;
	};
}