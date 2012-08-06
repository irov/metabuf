#	include "Xml2Metabuf.hpp"
#	include "XmlProtocol.hpp"

#   include <Windows.h>
#	include <sstream>

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	Xml2Metabuf::Xml2Metabuf( char * _out, size_t _size, XmlProtocol * _protocol )
		: m_out(_out)
        , m_size(_size)
		, m_write(0)
		, m_protocol(_protocol)
	{

	}
    //////////////////////////////////////////////////////////////////////////
    namespace Serialize
    {
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_string( Xml2Metabuf * _metabuf, const char * _value )
        {
            size_t size = strlen( _value );
            if( _metabuf->writeSize( size ) == false )
            {
                return false;
            }

            _metabuf->writeCount( _value, size );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_wstring( Xml2Metabuf * _metabuf, const char * _value )
        {
            int size = ::MultiByteToWideChar( CP_UTF8, 0, _value, -1, 0, 0 );

            if( size == 0 )
            {
                return false;
            }

            if( _metabuf->writeSize( size - 1 ) == false )
            {
                return false;
            }

            wchar_t * buffer = new wchar_t[size + 1];
            int wc = ::MultiByteToWideChar( CP_UTF8, 0, _value, -1, buffer, size );
            
            _metabuf->writeCount( buffer, size - 1 );

            delete [] buffer;

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_bool( Xml2Metabuf * _metabuf, const char * _value )
        {
            int int_value;
            if( sscanf_s( _value, "%d", &int_value ) != 1 )
            {
                return false;
            }

            bool value = (int_value != 0);
            _metabuf->write( value );

            return true;
        }        
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_size_t( Xml2Metabuf * _metabuf, const char * _value )
        {
            size_t value;
            if( sscanf_s( _value, "%u", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float( Xml2Metabuf * _metabuf, const char * _value )
        {
            float value;
            if( sscanf_s( _value, "%f", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
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
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float3( Xml2Metabuf * _metabuf, const char * _value )
        {
            float value[3];
            if( sscanf_s( _value, "%f;%f;%f", &value[0], &value[1], &value[2] ) != 3 )
            {
                if( sscanf_s( _value, "%f %f %f", &value[0], &value[1], &value[2] ) != 3 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 3 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float4( Xml2Metabuf * _metabuf, const char * _value )
        {
            float value[4];
            if( sscanf_s( _value, "%f;%f;%f;%f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
            {
                if( sscanf_s( _value, "%f %f %f %f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 4 );

            return true;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::initialize()
    {
        m_serialization["string"] = &Serialize::s_write_string;
        m_serialization["wstring"] = &Serialize::s_write_wstring;
        m_serialization["bool"] = &Serialize::s_write_bool;
        m_serialization["size_t"] = &Serialize::s_write_size_t;
        m_serialization["float"] = &Serialize::s_write_float;
        m_serialization["float2"] = &Serialize::s_write_float2;
        m_serialization["float3"] = &Serialize::s_write_float3;
        m_serialization["float4"] = &Serialize::s_write_float4;
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

        const char * root_name = root.name();

        const XmlNode * node_root = m_protocol->getNode( root_name );

		if( this->writeNode_( node_root, root ) == false )
		{
			return false;
		}

		_write = m_write;

		return true;
	}
	std::string Xml2Metabuf::getError()
	{
		return m_error.str();
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNode_( const XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		size_t id = _node->id;
		if( this->writeSize( id ) == false )
        {
            return false;
        }

        if( this->writeNodeAttribute_( _node, _xml_node ) == false )
        {
            return false;
        }

        if( this->writeNodeIncludes_( _node, _xml_node ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        size_t attributeCount;
        this->getNodeAttributeSize_( _node, _xml_node, attributeCount );

        if( this->writeSize( attributeCount ) == false )
        {
            return false;
        }

        if( _node->inheritance.empty() == false )
        {
            this->writeNodeAttribute2_( _node->node_inheritance, _xml_node );
        }

        this->writeNodeAttribute2_( _node, _xml_node );

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

            for( TMapAttributes::const_iterator
                it = member->attributes.begin(),
                it_end = member->attributes.end();
            it != it_end;
            ++it )
            {
                const XmlAttribute * attr = &it->second;

                pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

                if( xml_attr == false )
                {
                    if( attr->required == true )
                    {
                        m_error << "Xml2Metabuf::writeNodeAttribute_:" << _node->name << " member " << member->name << " not found required argument " << attr->name << std::endl;

                        return false;
                    }
                    else
                    {
                        continue;
                    }
                }

				if( this->writeNodeArguments_( attr, xml_attr ) == false )
				{
					m_error << "Xml2Metabuf::writeNodeAttribute_:" << _node->name << " not write member " << member->name << " argument " << attr->name << std::endl;

					return false;
				}
            }
        }

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeAttribute2_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        for( TMapAttributes::const_iterator
            it = _node->attributes.begin(),
            it_end = _node->attributes.end();
        it != it_end;
        ++it )
        {
            const XmlAttribute * attr = &it->second;

            pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

            if( xml_attr == false )
            {
                if( attr->required == true )
                {
                    m_error << "Xml2Metabuf::writeNodeAttribute_:" << _node->name << " not found required argument " << attr->name << std::endl;

                    return false;
                }
                else
                {
                    continue;
                }
            }

            if( this->writeNodeArguments_( attr, xml_attr ) == false )
            {
                m_error << "Xml2Metabuf::writeNodeAttribute_:" << _node->name << " not write argument " << attr->name << std::endl;

                return false;
            }
        }

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeArguments_( const XmlAttribute * _attr, const pugi::xml_attribute & _xml_attr )
	{
		size_t id = _attr->id;
		if( this->writeSize( id ) == false )
		{
			return false;
		}

		TMapSerialization::const_iterator it_serialize = m_serialization.find( _attr->evict );

		if( it_serialize == m_serialization.end() )
		{
			m_error << "Xml2Metabuf::writeNodeArguments_: not found serialize " << _attr->evict << " for attribute " << _attr->name << std::endl;

			return false;
		}

		const char * attr_value = _xml_attr.value();
		if( (*it_serialize->second)( this, attr_value ) == false )
		{
			m_error << "Xml2Metabuf::writeNodeAttribute_: serialize " << _attr->evict << " for attribute " << _attr->name << " error for value " << attr_value << std::endl;

			return false;
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
        size_t incluidesCount;
        this->getNodeIncludesSize_( _node, _xml_node, incluidesCount );

        this->write( incluidesCount );

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
        it != it_end;
        ++it )
        {
            const pugi::xml_node & child = *it;

            const char * child_name = child.name();

            const XmlNode * node_includes = _node->getInclude( child_name );

            if( node_includes == 0 )
            {
                continue;
            }

            if( this->writeSize( node_includes->id ) == false )
			{
				return false;
			}

            if( node_includes->generator.empty() == true )
            {
                if( this->writeNode_( node_includes, child ) == false )
				{
                    m_error << "Xml2Metabuf::writeNodeIncludes_: error write node " << _node->name << " includes " << node_includes->name << std::endl;

					return false;
				}
            }
            else
            {
                pugi::xml_attribute attr_generator = child.attribute( node_includes->generator.c_str() );
                
                const char * value_generator = attr_generator.value();

                const XmlNode * node_generator = _node->getGenerator( value_generator );

                if( node_generator == 0 )
                {
                    m_error << "Xml2Metabuf::writeNodeIncludes_: error write node " << _node->name << " includes " << node_includes->name << " not found generator " << value_generator << std::endl;

                    return false;
                }

                if( this->writeNode_( node_generator, child ) == false )
				{
                    m_error << "Xml2Metabuf::writeNodeIncludes_: error write node " << _node->name << " includes " << node_generator->name << std::endl;

					return false;
				}
            }            
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

            if( _node->getInclude( child_name ) == 0 )
            {
                continue;
            }
            
            ++count;
        }

        _count = count;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeSize( size_t _value )
    {
        if( _value > 255 )
        {
            return false;
        }

        unsigned char size = (unsigned char)_value;

        this->writeBuffer( (const char * )&size, 1 );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::writeBuffer( const char * _buff, size_t _size )
    {
        if( m_write + _size > m_size )
        {
            throw std::exception();
        }

        memcpy( m_out + m_write, _buff, _size );
        m_write += _size;
    }
}