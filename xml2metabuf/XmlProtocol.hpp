#	pragma once

#	include <pugixml.hpp>

#	include <string>
#	include <map>
#	include <vector>

namespace Metabuf
{
	struct XmlAttribute
	{
		size_t id;
		std::string name;
		std::string type;
		std::string evict;
		bool required;
	};

	typedef std::map<std::string, XmlAttribute> TMapAttributes;

	struct XmlMember
	{
		std::string name;
		TMapAttributes attributes;

        const XmlAttribute * getAttribute( const std::string & _name ) const;
	};
	
	typedef std::map<std::string, XmlMember> TMapMembers;
	
	typedef std::vector<std::string> TVectorIncludes;

	struct XmlNode
	{
		size_t id;
		std::string name;
		std::string generator;
		std::string inheritance;

        const XmlNode * node_inheritance;

        mutable size_t enumerator;

		TMapAttributes attributes;
		TMapMembers members;

		TVectorIncludes includes;

		const XmlAttribute * getAttribute( const std::string & _name ) const;
		const XmlMember * getMember( const std::string & _name ) const;
        bool isInclude( const std::string & _name ) const;
                
        std::string getName() const;
	};

	typedef std::map<std::string, XmlNode> TMapNodes;

	class XmlProtocol
	{
	public:
		XmlProtocol();

	public:
		const XmlNode * getNode( const std::string & _name ) const;
		const TMapNodes & getNodes() const;

	public:
		bool readProtocol( const void * _buff, size_t _size );

	protected:
		bool readNode_( const pugi::xml_node & _node );
		
	protected:
		size_t m_enumerator;

		TMapNodes m_nodes;
	};
}