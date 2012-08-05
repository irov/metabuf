#	pragma once

#   include "XmlProtocol.hpp"

#	include <pugixml.hpp>

#   include <map>
#	include <string>
#	include <sstream>

namespace Metabuf
{
	class Xml2Metabuf
	{
	public:
		Xml2Metabuf( char * _out, XmlProtocol * _protocol );

    public:
        void initialize();

	public:
		bool convert( const void * _buff, size_t _size, size_t & _write );
		std::string getError();


	protected:
		bool writeNode_( const XmlNode * _node, const pugi::xml_node & _xml_node );

        bool writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeAttributeSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, size_t & _count );

        bool writeNodeIncludes_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeIncludesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, size_t & _count );

		bool writeNodeArguments_( const XmlAttribute * _node, const pugi::xml_attribute & _xml_attr );

    public:
        template<class T>
        void write( const T & _value )
        {
            this->writeBuffer( (const char * )&_value, sizeof(T) );
        }   

        bool writeSize( size_t _value );

        template<class T>
        void writeCount( const T * _value, size_t _count )
        {
            this->writeBuffer( (const char * )_value, sizeof(T) * _count );
        }   
        
    protected:
        void writeBuffer( const char * _buff, size_t _size );

	protected:
		char * m_out;
		size_t m_write;

		XmlProtocol * m_protocol;

		std::stringstream m_error;

        typedef bool (*ValueSerialization)( Xml2Metabuf * _metabuf, const char * _value );
        typedef std::map<std::string, ValueSerialization> TMapSerialization;
        TMapSerialization m_serialization;
	};
}