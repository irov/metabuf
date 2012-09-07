#	pragma once

#	include <pugixml.hpp>

#	include <string>
#	include <map>
#	include <vector>

#   include <sstream>

namespace Metabuf
{
	struct XmlAttribute
	{
		size_t id;
		std::string name;
		std::string type;
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

    typedef std::map<std::string, struct XmlNode *> TMapNodes;

	struct XmlNode
	{
        XmlNode();
        ~XmlNode();

		size_t id;
		std::string name;
		std::string generator;
		std::string inheritance;

        const XmlNode * node_inheritance;
        const XmlNode * node_scope;

        mutable size_t enumerator;

		TMapAttributes attributes;
		TMapMembers members;

		TMapNodes includes;
        TMapNodes inheritances;
        TMapNodes generators;

		const XmlAttribute * getAttribute( const std::string & _name ) const;
		const XmlMember * getMember( const std::string & _name ) const;
        
        const XmlNode * getInclude( const std::string & _name ) const;
        const XmlNode * getInheritances( const std::string & _name ) const;
        const XmlNode * getGenerator( const std::string & _name ) const;

        const TMapNodes & getGenerators() const;
                
        std::string getName() const;
        std::string getScope() const;
	};
    
	class XmlProtocol
	{
	public:
		XmlProtocol();
        ~XmlProtocol();

	public:
		const XmlNode * getNode( const std::string & _name ) const;
		const TMapNodes & getNodes() const;

    public:
        unsigned int getVersion() const;

    public:
        bool getEvict( const std::string & _type, std::string & _evict ) const;

	public:
		bool readProtocol( const void * _buff, size_t _size );
        std::string getError();

	protected:
        bool readType_( const pugi::xml_node & _xml_node );
		bool readNode_( XmlNode * _node, const pugi::xml_node & _xml_node );
		
	protected:
        unsigned int m_version;

		size_t m_enumerator;

		TMapNodes m_nodes;

        typedef std::map<std::string, std::string> TMapEvictors;
        TMapEvictors m_evictors;

        std::stringstream m_error;
	};
}