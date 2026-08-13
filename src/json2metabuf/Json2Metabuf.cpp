#include "Json2Metabuf.hpp"

#include "xml2metabuf/Xml2Metabuf.hpp"
#include "xml2metabuf/XmlProtocol.hpp"

#include "jansson.h"

#include <cstdio>
#include <sstream>

namespace Metabuf
{
    namespace Detail
    {
        //////////////////////////////////////////////////////////////////////////
        static const XmlNode * resolveGenerator( const XmlNode * _schema, const json_t * _value )
        {
            if( _schema == nullptr || _schema->generator.empty() == true || json_is_object( _value ) == 0 )
            {
                return _schema;
            }

            json_t * generator = json_object_get( _value, _schema->generator.c_str() );

            if( json_is_string( generator ) == 0 || _schema->node_scope == nullptr )
            {
                return _schema;
            }

            const XmlNode * generated = _schema->node_scope->getGenerator( json_string_value( generator ) );

            return generated != nullptr ? generated : _schema;
        }
        //////////////////////////////////////////////////////////////////////////
        static const XmlNode * findChildSchema( const XmlMeta * _meta, const XmlNode * _schema, const char * _name, const json_t * _value )
        {
            const XmlNode * child = _schema->getSingle( _name );

            if( child == nullptr )
            {
                child = _schema->getInclude( _name );
            }

            if( child == nullptr )
            {
                child = _schema->getInheritances( _name );
            }

            if( child == nullptr )
            {
                child = _meta->getNode( _name );
            }

            const XmlNode * generated = resolveGenerator( child, _value );

            return generated;
        }
        //////////////////////////////////////////////////////////////////////////
        static const XmlNode * findGroupSchema( const XmlMeta * _meta, const XmlNode * _schema, const char * _group )
        {
            const XmlNode * hierarchy = _schema;

            while( hierarchy != nullptr )
            {
                for( TMapChildren::const_iterator
                    it = hierarchy->children.begin(),
                    it_end = hierarchy->children.end();
                    it != it_end;
                    ++it )
                {
                    if( it->second.group == _group )
                    {
                        const XmlNode * node = _meta->getNode( it->second.type );

                        return node;
                    }
                }

                hierarchy = hierarchy->node_inheritance;
            }

            return nullptr;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool appendScalar( const json_t * _value, std::string & _string )
        {
            char buffer[64];

            if( json_is_string( _value ) != 0 )
            {
                _string.append( json_string_value( _value ) );

                return true;
            }

            if( json_is_true( _value ) != 0 )
            {
                _string.append( "true" );

                return true;
            }

            if( json_is_false( _value ) != 0 )
            {
                _string.append( "false" );

                return true;
            }

            if( json_is_integer( _value ) != 0 )
            {
                std::snprintf( buffer, sizeof( buffer ), "%lld", static_cast<long long>(json_integer_value( _value )) );
                _string.append( buffer );

                return true;
            }

            if( json_is_real( _value ) != 0 )
            {
                std::snprintf( buffer, sizeof( buffer ), "%.9g", json_real_value( _value ) );
                _string.append( buffer );

                return true;
            }

            return false;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    bool Json2Metabuf::convert( const XmlProtocol * _protocol, const void * _buffer, size_t _size, const char * _meta, const char * _node, OutputAdapter & _output, std::string & _error ) const
    {
        _error.clear();
        _output.clear();

        const XmlMeta * meta = _protocol->getMeta( _meta );

        if( meta == nullptr )
        {
            _error = std::string( "protocol meta not found: " ) + _meta;

            return false;
        }

        const XmlNode * node = meta->getNode( _node );

        if( node == nullptr )
        {
            _error = std::string( "protocol node not found: " ) + _node;

            return false;
        }

        json_error_t json_error;
        json_t * json = json_loadb( static_cast<const char *>(_buffer), _size, JSON_REJECT_DUPLICATES, &json_error );

        if( json == nullptr )
        {
            std::stringstream ss;
            ss << "JSON parse error at " << json_error.line << ':' << json_error.column << ": " << json_error.text;
            _error = ss.str();

            return false;
        }

        if( json_is_object( json ) == 0 )
        {
            _error = "JSON root '$' must be an unwrapped object";
            json_decref( json );

            return false;
        }

        pugi::xml_document document;
        pugi::xml_node root = document.append_child( node->getName().c_str() );
        const bool successful = this->writeObject_( json, meta, node, root, "$", _error );

        json_decref( json );

        if( successful == false )
        {
            return false;
        }

        Xml2Metabuf jsonMetabuf( _protocol, meta );
        jsonMetabuf.initialize();

        if( jsonMetabuf.convert( _output, root, node ) == false )
        {
            _error = jsonMetabuf.getError();

            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Json2Metabuf::writeObject_( const json_t * _object, const XmlMeta * _meta, const XmlNode * _schema, pugi::xml_node _node, const std::string & _path, std::string & _error ) const
    {
        const char * key;
        json_t * value;
        json_object_foreach( const_cast<json_t *>(_object), key, value )
        {
            const std::string path = _path + "." + key;
            const XmlAttribute * attribute = _schema->getAttribute( key );

            if( attribute != nullptr )
            {
                if( this->writeAttribute_( value, attribute, _node, key, path, _error ) == false )
                {
                    return false;
                }

                continue;
            }

            const XmlMember * member = _schema->getMember( key );

            if( member != nullptr )
            {
                if( json_is_object( value ) == 0 )
                {
                    _error = "JSON path '" + path + "' must be an object";

                    return false;
                }

                pugi::xml_node child = _node.append_child( key );
                const char * member_key;
                json_t * member_value;
                json_object_foreach( value, member_key, member_value )
                {
                    const XmlAttribute * member_attribute = member->getAttribute( member_key );

                    if( member_attribute == nullptr )
                    {
                        _error = "JSON path '" + path + "." + member_key + "' is not declared by protocol";

                        return false;
                    }

                    if( this->writeAttribute_( member_value, member_attribute, child, member_key, path + "." + member_key, _error ) == false )
                    {
                        return false;
                    }
                }

                continue;
            }

            const XmlNode * group_schema = Detail::findGroupSchema( _meta, _schema, key );

            if( group_schema != nullptr )
            {
                if( json_is_object( value ) == 0 )
                {
                    _error = "JSON path '" + path + "' must be an object";

                    return false;
                }

                pugi::xml_node group = _node.append_child( key );

                if( this->writeGroup_( value, _meta, group_schema, group, path, _error ) == false )
                {
                    return false;
                }

                continue;
            }

            const json_t * schema_value = value;

            if( json_is_array( value ) != 0 && json_array_size( value ) != 0 )
            {
                schema_value = json_array_get( value, 0 );
            }

            const XmlNode * child_schema = Detail::findChildSchema( _meta, _schema, key, schema_value );

            if( child_schema == nullptr )
            {
                _error = "JSON path '" + path + "' is not declared by protocol";

                return false;
            }

            if( json_is_array( value ) != 0 )
            {
                size_t index;
                json_t * item;
                json_array_foreach( value, index, item )
                {
                    if( json_is_object( item ) == 0 )
                    {
                        std::stringstream ss;
                        ss << "JSON path '" << path << '[' << index << "]' must be an object";
                        _error = ss.str();

                        return false;
                    }

                    const XmlNode * item_schema = Detail::findChildSchema( _meta, _schema, key, item );
                    pugi::xml_node child = _node.append_child( key );

                    if( this->writeObject_( item, _meta, item_schema, child, path + "[]", _error ) == false )
                    {
                        return false;
                    }
                }
            }
            else if( json_is_object( value ) != 0 )
            {
                pugi::xml_node child = _node.append_child( key );

                if( this->writeObject_( value, _meta, child_schema, child, path, _error ) == false )
                {
                    return false;
                }
            }
            else
            {
                _error = "JSON path '" + path + "' must be an object or array";

                return false;
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Json2Metabuf::writeGroup_( const json_t * _object, const XmlMeta * _meta, const XmlNode * _schema, pugi::xml_node _node, const std::string & _path, std::string & _error ) const
    {
        const char * key;
        json_t * value;
        json_object_foreach( const_cast<json_t *>(_object), key, value )
        {
            const std::string path = _path + "." + key;

            if( _schema->getName() != key )
            {
                _error = "JSON path '" + path + "' is not declared by protocol group";

                return false;
            }

            if( json_is_array( value ) != 0 )
            {
                size_t index;
                json_t * item;
                json_array_foreach( value, index, item )
                {
                    if( json_is_object( item ) == 0 )
                    {
                        _error = "JSON path '" + path + "[]' must be an object";

                        return false;
                    }

                    const XmlNode * item_schema = Detail::resolveGenerator( _schema, item );
                    pugi::xml_node child = _node.append_child( key );

                    if( this->writeObject_( item, _meta, item_schema, child, path + "[]", _error ) == false )
                    {
                        return false;
                    }
                }
            }
            else if( json_is_object( value ) != 0 )
            {
                const XmlNode * item_schema = Detail::resolveGenerator( _schema, value );
                pugi::xml_node child = _node.append_child( key );

                if( this->writeObject_( value, _meta, item_schema, child, path, _error ) == false )
                {
                    return false;
                }
            }
            else
            {
                _error = "JSON path '" + path + "' must be an object or array";

                return false;
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Json2Metabuf::writeAttribute_( const json_t * _value, const XmlAttribute * _attribute, pugi::xml_node _node, const char * _name, const std::string & _path, std::string & _error ) const
    {
        std::string string_value;

        if( this->stringify_( _value, _attribute, string_value, _path, _error ) == false )
        {
            return false;
        }

        _node.append_attribute( _name ).set_value( string_value.c_str() );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Json2Metabuf::stringify_( const json_t * _value, const XmlAttribute * _attribute, std::string & _string, const std::string & _path, std::string & _error ) const
    {
        if( json_is_array( _value ) == 0 && json_is_object( _value ) == 0 )
        {
            if( Detail::appendScalar( _value, _string ) == true )
            {
                return true;
            }

            _error = "JSON path '" + _path + "' has unsupported null value";

            return false;
        }

        if( json_is_array( _value ) == 0 || json_array_size( _value ) == 0 )
        {
            _error = "JSON path '" + _path + "' has invalid complex value";

            return false;
        }

        const bool normalize_color = _attribute->type == "Color";

        size_t index;
        json_t * value;
        json_array_foreach( const_cast<json_t *>(_value), index, value )
        {
            if( _string.empty() == false )
            {
                _string.append( " " );
            }

            if( normalize_color == true && json_is_integer( value ) != 0 )
            {
                char buffer[64];
                std::snprintf( buffer, sizeof( buffer ), "%.9g", static_cast<double>(json_integer_value( value )) / 255.0 );
                _string.append( buffer );
            }
            else if( Detail::appendScalar( value, _string ) == false )
            {
                std::stringstream ss;
                ss << "JSON path '" << _path << '[' << index << "]' must be a scalar";
                _error = ss.str();

                return false;
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
