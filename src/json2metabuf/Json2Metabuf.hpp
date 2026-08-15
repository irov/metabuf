#pragma once

#include "metabuf/Metaconvert.hpp"

#include "pugixml.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

struct json_t;

namespace Metabuf
{
    class Json2Metabuf
    {
    public:
        bool convert( const ProtocolInterface * _protocol, const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error ) const;

    protected:
        bool writeObject_( const json_t * _object, const MetaInterface * _meta, const NodeInterface * _schema, pugi::xml_node _node, const std::string & _path, std::string & _error ) const;
        bool writeGroup_( const json_t * _object, const MetaInterface * _meta, const NodeInterface * _schema, pugi::xml_node _node, const std::string & _path, std::string & _error ) const;
        bool writeAttribute_( const json_t * _value, const std::string & _attributeType, pugi::xml_node _node, const char * _name, const std::string & _path, std::string & _error ) const;
        bool stringify_( const json_t * _value, const std::string & _attributeType, std::string & _string, const std::string & _path, std::string & _error ) const;
    };
}
