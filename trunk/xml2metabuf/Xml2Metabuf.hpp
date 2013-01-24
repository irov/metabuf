#	pragma once

#   include "XmlProtocol.hpp"

#	include <pugixml.hpp>

#   include <map>
#	include <string>
#	include <sstream>

namespace Metabuf
{
    typedef bool (*ValueSerialization)( class Xml2Metabuf * _metabuf, const char * _value, void * _user );

	class Xml2Metabuf
	{
	public:
		Xml2Metabuf( unsigned char * _out, size_t _size, XmlProtocol * _protocol );

    public:
        void initialize();
        void addSerializator( const std::string & _type, ValueSerialization _serializator, void * _user );

	public:
		bool convert( const void * _buff, size_t _size, size_t & _write );
		std::string getError();


	protected:
		bool writeNode_( const XmlNode * _node, const pugi::xml_node & _xml_node );

        bool writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool writeNodeAttribute2_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeAttributeSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, size_t & _count );

        bool writeNodeIncludes_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool writeNodeGenerators_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeIncludesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, size_t & _count );
        bool getNodeGeneratorSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, size_t & _count );

		bool writeNodeArguments_( const XmlAttribute * _node, const pugi::xml_attribute & _xml_attr );

    public:
        template<class T>
        void write( const T & _value )
        {
            this->writeBuffer( (const unsigned char * )&_value, sizeof(T) );
        }   

        bool writeSize( size_t _value );

        template<class T>
        void writeCount( const T * _value, size_t _count )
        {
            this->writeBuffer( (const unsigned char * )_value, sizeof(T) * _count );
        }   
        
    protected:
        void writeBuffer( const unsigned char * _buff, size_t _size );

	protected:
		unsigned char * m_out;
        size_t m_size;

		size_t m_write;

		XmlProtocol * m_protocol;

		std::stringstream m_error;
        
        struct SerializationDesc
        {
            ValueSerialization serialization;
            void * user;
        };

        typedef std::map<std::string, SerializationDesc> TMapSerialization;
        TMapSerialization m_serialization;
	};
}