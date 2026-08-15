#pragma once

#include <stddef.h>
#include <stdint.h>

#include <string>
#include <string_view>

#if !defined(METABUF_EXPORT)
#define METABUF_EXPORT extern "C"
#endif

#define METABUF_BIN_VERSION 10

namespace Metabuf
{
    class NodeInterface;

    class TypeInterface
    {
    public:
        virtual ~TypeInterface() = default;

    public:
        virtual void addEnumerator( const std::string & _name, const std::string & _write, uint32_t _index ) = 0;

    protected:
        TypeInterface() = default;
    };

    class MetaInterface
    {
    public:
        virtual ~MetaInterface() = default;

    public:
        virtual uint32_t getVersion() const = 0;
        virtual const NodeInterface * getNode( std::string_view _name ) const = 0;

    public:
        virtual void addNode( NodeInterface * _node ) = 0;

    protected:
        MetaInterface() = default;
    };

    class NodeInterface
    {
    public:
        virtual ~NodeInterface() = default;

    public:
        virtual const std::string & getName() const = 0;
        virtual const std::string & getGenerator() const = 0;
        virtual const NodeInterface * getScopeNode() const = 0;

        virtual const std::string * getAttributeType( std::string_view _name ) const = 0;
        virtual bool hasMember( std::string_view _name ) const = 0;
        virtual const std::string * getMemberAttributeType( std::string_view _member, std::string_view _name ) const = 0;

        virtual const NodeInterface * getSingle( std::string_view _name ) const = 0;
        virtual const NodeInterface * getInclude( std::string_view _name ) const = 0;
        virtual const NodeInterface * getInheritances( std::string_view _name ) const = 0;
        virtual const NodeInterface * getGenerator( std::string_view _name ) const = 0;

        virtual const std::string * getChildrenType( std::string_view _group ) const = 0;

    public:
        virtual void setInheritance( const NodeInterface * _inheritance ) = 0;
        virtual void addAttribute( uint32_t _id, const std::string & _name, const std::string & _type, bool _required, const std::string & _defaultValue ) = 0;
        virtual void addMemberAttribute( const std::string & _member, uint32_t _id, const std::string & _name, const std::string & _type, bool _required, const std::string & _defaultValue ) = 0;
        virtual void addChildren( const std::string & _name, const std::string & _group, const std::string & _type ) = 0;

        virtual void addSingle( NodeInterface * _single ) = 0;
        virtual void addInclude( NodeInterface * _include ) = 0;
        virtual void addInheritance( NodeInterface * _inheritance ) = 0;
        virtual void addGenerator( NodeInterface * _generator ) = 0;

    protected:
        NodeInterface() = default;
    };

    class ProtocolInterface
    {
    public:
        virtual ~ProtocolInterface() = default;

    public:
        virtual uint32_t getVersion() const = 0;
        virtual uint32_t getCrc32() const = 0;
        virtual const MetaInterface * getMeta( std::string_view _name ) const = 0;

    public:
        virtual void setVersion( uint32_t _version, uint32_t _crc32 ) = 0;

        virtual TypeInterface * addType( const std::string & _name, const std::string & _write, const std::string & _evict, bool _enumerator, bool _ncr, bool _template ) = 0;
        virtual MetaInterface * addMeta( const std::string & _name, uint32_t _version ) = 0;
        virtual NodeInterface * addNode( uint32_t _id, const std::string & _name, const std::string & _generator, const std::string & _inheritance, uint32_t _enumerator, uint32_t _enumeratorNRA, bool _noWrite, const NodeInterface * _scope ) = 0;
        virtual void addInternal( const std::string & _internal ) = 0;
    };

    class ProtocolGenerator
    {
    public:
        virtual ~ProtocolGenerator() = default;

    public:
        virtual void generate( ProtocolInterface * _protocol ) const = 0;
    };

    class OutputAdapter
    {
    public:
        template<class T>
        explicit OutputAdapter( T & _output )
            : m_output( &_output )
            , m_clear( &OutputAdapter::clear_<T> )
            , m_reserve( &OutputAdapter::reserve_<T> )
            , m_append( &OutputAdapter::append_<T> )
        {
        }

    public:
        void clear() const
        {
            (*m_clear)(m_output);
        }

        void reserve( size_t _size ) const
        {
            (*m_reserve)(m_output, _size);
        }

        void append( const uint8_t * _buffer, size_t _size ) const
        {
            if( _size == 0 )
            {
                return;
            }

            (*m_append)(m_output, _buffer, _size);
        }

    protected:
        template<class T>
        static void clear_( void * _output )
        {
            T * output = static_cast<T *>(_output);
            output->clear();
        }

        template<class T>
        static void reserve_( void * _output, size_t _size )
        {
            T * output = static_cast<T *>(_output);
            output->reserve( _size );
        }

        template<class T>
        static void append_( void * _output, const uint8_t * _buffer, size_t _size )
        {
            T * output = static_cast<T *>(_output);
            output->insert( output->end(), _buffer, _buffer + _size );
        }

    protected:
        typedef void( *Clear )(void * _output);
        typedef void( *Reserve )(void * _output, size_t _size);
        typedef void( *Append )(void * _output, const uint8_t * _buffer, size_t _size);

        void * m_output;
        Clear m_clear;
        Reserve m_reserve;
        Append m_append;
    };

    class MetaconvertInterface
    {
    public:
        virtual ~MetaconvertInterface() = default;

    public:
        virtual uint32_t getProtocolVersion() const = 0;
        virtual uint32_t getProtocolCrc32() const = 0;
        virtual const MetaInterface * getMeta( std::string_view _name ) const = 0;
        virtual bool validateProtocol( const void * _buffer, size_t _size, std::string & _error ) const = 0;

    public:
        virtual bool makeHeader( const MetaInterface * _meta, OutputAdapter & _output, std::string & _error ) const = 0;
        virtual bool convert( const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, OutputAdapter & _output, std::string & _error ) const = 0;

    public:
        template<class T>
        bool makeHeader( const MetaInterface * _meta, T & _output, std::string & _error ) const
        {
            OutputAdapter output( _output );

            if( this->makeHeader( _meta, output, _error ) == false )
            {
                return false;
            }

            return true;
        }

        template<class T>
        bool convert( const void * _buffer, size_t _size, const MetaInterface * _meta, const NodeInterface * _node, T & _output, std::string & _error ) const
        {
            OutputAdapter output( _output );

            if( this->convert( _buffer, _size, _meta, _node, output, _error ) == false )
            {
                return false;
            }

            return true;
        }
    };

    METABUF_EXPORT MetaconvertInterface * createXmlMetaconvert( const ProtocolGenerator * _generator );
    METABUF_EXPORT MetaconvertInterface * createJsonMetaconvert( const ProtocolGenerator * _generator );
    METABUF_EXPORT void destroyMetaconvert( MetaconvertInterface * _metaconvert );
}
