#pragma once

#include "metabufconverter/OutputAdapter.hpp"

#include "pugixml.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

struct json_t;

namespace Metabuf
{
    class XmlAttribute;
    class XmlMeta;
    class XmlNode;
    class XmlProtocol;

    class Json2Metabuf
    {
    public:
        bool convert( const XmlProtocol * _protocol, const void * _buffer, size_t _size, const char * _meta, const char * _node, OutputAdapter & _output, std::string & _error ) const;

    protected:
        bool writeObject_( const json_t * _object, const XmlMeta * _meta, const XmlNode * _schema, pugi::xml_node _node, const std::string & _path, std::string & _error ) const;
        bool writeGroup_( const json_t * _object, const XmlMeta * _meta, const XmlNode * _schema, pugi::xml_node _node, const std::string & _path, std::string & _error ) const;
        bool writeAttribute_( const json_t * _value, const XmlAttribute * _attribute, pugi::xml_node _node, const char * _name, const std::string & _path, std::string & _error ) const;
        bool stringify_( const json_t * _value, const XmlAttribute * _attribute, std::string & _string, const std::string & _path, std::string & _error ) const;
    };
}
