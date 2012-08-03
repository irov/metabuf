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
    namespace Serialize
    {
        static bool s_write_string( Xml2Metabuf * _metabuf, const char * _value )
        {
            size_t size = strlen( _value );
            _metabuf->write( size );
            _metabuf->writeCount( _value, size );

            return true;
        }

        static bool s_write_wstring( Xml2Metabuf * _metabuf, const char * _value )
        {
            size_t size = strlen( _value );

            _metabuf->write( size );
            _metabuf->writeCount( _value, size );

            return true;
        }

        static bool s_write_float2( Xml2Metabuf * _metabuf, const char * _value )
        {
            float value[2];
            if( sscanf_s( _value, "%f;%f", &value[0], &value[1] ) != 2 )
            {
                if( sscanf_s( _value, "%f %f", &value[0], &value[1] ) != 2 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 2 );

            return true;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::initialize()
    {
        m_serialization["string"] = &Serialize::s_write_string;
        m_serialization["wstring"] = &Serialize::s_write_wstring;
        m_serialization["float2"] = &Serialize::s_write_float2;        
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
	bool Xml2Metabuf::writeNode_( const pugi::xml_node & _xml_node )
	{
		const char * nodeName = _xml_node.name();

		const XmlNode * node = m_protocol->getNode( nodeName );

		if( node == 0 )
		{
			std::stringstream ss;
			ss << "XmlToBin write not found node '" << nodeName << "'";
			m_error = ss.str();

			return false;
		}

		size_t id = node->id;
		this->write( id );

        this->writeNodeAttribute_( node, _xml_node );
        this->writeNodeIncludes_( node, _xml_node );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        size_t attributeCount;
        this->getNodeAttributeSize_( _node, _xml_node, attributeCount );

        this->write( attributeCount );

		for( pugi::xml_node::attribute_iterator
			it = _xml_node.attributes_begin(),
			it_end = _xml_node.attributes_end();
		it != it_end;
		++it )
		{
			const pugi::xml_attribute & xml_attr = *it;

			const char * attrName = xml_attr.name();

			const XmlAttribute * attr = _node->getAttribute( attrName );

			size_t id = attr->id;
            this->write( id );

            TMapSerialization::const_iterator it_serialize = m_serialization.find( attr->evict );

            if( it_serialize == m_serialization.end() )
            {
                return false;
            }

            const char * attr_value = xml_attr.value();
            (*it_serialize->second)( this, attr_value );
		}

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::getNodeAttributeSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, size_t & _count )
    {    
        size_t count = 0;

        for( pugi::xml_node::attribute_iterator
            it = _xml_node.attributes_begin(),
            it_end = _xml_node.attributes_end();
        it != it_end;
        ++it )
        {
            ++count;
        }

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
        it != it_end;
        ++it )
        {
            const pugi::xml_node & child = *it;
            
            const char * child_name = child.name();

            const XmlMember * member = _node->getMember( child_name );
            
            if( member == 0 )
            {
                continue;
            }
             
            for( pugi::xml_node::attribute_iterator
                it = child.attributes_begin(),
                it_end = child.attributes_end();
            it != it_end;
            ++it )
            {
                ++count;
            }
        }

        _count = count;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeIncludes_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        size_t attributeCount;
        this->getNodeIncludesSize_( _node, _xml_node, attributeCount );

        this->write( attributeCount );

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
        it != it_end;
        ++it )
        {
            const pugi::xml_node & child = *it;

            const char * child_name = child.name();

            if( _node->isInclude( child_name ) == false )
            {
                continue;
            }

            this->writeNode_( child );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::getNodeIncludesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, size_t & _count )
    {
        size_t count = 0;

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
        it != it_end;
        ++it )
        {
            const pugi::xml_node & child = *it;

            const char * child_name = child.name();

            if( _node->isInclude( child_name ) == false )
            {
                continue;
            }

            ++count;
        }

        _count = count;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::writeBuffer( const char * _buff, size_t _size )
    {
        memcpy( m_out + m_write, _buff, _size );
        m_write += _size;
    }
}