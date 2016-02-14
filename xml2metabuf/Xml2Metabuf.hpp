#	pragma once

//#   include "XmlProtocol.hpp"

#	include <pugixml.hpp>

#	include <vector>
#   include <map>
#	include <string>
#	include <sstream>

#	include <stdint.h>

namespace Metabuf
{
	class XmlProtocol;
	class XmlNode;
	class XmlAttribute;

    typedef bool (*ValueSerialization)( class Xml2Metabuf * _metabuf, const char * _value, void * _user );

	class Xml2Metabuf
	{
	public:
		Xml2Metabuf( XmlProtocol * _protocol );

    public:
        void initialize();
        void addSerializator( const std::string & _type, ValueSerialization _serializator, void * _user );

	public:
        bool header( unsigned char * _binBuff, size_t _binSize, size_t & _writeSize );
		bool convert( unsigned char * _binBuff, size_t _binSize, const void * _xmlBuff, size_t _xmlSize, size_t & _writeSize );
		std::string getError();

	protected:
		bool writeNode_( const XmlNode * _node, const pugi::xml_node & _xml_node );

		bool writeNodeData_(const XmlNode * _node, const pugi::xml_node & _xml_node);

        bool writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool writeNodeAttribute2_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeAttributeSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, uint32_t & _count );

        bool writeNodeIncludes_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool writeNodeGenerators_( const XmlNode * _node, const pugi::xml_node & _xml_node );
        bool getNodeIncludesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, uint32_t & _count );
        bool getNodeGeneratorSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const XmlNode * _inheritance, uint32_t & _count );

		bool writeNodeArguments_( const XmlAttribute * _node, const pugi::xml_attribute & _xml_attr );

    public:
        template<class T>
        void write( const T & _value )
        {
            this->writeBuffer( (const unsigned char * )&_value, sizeof(T) );
        }   

        void writeSize( uint32_t _value );

        template<class T>
        void writeCount( const T * _value, uint32_t _count )
        {
            this->writeBuffer( (const unsigned char * )_value, sizeof(T) * _count );
        }

        void writeString( const char * _value );
        
    protected:
        void writeBuffer( const unsigned char * _buff, size_t _size );

	protected:
		XmlProtocol * m_protocol;

		std::stringstream m_error;
        
        struct SerializationDesc
        {
            ValueSerialization serialization;
            void * user;
        };

        typedef std::map<std::string, SerializationDesc> TMapSerialization;
        TMapSerialization m_serialization;

        typedef std::vector<std::string> TVectorStringCache;
        TVectorStringCache m_stringCache;

        typedef std::vector<unsigned char> TBlobject;
        TBlobject m_buff;
	};
}