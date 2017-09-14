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
		: id( 0 )
		, node_inheritance( nullptr )
		, node_scope( nullptr )
		, enumerator( 0 )
		, noWrite( false )
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
	const XmlNode * XmlNode::getSingle( const std::string & _name ) const
	{
		TMapNodes::const_iterator it_found = this->singles.find( _name );

		if( it_found == this->singles.end() )
		{
			return nullptr;
		}

		XmlNode * node = it_found->second;

		return node;
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

		const XmlNode * node = it_found->second;

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
	bool XmlNode::hasNode( const std::string & _type ) const
	{
		TMapNodes::const_iterator it_singles_found = singles.find( _type );

		if( it_singles_found != singles.end() )
		{
			return true;
		}

		for( TMapNodes::const_iterator
			it = singles.begin(),
			it_end = singles.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			if( node->hasNode( _type ) == true )
			{
				return true;
			}
		}

		TMapNodes::const_iterator it_includes_found = includes.find( _type );

		if( it_includes_found != includes.end() )
		{
			return true;
		}

		for( TMapNodes::const_iterator
			it = includes.begin(),
			it_end = includes.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			if( node->hasNode( _type ) == true )
			{
				return true;
			}
		}

		TMapNodes::const_iterator it_inheritances_found = inheritances.find( _type );

		if( it_inheritances_found != inheritances.end() )
		{
			return true;
		}

		for( TMapNodes::const_iterator
			it = inheritances.begin(),
			it_end = inheritances.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			if( node->hasNode( _type ) == true )
			{
				return true;
			}
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	const XmlNode * XmlNode::getNode( const std::string & _type ) const
	{
		TMapNodes::const_iterator it_singles_found = singles.find( _type );

		if( it_singles_found != singles.end() )
		{
			const XmlNode * node = it_singles_found->second;

			return node;
		}

		for( TMapNodes::const_iterator
			it = singles.begin(),
			it_end = singles.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			const XmlNode * found_node = node->getNode( _type );

			if( found_node != nullptr )
			{
				return found_node;
			}
		}

		TMapNodes::const_iterator it_includes_found = includes.find( _type );

		if( it_includes_found != includes.end() )
		{
			const XmlNode * node = it_includes_found->second;

			return node;
		}

		for( TMapNodes::const_iterator
			it = includes.begin(),
			it_end = includes.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			const XmlNode * found_node = node->getNode( _type );

			if( found_node != nullptr )
			{
				return found_node;
			}
		}

		TMapNodes::const_iterator it_inheritances_found = inheritances.find( _type );

		if( it_inheritances_found != inheritances.end() )
		{
			const XmlNode * node = it_inheritances_found->second;

			return node;
		}

		for( TMapNodes::const_iterator
			it = inheritances.begin(),
			it_end = inheritances.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			const XmlNode * found_node = node->getNode( _type );

			if( found_node != nullptr )
			{
				return found_node;
			}
		}

		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	XmlProtocol::XmlProtocol()
		: m_enumerator( 0 )
		, m_version( 0 )
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

		pugi::xml_attribute xml_version = root.attribute( "Version" );

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

			if( strcmp( element_name, "Enum" ) == 0 )
			{
				if( this->readEnum_( element ) == false )
				{
					return false;
				}
			}
		}

		for( pugi::xml_node::iterator
			it = root.begin(),
			it_end = root.end();
			it != it_end;
			++it )
		{
			const pugi::xml_node & element = *it;

			const char * element_name = element.name();

			if( strcmp( element_name, "Node" ) == 0 )
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
	bool XmlProtocol::readEnum_( const pugi::xml_node & _xml_node )
	{
		pugi::xml_attribute Name = _xml_node.attribute( "Name" );
		pugi::xml_attribute Type = _xml_node.attribute( "Type" );

		if( Name.empty() == true )
		{
			m_error << "XmlProtocol::readEnum_: Name not set" << std::endl;

			return false;
		}

		const char * type_name = Name.value();
		const char * type_write = Type.value();


		XmlType type;
		type.is_enumerator = true;
		type.write = (strlen( type_write ) == 0) ? type_name : type_write;
		type.evict = "uint32_t";

		for( pugi::xml_node::iterator
			it = _xml_node.begin(),
			it_end = _xml_node.end();
			it != it_end;
			++it )
		{
			const pugi::xml_node & element = *it;

			const char * element_name = element.name();

			if( element_name == 0 )
			{
				m_error << "XmlProtocol::readEnum_: Enum '" << type_name << "' invalid Enum Value" << std::endl;

				return false;
			}

			type.enumerators.push_back( element_name );
		}

		m_types.insert( std::make_pair( type_name, type ) );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::readType_( const pugi::xml_node & _xml_node )
	{
		pugi::xml_attribute Name = _xml_node.attribute( "Name" );
		pugi::xml_attribute Type = _xml_node.attribute( "Type" );
		pugi::xml_attribute Evict = _xml_node.attribute( "Evict" );
		pugi::xml_attribute NCR = _xml_node.attribute( "NCR" );

		if( Name.empty() == true || Evict.empty() == true )
		{
			m_error << "XmlProtocol::readType_: Name or Evict not set" << std::endl;

			return false;
		}

		const char * type_name = Name.value();
		const char * type_write = Type.value();
		const char * type_evict = Evict.value();

		XmlType type;
		type.is_enumerator = false;
		type.write = (strlen( type_write ) == 0) ? type_name : type_write;
		type.evict = type_evict;

		uint32_t type_NCR_value = 0;

		if( NCR != nullptr )
		{
			const char * type_NCR = NCR.value();

			if( sscanf( type_NCR, "%d", &type_NCR_value ) != 1 )
			{
				m_error << "XmlProtocol::readType_: NCR invalid" << std::endl;

				return false;
			}
		}

		type.is_ncr = (type_NCR_value != 0);

		m_types.insert( std::make_pair( type_name, type ) );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::readNode_( XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		pugi::xml_attribute NodeName = _xml_node.attribute( "Name" );
		pugi::xml_attribute NodeGenerator = _xml_node.attribute( "Generator" );
		pugi::xml_attribute NodeInheritance = _xml_node.attribute( "Inheritance" );
		pugi::xml_attribute NodeNoWrite = _xml_node.attribute( "NoWrite" );
		pugi::xml_attribute NodeSingle = _xml_node.attribute( "Single" );

		XmlNode * nodeXml = new XmlNode();

		if( _node == nullptr )
		{
			m_nodes.insert( std::make_pair( NodeName.value(), nodeXml ) );
		}
		else
		{
			if( NodeSingle.empty() == false && strcmp( NodeSingle.value(), "1" ) == 0 )
			{
				if( _node->singles.find( NodeName.value() ) != _node->singles.end() )
				{
					m_error << "XmlProtocol::readNode_: node " << nodeXml->name << " have multiply single element " << NodeSingle.value() << std::endl;

					return false;
				}

				_node->singles.insert( std::make_pair( NodeName.value(), nodeXml ) );
			}
			else if( NodeGenerator.empty() == false )
			{
				_node->inheritances.insert( std::make_pair( NodeName.value(), nodeXml ) );
			}
			else if( NodeInheritance.empty() == false )
			{
				_node->generators.insert( std::make_pair( NodeName.value(), nodeXml ) );
			}
			else
			{
				_node->includes.insert( std::make_pair( NodeName.value(), nodeXml ) );
			}
		}

		if( NodeNoWrite.empty() == false )
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
		nodeXml->name = NodeName.value();

		if( NodeGenerator.empty() == false )
		{
			nodeXml->generator = NodeGenerator.value();
		}

		if( NodeInheritance.empty() == false )
		{
			nodeXml->inheritance = NodeInheritance.value();

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

			const char * element_name = element.name();

			if( strcmp( element_name, "Attribute" ) == 0 )
			{
				pugi::xml_attribute AttributeName = element.attribute( "Name" );
				pugi::xml_attribute AttributeType = element.attribute( "Type" );
				pugi::xml_attribute AttributeRequired = element.attribute( "Required" );

				XmlAttribute & attributeXml = nodeXml->attributes[AttributeName.value()];

				attributeXml.id = ++nodeXml->enumerator;
				attributeXml.name = AttributeName.value();
				attributeXml.type = AttributeType.value();
				attributeXml.required = AttributeRequired.empty() == false;

				if( this->hasType( attributeXml.type ) == false )
				{
					m_error << "XmlProtocol::readNode_: Attribute " << nodeXml->name << " not found type " << attributeXml.type << std::endl;

					return false;
				}
			}
			else if( strcmp( element_name, "Member" ) == 0 )
			{
				pugi::xml_attribute MemberNode = element.attribute( "Node" );
				pugi::xml_attribute MemberName = element.attribute( "Name" );
				pugi::xml_attribute MemberType = element.attribute( "Type" );
				//pugi::xml_attribute Evict = element.attribute("Evict");
				pugi::xml_attribute MemberRequired = element.attribute( "Required" );

				XmlMember & memberXml = nodeXml->members[MemberNode.value()];

				memberXml.name = MemberNode.value();

				XmlAttribute & attributeXml = memberXml.attributes[MemberName.value()];

				attributeXml.id = ++nodeXml->enumerator;
				attributeXml.name = MemberName.value();
				attributeXml.type = MemberType.value();
				attributeXml.required = MemberRequired.empty() == false;

				if( this->hasType( attributeXml.type ) == false )
				{
					m_error << "XmlProtocol::readNode_: Member " << nodeXml->name << " not found type " << attributeXml.type << std::endl;

					return false;
				}
			}
			else if( strcmp( element_name, "Children" ) == 0 )
			{
				pugi::xml_attribute ChildrenGroup = element.attribute( "Group" );
				pugi::xml_attribute ChildrenType = element.attribute( "Type" );

				XmlChildren & childrenXml = nodeXml->children[ChildrenGroup.value()];

				childrenXml.group = ChildrenGroup.value();
				childrenXml.type = ChildrenType.value();

				if( this->hasNode( childrenXml.type ) == false )
				{
					m_error << "XmlProtocol::readNode_: Children " << nodeXml->name << " not found type " << childrenXml.type << std::endl;

					return false;
				}
			}
			else if( strcmp( element_name, "Node" ) == 0 )
			{
				if( this->readNode_( nodeXml, element ) == false )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::hasType( const std::string & _name ) const
	{
		TMapTypes::const_iterator it_found = m_types.find( _name );

		if( it_found == m_types.end() )
		{
			return false;
		}
		
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::getType( const std::string & _name, XmlType & _type ) const
	{
		TMapTypes::const_iterator it_found = m_types.find( _name );

		if( it_found == m_types.end() )
		{
			return false;
		}

		_type = it_found->second;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool XmlProtocol::hasNode( const std::string & _type ) const
	{
		TMapNodes::const_iterator it_found = m_nodes.find( _type );

		if( it_found != m_nodes.end() )
		{
			return true;
		}

		for( TMapNodes::const_iterator
			it = m_nodes.begin(),
			it_end = m_nodes.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			if( node->hasNode( _type ) == true )
			{
				return true;
			}
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	const XmlNode * XmlProtocol::getNode( const std::string & _type ) const
	{
		TMapNodes::const_iterator it_found = m_nodes.find( _type );

		if( it_found != m_nodes.end() )
		{
			const XmlNode * found_node = it_found->second;

			return found_node;
		}

		for( TMapNodes::const_iterator
			it = m_nodes.begin(),
			it_end = m_nodes.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			const XmlNode * found_node = node->getNode( _type );

			if( found_node != nullptr )
			{
				return found_node;
			}
		}

		return nullptr;
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