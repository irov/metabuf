#	include "XmlProtocol.hpp"

#   include <algorithm>

#   include <stdio.h>
#   include <string.h>
#   include <memory.h>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    const XmlAttribute * XmlMember::getAttribute( const std::string & _name ) const
    {
        TMapAttributes::const_iterator it_found = this->attributes.find( _name );

        if( it_found == this->attributes.end() )
        {
            return nullptr;
        }

        const XmlAttribute * attr = &it_found->second;

        return attr;
    }
    //////////////////////////////////////////////////////////////////////////
    XmlNode::XmlNode()
        : id(0)
        , node_inheritance(nullptr)
        , node_scope(nullptr)
        , enumerator(0)
        , noWrite(false)
    {

    }
    //////////////////////////////////////////////////////////////////////////
    XmlNode::~XmlNode()
    {
        for( TMapNodes::const_iterator
            it = includes.begin(),
            it_end = includes.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node = it->second;

            delete node;
        }

        for( TMapNodes::const_iterator
            it = inheritances.begin(),
            it_end = inheritances.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node = it->second;

            delete node;
        }

        for( TMapNodes::const_iterator
            it = generators.begin(),
            it_end = generators.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node = it->second;

            delete node;
        }
    }
	//////////////////////////////////////////////////////////////////////////
	const XmlAttribute * XmlNode::getAttribute( const std::string & _name ) const
	{
		TMapAttributes::const_iterator it_found = this->attributes.find( _name );

		if( it_found == this->attributes.end() )
		{
            if( this->node_inheritance == nullptr )
            {
        		return nullptr;
            }
            else
            {
                const XmlAttribute * attr_inheritance = this->node_inheritance->getAttribute( _name );

                return attr_inheritance;
            }
		}

        const XmlAttribute * attr = &it_found->second;

		return attr;
	}
	//////////////////////////////////////////////////////////////////////////
	const XmlMember * XmlNode::getMember( const std::string & _name ) const
	{
		TMapMembers::const_iterator it_found = this->members.find( _name );

		if( it_found == this->members.end() )
		{
            if( this->node_inheritance == nullptr )
            {
                return nullptr;
            }
            else
            {
                const XmlMember * member_inheritance = this->node_inheritance->getMember( _name );

                return member_inheritance;
            }
		}

        const XmlMember * member = &it_found->second;

		return member;
	}
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getInclude( const std::string & _name ) const
    {
        TMapNodes::const_iterator it_found = this->includes.find( _name );

        if( it_found == this->includes.end() )
        {
            return nullptr;
        }

        XmlNode * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getInheritances( const std::string & _name ) const
    {
        TMapNodes::const_iterator it_found = this->inheritances.find( _name );

        if( it_found == this->inheritances.end() )
        {
            return nullptr;
        }

        XmlNode * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getGenerator( const std::string & _name ) const
    {
        TMapNodes::const_iterator it_found = this->generators.find( _name );

        if( it_found == this->generators.end() )
        {
            return nullptr;
        }

        XmlNode * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlNode::getName() const
    {
        std::string write_name;
        write_name += "Meta_";
        write_name += this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlNode::getScope() const
    {
        std::string write_scope;

        if( this->node_scope == nullptr )
        {
            write_scope += this->getName();
            return write_scope;
        }
        
        write_scope += this->node_scope->getScope();
        write_scope += "::";
        write_scope += this->getName();

        return write_scope;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlNode::getNoWrite() const
    {
        return noWrite;
    }
	//////////////////////////////////////////////////////////////////////////
	XmlProtocol::XmlProtocol()
		: m_enumerator(0)
        , m_version(0)
	{
	}
    //////////////////////////////////////////////////////////////////////////
    XmlProtocol::~XmlProtocol()
    {
        for( TMapNodes::const_iterator
            it = m_nodes.begin(),
            it_end = m_nodes.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node = it->second;
            
            delete node;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlProtocol::getError()
    {
        return m_error.str();
    }
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::readProtocol( const void * _buff, size_t _size )
	{
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_buffer( _buff, _size );

		if( result == false )
		{
            m_error << "XmlProtocol::readProtocol:" << result.description() << std::endl;

			return false;
		}

		pugi::xml_node root = doc.document_element();

		if( root.empty() == true )
		{
			return true;
		}

        pugi::xml_attribute xml_version = root.attribute("Version");

        const char * version_value = xml_version.value();

        if( sscanf( version_value, "%u", &m_version ) != 1 )
        {
            m_error << "XmlProtocol::readProtocol: not set version!" << std::endl;

            return false;
        }

		for( pugi::xml_node::iterator
			it = root.begin(),
			it_end = root.end();
		it != it_end;
		++it )
		{
			const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == 0 )
            {
                return false;
            }

            if( strcmp( element_name, "Type" ) == 0 )
            {
                if( this->readType_( element ) == false )
                {                   
                    return false;
                }
            }			
			else if( strcmp( element_name, "Node" ) == 0 )
			{
				if( this->readNode_( 0, element ) == false )
                {
                    return false;
                }
			}
		}

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::readType_( const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute Name = _xml_node.attribute("Name");
        pugi::xml_attribute Evict = _xml_node.attribute("Evict");

        const char * type_name = Name.value();
        const char * type_evict = Evict.value();

        if( type_name == 0 || type_evict == 0 )
        {
            m_error << "XmlProtocol::readType_: Name or Evict not set" << std::endl;

            return false;
        }

        m_evictors.insert( std::make_pair(type_name, type_evict));

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::readNode_( XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		pugi::xml_attribute Name = _xml_node.attribute("Name");
		pugi::xml_attribute Generator = _xml_node.attribute("Generator");
		pugi::xml_attribute Inheritance = _xml_node.attribute("Inheritance");
        pugi::xml_attribute NoWrite = _xml_node.attribute("NoWrite");
		
		XmlNode * nodeXml = new XmlNode();
        
        if( _node == 0 )
        {
            m_nodes.insert( std::make_pair(Name.value(), nodeXml) );
        }
        else
        {           
            if( Generator.empty() == false )
            {
                _node->inheritances.insert( std::make_pair(Name.value(), nodeXml) );
            }
            else if( Inheritance.empty() == false )
            {
                _node->generators.insert( std::make_pair(Name.value(), nodeXml) );
            }
            else
            {
                _node->includes.insert( std::make_pair(Name.value(), nodeXml) );
            }
        }

        if( NoWrite.empty() == false )
        {
            nodeXml->noWrite = true;
        }

        if( _node == 0 )
        {
		    nodeXml->id = ++m_enumerator;
        }
        else
        {
            nodeXml->id = ++_node->enumerator;
        }

        nodeXml->enumerator = 0;
        nodeXml->node_inheritance = NULL;
        nodeXml->node_scope = _node;
		nodeXml->name = Name.value();

		if( Generator.empty() == false )
		{
			nodeXml->generator = Generator.value();
		}

		if( Inheritance.empty() == false )
		{
			nodeXml->inheritance = Inheritance.value();

            nodeXml->node_inheritance = _node->getInheritances( nodeXml->inheritance );

            if( nodeXml->node_inheritance == NULL )
            {
                m_error << "XmlProtocol::readNode_: node " << nodeXml->name << " not found inheritance " << nodeXml->inheritance << std::endl;

                return false;
            }
            
            nodeXml->enumerator = nodeXml->node_inheritance->enumerator;
		}

		for( pugi::xml_node::iterator
			it = _xml_node.begin(),
			it_end = _xml_node.end();
		it != it_end;
		++it )
		{
			const pugi::xml_node & element = *it;

			if( strcmp( element.name(), "Attribute" ) == 0 )
			{
				pugi::xml_attribute Name = element.attribute("Name");
				pugi::xml_attribute Type = element.attribute("Type");
				//pugi::xml_attribute Evict = element.attribute("Evict");
				pugi::xml_attribute Required = element.attribute("Required");

				XmlAttribute & attributeXml = nodeXml->attributes[ Name.value() ];
                
				attributeXml.id = ++nodeXml->enumerator;
				attributeXml.name = Name.value();
				attributeXml.type = Type.value();
				attributeXml.required = Required.empty() == false;
			}
			else if( strcmp( element.name(), "Member" ) == 0 )
			{
				pugi::xml_attribute Node = element.attribute("Node");
				pugi::xml_attribute Name = element.attribute("Name");
				pugi::xml_attribute Type = element.attribute("Type");
				//pugi::xml_attribute Evict = element.attribute("Evict");
                pugi::xml_attribute Required = element.attribute("Required");

				XmlMember & memberXml = nodeXml->members[ Node.value() ];

				memberXml.name = Node.value();

				XmlAttribute & attributeXml = memberXml.attributes[ Name.value() ];

				attributeXml.id = ++nodeXml->enumerator;
				attributeXml.name = Name.value();
				attributeXml.type = Type.value();
				attributeXml.required = Required.empty() == false;
			}
            else if( strcmp( element.name(), "Inheritance" ) == 0 )
            {
            }
            else if( strcmp( element.name(), "Node" ) == 0 )
            {
                if( this->readNode_( nodeXml, element ) == false )
                {                    
                    return false;
                }
            }
		}

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::getEvict( const std::string & _type, std::string & _evict ) const
    {
        TMapEvictors::const_iterator it_found = m_evictors.find( _type );

        if( it_found == m_evictors.end() )
        {
            return false;
        }

        _evict = it_found->second;

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	const XmlNode * XmlProtocol::getNode( const std::string & _name ) const
	{
		TMapNodes::const_iterator it_found = m_nodes.find( _name );

		if( it_found == m_nodes.end() )
		{
			return 0;
		}

		return it_found->second;
	}
	//////////////////////////////////////////////////////////////////////////
	const TMapNodes & XmlProtocol::getNodes() const
	{
		return m_nodes;
	}
    //////////////////////////////////////////////////////////////////////////
    uint32_t XmlProtocol::getVersion() const
    {
        return m_version;
    }
}