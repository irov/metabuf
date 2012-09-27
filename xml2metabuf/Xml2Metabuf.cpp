#	include "Xml2Metabuf.hpp"
#	include "XmlProtocol.hpp"

#	include <sstream>

#   include <stdio.h>
#   include <string.h>
#   include <memory.h>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    namespace Serialize
    {
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_string( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            size_t size = strlen( _value );
            _metabuf->write( size );

            _metabuf->writeCount( _value, size );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_bool( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            int int_value;
            if( sscanf( _value, "%d", &int_value ) != 1 )
            {
                return false;
            }

            bool value = (int_value != 0);
            _metabuf->write( value );

            return true;
        }      
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_int( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            int value;
            if( sscanf( _value, "%d", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_size_t( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            size_t value;
            if( sscanf( _value, "%u", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            float value;
            if( sscanf( _value, "%f", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float2( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            float value[2];
            if( sscanf( _value, "%f;%f", &value[0], &value[1] ) != 2 )
            {
                if( sscanf( _value, "%f %f", &value[0], &value[1] ) != 2 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 2 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float3( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            float value[3];
            if( sscanf( _value, "%f;%f;%f", &value[0], &value[1], &value[2] ) != 3 )
            {
                if( sscanf( _value, "%f %f %f", &value[0], &value[1], &value[2] ) != 3 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 3 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float4( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            float value[4];
            if( sscanf( _value, "%f;%f;%f;%f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
            {
                if( sscanf( _value, "%f %f %f %f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 4 );

            return true;
        }
    }
	//////////////////////////////////////////////////////////////////////////
	Xml2Metabuf::Xml2Metabuf( char * _out, size_t _size, XmlProtocol * _protocol )
		: m_out(_out)
        , m_size(_size)
		, m_write(0)
		, m_protocol(_protocol)
	{

	}
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::initialize()
    {
        this->addSerializator( "string", &Serialize::s_write_string, 0 );
        
        this->addSerializator( "bool", &Serialize::s_write_bool, 0 );
        this->addSerializator( "int", &Serialize::s_write_int, 0 );
        this->addSerializator( "size_t", &Serialize::s_write_size_t, 0 );
        
        this->addSerializator( "float", &Serialize::s_write_float, 0 );
        this->addSerializator( "float2", &Serialize::s_write_float2, 0 );
        this->addSerializator( "float3", &Serialize::s_write_float3, 0 );
        this->addSerializator( "float4", &Serialize::s_write_float4, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::addSerializator( const std::string & _type, ValueSerialization _serializator, void * _user )
    {
        SerializationDesc desc;
        desc.serialization = _serializator;
        desc.user = _user;

        m_serialization[_type] = desc;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::convert( const void * _buff, size_t _size, size_t & _write )
	{	
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_buffer( _buff, _size );

		if( result == false )
		{
            m_error << "Xml2Metabuf::convert xml parser error:" << std::endl << result.description() << std::endl;

			return false;
		}

        unsigned int magic = 3133062829u;
        this->write( magic );

        unsigned int version = m_protocol->getVersion();
        this->write( version );

		pugi::xml_node root = doc.document_element();

        const char * root_name = root.name();

        const XmlNode * node_root = m_protocol->getNode( root_name );

        if( node_root == NULL )
        {
            m_error << "Xml2Metabuf::convert: invalid root node" << root_name << std::endl;

            return false;
        }

		if( this->writeNode_( node_root, root ) == false )
		{
            m_error << "Xml2Metabuf::convert: invalid write node" << root_name << std::endl;

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
        if( _xml_node.begin() == _xml_node.end() && _xml_node.attributes_begin() == _xml_node.attributes_end() )
        {
            return true;
        }

		//size_t id = _node->id;
		//if( this->writeSize( id ) == false )
  //      {
  //          return false;
  //      }

        if( this->writeNodeAttribute_( _node, _xml_node ) == false )
        {
            return false;
        }

        if( this->writeNodeIncludes_( _node, _xml_node ) == false )
        {
            return false;
        }

        if( this->writeNodeGenerators_( _node, _xml_node ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        size_t attributeCount;
        if( this->getNodeAttributeSize_( _node, _xml_node, attributeCount ) == false )
        {
            return false;
        }

        if( this->writeSize( attributeCount ) == false )
        {
            return false;
        }

        if( _node->inheritance.empty() == false )
        {
            if( this->writeNodeAttribute2_( _node->node_inheritance, _xml_node ) == false )
            {
                return false;
            }
        }

        if( this->writeNodeAttribute2_( _node, _xml_node ) == false )
        {
            return false;
        }

        for( TMapMembers::const_iterator
            it = _node->members.begin(),
            it_end = _node->members.end();
        it != it_end;
        ++it )
        {
            const XmlMember * member = &it->second;
            
            for( TMapAttributes::const_iterator
                it = member->attributes.begin(),
                it_end = member->attributes.end();
            it != it_end;
            ++it )
            {
                const XmlAttribute * attr = &it->second;

                bool member_found = false;

                for( pugi::xml_node::iterator
                    it = _xml_node.begin(),
                    it_end = _xml_node.end();
                it != it_end;
                ++it )
                {
                    pugi::xml_node & child = *it;

                    const char * child_name = child.name();

                    if( member->name != child_name )
                    {
                        continue;
                    }
                    
                    pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

                    if( xml_attr.empty() == true )
                    {
                        continue;
                    }

                    if( this->writeNodeArguments_( attr, xml_attr ) == false )
                    {
                        m_error << "Xml2Metabuf::writeNodeAttribute_:" << _node->name << " not write member " << member->name << " argument " << attr->name << std::endl;

                        return false;
                    }

                    member_found = true;
                    break;
                }

                if( attr->required == true )
                {
                    if( member_found == false )
                    {                   
                        m_error << "Xml2Metabuf::writeNodeAttribute_:" << _node->name << " member " << member->name << " not found required argument " << attr->name << std::endl;
                            
                        return false;
                    }
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

            if( xml_attr.empty() == true )
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

        std::string evict;
        m_protocol->getEvict( _attr->type, evict );

		TMapSerialization::const_iterator it_serialize = m_serialization.find( evict );

		if( it_serialize == m_serialization.end() )
		{
			m_error << "Xml2Metabuf::writeNodeArguments_: not found serialize " << evict << " for attribute " << _attr->name << std::endl;

			return false;
		}

		const char * attr_value = _xml_attr.value();

        const SerializationDesc & desc = it_serialize->second;

		if( (*desc.serialization)( this, attr_value, desc.user ) == false )
		{
			m_error << "Xml2Metabuf::writeNodeAttribute_: serialize " << evict << " for attribute " << _attr->name << " error for value " << attr_value << std::endl;

			return false;
		}

        return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::getNodeAttributeSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, size_t & _count )
    {    
        size_t count = 0;

        for( TMapAttributes::const_iterator
            it = _node->attributes.begin(),
            it_end = _node->attributes.end();
        it != it_end;
        ++it )
        {
            const XmlAttribute * attr = &it->second;

            pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

            if( xml_attr.empty() == true )
            {
                if( attr->required == true )
                {
                    m_error << "Xml2Metabuf::getNodeAttributeSize_: node " << _node->name << " not found attribute " << attr->name << std::endl;

                    return false;
                }
                else
                {
                    continue;
                }
            }

            ++count;
        }

        if( _node->inheritance.empty() == false )
        {
            for( TMapAttributes::const_iterator
                it = _node->node_inheritance->attributes.begin(),
                it_end = _node->node_inheritance->attributes.end();
            it != it_end;
            ++it )
            {
                const XmlAttribute * attr = &it->second;

                pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

                if( xml_attr.empty() == true )
                {
                    if( attr->required == true )
                    {
                        m_error << "Xml2Metabuf::getNodeAttributeSize_: node " << _node->name << " not found attribute " << attr->name << std::endl;

                        return false;
                    }
                    else
                    {
                        continue;
                    }
                }

                ++count;
            }
        }

        for( TMapMembers::const_iterator
            it = _node->members.begin(),
            it_end = _node->members.end();
        it != it_end;
        ++it )
        {
            const XmlMember * member = &it->second;

            for( TMapAttributes::const_iterator
                it = member->attributes.begin(),
                it_end = member->attributes.end();
            it != it_end;
            ++it )
            {
                const XmlAttribute * attr = &it->second;

                bool member_found = false;

                for( pugi::xml_node::iterator
                    it = _xml_node.begin(),
                    it_end = _xml_node.end();
                it != it_end;
                ++it )
                {
                    const pugi::xml_node & child = *it;

                    const char * child_name = child.name();

                    if( member->name != child_name )
                    {
                        continue;
                    }

                    pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

                    if( xml_attr.empty() == true )
                    {
                        continue;
                    }

                    ++count;
                    
                    member_found = true;

                    break;
                }

                if( attr->required == true )
                {
                    if( member_found == false )
                    {                   
                        m_error << "Xml2Metabuf::getNodeAttributeSize_:" << _node->name << " member " << member->name << " not found required argument " << attr->name << std::endl;

                        return false;
                    }
                }
            }
        }

        _count = count;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeIncludes_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        size_t includesTypeCount = _node->includes.size();
        this->writeSize( includesTypeCount );

        for( TMapNodes::const_iterator
            it = _node->includes.begin(),
            it_end = _node->includes.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node_include = it->second;
            
            size_t incluidesCount;
            this->getNodeIncludesSize_( _node, _xml_node, node_include->name, incluidesCount );

            this->write( incluidesCount );

            this->writeSize( node_include->id );

            for( pugi::xml_node::iterator
                it = _xml_node.begin(),
                it_end = _xml_node.end();
            it != it_end;
            ++it )
            {
                const pugi::xml_node & child = *it;

                const char * child_name = child.name();

                if( node_include->name != child_name )
                {
                    continue;
                }

                if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
                {
                    continue;
                }
                
                if( this->writeNode_( node_include, child ) == false )
                {
                    m_error << "Xml2Metabuf::writeNodeIncludes_: error write node " << _node->name << " includes " << node_include->name << std::endl;

                    return false;
                }
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeGenerators_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        size_t generatorsTypeCount = _node->inheritances.size();
        this->writeSize( generatorsTypeCount );

        for( TMapNodes::const_iterator
            it = _node->inheritances.begin(),
            it_end = _node->inheritances.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node_inheritance = it->second;

            size_t generatorsCount;
            this->getNodeGeneratorSize_( _node, _xml_node, node_inheritance->name, generatorsCount );

            this->write( generatorsCount );

            this->writeSize( node_inheritance->id );

            for( pugi::xml_node::iterator
                it = _xml_node.begin(),
                it_end = _xml_node.end();
            it != it_end;
            ++it )
            {
                const pugi::xml_node & child = *it;

                const char * child_name = child.name();

                if( node_inheritance->name != child_name )
                {
                    continue;
                }

                if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
                {
                    continue;
                }
                                                
                pugi::xml_attribute attr_generator = child.attribute( node_inheritance->generator.c_str() );

                const char * value_generator = attr_generator.value();

                const XmlNode * node_generator = _node->getGenerator( value_generator );

                if( node_generator == 0 )
                {
                    m_error << "Xml2Metabuf::writeNodeIncludes_: error write node " << _node->name << " includes " << node_inheritance->name << " not found generator " << value_generator << std::endl;

                    return false;
                }

                this->writeSize( node_generator->id );

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
    bool Xml2Metabuf::getNodeIncludesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, size_t & _count )
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

            if( _type != child_name )
            {
                continue;
            }

            if( _node->getInclude( child_name ) == NULL )
            {
                continue;
            }

            if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
            {
                continue;
            }
            
            ++count;
        }

        _count = count;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::getNodeGeneratorSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, size_t & _count )
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

            if( _type != child_name )
            {
                continue;
            }

            if( _node->getInheritances( child_name ) == NULL )
            {
                continue;
            }

            if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
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