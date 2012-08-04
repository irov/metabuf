#	include "XmlProtocol.hpp"

#   include <algorithm>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    const XmlAttribute * XmlMember::getAttribute( const std::string & _name ) const
    {
        TMapAttributes::const_iterator it_found = this->attributes.find( _name );

        if( it_found == this->attributes.end() )
        {
            return 0;
        }

        const XmlAttribute * attr = &it_found->second;

        return attr;
    }
	//////////////////////////////////////////////////////////////////////////
	const XmlAttribute * XmlNode::getAttribute( const std::string & _name ) const
	{
		TMapAttributes::const_iterator it_found = this->attributes.find( _name );

		if( it_found == this->attributes.end() )
		{
            if( this->node_inheritance == 0 )
            {
        		return 0;
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
            if( this->node_inheritance == 0 )
            {
                return 0;
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
    bool XmlNode::isInclude( const std::string & _name ) const
    {
        TVectorIncludes::const_iterator it_found = std::find( this->includes.begin(), this->includes.end(), _name );

        if( it_found == this->includes.end() )
        {
            return false;
        }

        return true;
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
	XmlProtocol::XmlProtocol()
		: m_enumerator(0)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::readProtocol( const void * _buff, size_t _size )
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

		if( root.empty() == true )
		{
			return 0;
		}

		for( pugi::xml_node::iterator
			it = root.begin(),
			it_end = root.end();
		it != it_end;
		++it )
		{
			const pugi::xml_node & element = *it;

			
			//element->attribute()
			if( strcmp( element.name(), "Node" ) == 0 )
			{
				this->readNode_( element );
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::readNode_( const pugi::xml_node & _node )
	{
		pugi::xml_attribute Name = _node.attribute("Name");
		pugi::xml_attribute Generator = _node.attribute("Generator");
		pugi::xml_attribute Inheritance = _node.attribute("Inheritance");
		
		XmlNode & nodeXml = m_nodes[Name.value()];

		nodeXml.id = ++m_enumerator;
        nodeXml.enumerator = 0;
        nodeXml.node_inheritance = NULL;
		nodeXml.name = Name.value();

		if( Generator.empty() == false )
		{
			nodeXml.generator = Generator.value();
		}

		if( Inheritance.empty() == false )
		{
			nodeXml.inheritance = Inheritance.value();

            nodeXml.node_inheritance = this->getNode( nodeXml.inheritance );

            nodeXml.enumerator = nodeXml.node_inheritance->enumerator;
		}

		for( pugi::xml_node::iterator
			it = _node.begin(),
			it_end = _node.end();
		it != it_end;
		++it )
		{
			const pugi::xml_node & element = *it;

			if( strcmp( element.name(), "Attribute" ) == 0 )
			{
				pugi::xml_attribute Name = element.attribute("Name");
				pugi::xml_attribute Type = element.attribute("Type");
				pugi::xml_attribute Evict = element.attribute("Evict");
				pugi::xml_attribute Required = element.attribute("Required");

				XmlAttribute & attributeXml = nodeXml.attributes[ Name.value() ];
                
				attributeXml.id = ++nodeXml.enumerator;
				attributeXml.name = Name.value();
				attributeXml.type = Type.value();
				attributeXml.evict = Evict.value();
				attributeXml.required = Required.empty() == false;
			}
			else if( strcmp( element.name(), "Member" ) == 0 )
			{
				pugi::xml_attribute Node = element.attribute("Node");
				pugi::xml_attribute Name = element.attribute("Name");
				pugi::xml_attribute Type = element.attribute("Type");
				pugi::xml_attribute Evict = element.attribute("Evict");

				XmlMember & memberXml = nodeXml.members[ Node.value() ];

				memberXml.name = Node.value();

				XmlAttribute & attributeXml = memberXml.attributes[ Name.value() ];

				attributeXml.id = ++nodeXml.enumerator;
				attributeXml.name = Name.value();
				attributeXml.type = Type.value();
				attributeXml.evict = Evict.value();
				attributeXml.required = false;
			}
			else if( strcmp( element.name(), "Includes" ) == 0 )
			{
				pugi::xml_attribute Node = element.attribute("Node");

				nodeXml.includes.push_back( Node.value() );
			}
		}

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

		return &it_found->second;
	}
	//////////////////////////////////////////////////////////////////////////
	const TMapNodes & XmlProtocol::getNodes() const
	{
		return m_nodes;
	}
}