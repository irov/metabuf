#	include "Xml2Metabuf.hpp"
#	include "XmlProtocol.hpp"

#	include <sstream>

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	Xml2Metabuf::Xml2Metabuf( char * _out, XmlProtocol * _protocol )
		: m_out(_out)
		, m_write(0)
		, m_protocol(_protocol)
	{

	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::convert( const void * _buff, size_t _size, size_t & _write )
	{	
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_buffer( _buff, _size );

		if( result == false )
		{
			printf("%s"
				, result.description()
				);

			return false;
		}

		pugi::xml_node root = doc.document_element();

		if( this->writeNode_( root ) == false )
		{
			return false;
		}

		_write = m_write;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNode_( const pugi::xml_node & _node )
	{
		const char * nodeName = _node.name();

		const XmlNode * node = m_protocol->getNode( nodeName );

		if( node == 0 )
		{
			std::stringstream ss;
			ss << "XmlToBin write not found node '" << nodeName << "'";
			m_error = ss.str();

			return false;
		}

		size_t id = node->id;
		//s_writeStream( _stream, id );

		for( pugi::xml_node::attribute_iterator
			it = _node.attributes_begin(),
			it_end = _node.attributes_end();
		it != it_end;
		++it )
		{
			const pugi::xml_attribute & xml_attr = *it;

			const char * attrName = xml_attr.name();

			const XmlAttribute * attr = node->getAttribute( attrName );

			
		}

		return true;
	}

}