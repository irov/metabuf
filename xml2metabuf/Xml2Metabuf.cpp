#	include "Xml2Metabuf.hpp"
#	include "XmlProtocol.hpp"

#   include <algorithm>
#	include <sstream>

#   include <stdio.h>
#   include <string.h>
#   include <memory.h>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    namespace Serialize
    {
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_string( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            _metabuf->writeString( _value );
            
            return true;
        }
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_bool( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			if( strcmp( _value, "true" ) == 0 )
			{
				uint8_t write_value = 1;
				_metabuf->write( write_value );

				return true;
			}
			else if( strcmp( _value, "false" ) == 0 )
			{
				uint8_t write_value = 0;
				_metabuf->write( write_value );

				return true;
			}

			uint32_t int_value;
			if( sscanf( _value, "%u", &int_value ) != 1 )
			{
				return false;
			}

			uint8_t write_value = (uint8_t)int_value;
			_metabuf->write( write_value );

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_int8_t( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			int32_t value;
			if( sscanf( _value, "%d", &value ) != 1 )
			{
				return false;
			}

			int8_t write_value = (int8_t)value;
			_metabuf->write( write_value );

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
        static bool s_write_uint8_t( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

			uint32_t value;
            if( sscanf( _value, "%u", &value ) != 1 )
            {
                return false;
            }

			uint8_t write_value = (uint8_t)value;
            _metabuf->write( write_value );

            return true;
        }    
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_int16_t( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			int32_t value;
			if( sscanf( _value, "%d", &value ) != 1 )
			{
				return false;
			}

			int16_t write_value = (int16_t)value;
			_metabuf->write( write_value );

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_uint16_t( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			uint32_t value;
			if( sscanf( _value, "%u", &value ) != 1 )
			{
				return false;
			}

			uint16_t write_value = (uint16_t)value;
			_metabuf->write( write_value );

			return true;
		}
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_int32_t( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            int32_t value;
            if( sscanf( _value, "%d", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_int32_t2( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            int32_t value0;
            int32_t value1;
            if( sscanf( _value, "%d %d", &value0, &value1 ) != 2 )
            {
                return false;
            }

            _metabuf->write( value0 );
            _metabuf->write( value1 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_uint32_t( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            uint32_t value;
            if( sscanf( _value, "%u", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_uint32_t2( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            uint32_t value0;
            uint32_t value1;
            if( sscanf( _value, "%u %u", &value0, &value1 ) != 2 )
            {
                return false;
            }

            _metabuf->write( value0 );
            _metabuf->write( value1 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            float value;
            if( sscanf( _value, "%f", &value ) != 1 )
            {
                return false;
            }

            _metabuf->write( value );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float2( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            float value[2];
            if( sscanf( _value, "%f;%f", &value[0], &value[1] ) != 2 )
            {
                if( sscanf( _value, "%f %f", &value[0], &value[1] ) != 2 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 2 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float3( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            float value[3];
            if( sscanf( _value, "%f;%f;%f", &value[0], &value[1], &value[2] ) != 3 )
            {
                if( sscanf( _value, "%f %f %f", &value[0], &value[1], &value[2] ) != 3 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 3 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float4( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            float value[4];
            if( sscanf( _value, "%f;%f;%f;%f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
            {
                if( sscanf( _value, "%f %f %f %f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
                {
                    return false;
                }
            }

            _metabuf->writeCount( value, 4 );

            return true;
        }        
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_float4inv255( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            float value[4];
            if( sscanf( _value, "%f;%f;%f;%f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
            {
                if( sscanf( _value, "%f %f %f %f", &value[0], &value[1], &value[2], &value[3] ) != 4 )
                {
                    return false;
                }
            }

            const float inv255 = 1.f / 255.f;

            value[0] *= inv255;
            value[1] *= inv255;
            value[2] *= inv255;
            value[3] *= inv255;

            _metabuf->writeCount( value, 4 );

            return true;
        }        
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_float6( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			float value[6];
			if( sscanf( _value, "%f;%f;%f;%f;%f;%f", &value[0], &value[1], &value[2], &value[3], &value[4], &value[5] ) != 6 )
			{
				if( sscanf( _value, "%f %f %f %f %f %f", &value[0], &value[1], &value[2], &value[3], &value[4], &value[5] ) != 6 )
				{
					return false;
				}
			}

			_metabuf->writeCount( value, 6 );

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_float8( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			float value[8];
			if( sscanf( _value, "%f;%f;%f;%f;%f;%f;%f;%f", &value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6], &value[7] ) != 8 )
			{
				if( sscanf( _value, "%f %f %f %f %f %f %f %f", &value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6], &value[7] ) != 8 )
				{
					return false;
				}
			}

			_metabuf->writeCount( value, 8 );

			return true;
		}		
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_float12( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			float value[12];
			if( sscanf( _value, "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f"
				, &value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6], &value[7]
				, &value[8], &value[9], &value[10], &value[11]
			) != 12 )
			{
				if( sscanf( _value, "%f %f %f %f %f %f %f %f %f %f %f %f"
					, &value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6], &value[7]
					, &value[8], &value[9], &value[10], &value[11]
				) != 12 )
				{
					return false;
				}
			}

			_metabuf->writeCount( value, 12 );

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_float16( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			(void)_user;

			float value[16];
			if( sscanf( _value, "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f"
				, &value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6], &value[7]
			, &value[8], &value[9], &value[10], &value[11], &value[12], &value[13], &value[14], &value[15]
			) != 16 )
			{
				if( sscanf( _value, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
					, &value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6], &value[7]
				, &value[8], &value[9], &value[10], &value[11], &value[12], &value[13], &value[14], &value[15]
				) != 16 )
				{
					return false;
				}
			}

			_metabuf->writeCount( value, 16 );

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		template<class T>
		static bool s_write_pods( Xml2Metabuf * _metabuf, const char * _format, const char * _value, void * _user )
		{
			(void)_user;

			typedef std::vector<T> TVectorPods;
			TVectorPods pods;

			size_t len = strlen( _value );
            
            typedef std::vector<char> TVectorParseValue; 
            TVectorParseValue parse_value;

            parse_value.resize( len + 1 );

            char * parse_value_buffer = &parse_value[0];

			strcpy( parse_value_buffer, _value );

			char * pch = strtok( parse_value_buffer, " " );

			while( pch != nullptr )
			{
				T value;
				if( sscanf( pch, _format, &value ) != 1 )
				{
					return false;
				}

				pods.push_back( value );

				pch = strtok( nullptr, " " );
			}

			uint32_t count = (uint32_t)pods.size();

			_metabuf->writeSize( count );

			if( count > 0 )
			{
				_metabuf->writeCount( &pods[0], count );
			}

			return true;
		}
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_floats( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
			bool successful = s_write_pods<float>( _metabuf, "%f", _value, _user );

            return successful;
        }
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_int8s( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			bool successful = s_write_pods<int8_t>( _metabuf, "%d", _value, _user );

			return successful;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_int16s( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			bool successful = s_write_pods<int16_t>( _metabuf, "%d", _value, _user );

			return successful;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_int32s( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			bool successful = s_write_pods<int32_t>( _metabuf, "%d", _value, _user );

			return successful;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_uint8s( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			bool successful = s_write_pods<uint8_t>( _metabuf, "%u", _value, _user );

			return successful;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_uint16s( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			bool successful = s_write_pods<uint16_t>( _metabuf, "%u", _value, _user );

			return successful;
		}
		//////////////////////////////////////////////////////////////////////////
		static bool s_write_uint32s( Xml2Metabuf * _metabuf, const char * _value, void * _user )
		{
			bool successful = s_write_pods<uint32_t>( _metabuf, "%u", _value, _user );

			return successful;
		}
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_sha1bin( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            uint32_t hashmask[] = { 0, 0, 0, 0, 0 };

            for( uint32_t i = 0; i != 40; ++i ) 
            {
                uint32_t j = (i / 8);
                uint32_t k = i % 8;

                char hash_char = _value[i];
                char char_code_0 = '0';
                char char_code_9 = '9';
                char char_code_a = 'a';

                uint32_t v = (hash_char > char_code_9) ? hash_char - char_code_a + 10 : hash_char - char_code_0;

                hashmask[j] += v << (k * 4);
            }

            _metabuf->writeCount( hashmask, 5 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_hexadecimal( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;
            size_t len = strlen( _value );

            if( len % 2 != 0 )
            {
                return false;
            }

            uint32_t hexadecimal_len = len / 2;

            _metabuf->writeSize( hexadecimal_len );

            if( hexadecimal_len == 0 )
            {
                return true;
            }           

            typedef std::vector<uint8_t> TVectorHexadecimal;
            TVectorHexadecimal hexadecimal;
            hexadecimal.reserve( hexadecimal_len );

            for( size_t i = 0; i != len; i += 2 )
            {
                char a0 = _value[i + 0];
                char a1 = _value[i + 1];
                
                uint8_t h0 = (a0 >= 'a') ? (10 + (a0 - 'a')) : (a0 - '0');
                uint8_t h1 = (a1 >= 'a') ? (10 + (a1 - 'a')) : (a1 - '0');

                uint8_t h2 = h0 << 4;

                uint8_t h = h1 + h2;
                
                hexadecimal.push_back( h );
            }

            _metabuf->writeCount( &hexadecimal[0], hexadecimal_len );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_angle360( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            float value;
            if( sscanf( _value, "%f", &value ) != 1 )
            {
                return false;
            }

            double d2_angle360 = double( value ) * 3.141592653589793238462643383279502884197169399375105820974944 / 180.0;
            float angle360 = (float)d2_angle360;

            _metabuf->write( angle360 );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        static bool s_write_opacity255( Xml2Metabuf * _metabuf, const char * _value, void * _user )
        {
            (void)_user;

            uint32_t value;
            if( sscanf( _value, "%u", &value ) != 1 )
            {
                return false;
            }

            double d2_opacity255 = double( value ) / 255.0;
            float opacity255 = (float)d2_opacity255;

            _metabuf->write( opacity255 );

            return true;
        }        
    }
    //////////////////////////////////////////////////////////////////////////
    static int64_t makeHash( const void * _data, size_t _len )
    {
        if( _len == 0 )
        {
            return 0;
        }

        const uint8_t * p = (const uint8_t *)_data;

		int64_t x = *p << 7;

        for( size_t i = 0; i != _len; ++i )
        {
            x = (1000003 * x) ^ *p++;
        }

		x ^= (int64_t)_len;

        if( x == -1 )
        {
            x = -2;
        }

        return x;
    }
	//////////////////////////////////////////////////////////////////////////
	Xml2Metabuf::Xml2Metabuf( const XmlProtocol * _protocol, const XmlMeta * _meta )
        : m_protocol( _protocol )
        , m_meta( _meta )
        , m_hashable( nullptr )
	{
	}
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::initialize()
    {
        this->addSerializator( "string", &Serialize::s_write_string, nullptr );
        
		this->addSerializator( "bool", &Serialize::s_write_bool, nullptr );
		this->addSerializator( "int8_t", &Serialize::s_write_int8_t, nullptr );
        this->addSerializator( "uint8_t", &Serialize::s_write_uint8_t, nullptr );
		this->addSerializator( "int16_t", &Serialize::s_write_int16_t, nullptr );
		this->addSerializator( "uint16_t", &Serialize::s_write_uint16_t, nullptr );

        this->addSerializator( "int32_t", &Serialize::s_write_int32_t, nullptr );
        this->addSerializator( "int32_t2", &Serialize::s_write_int32_t2, nullptr );

        this->addSerializator( "uint32_t", &Serialize::s_write_uint32_t, nullptr );
        this->addSerializator( "uint32_t2", &Serialize::s_write_uint32_t2, nullptr );
        
        this->addSerializator( "float", &Serialize::s_write_float, nullptr );
        this->addSerializator( "float2", &Serialize::s_write_float2, nullptr );
        this->addSerializator( "float3", &Serialize::s_write_float3, nullptr );
        this->addSerializator( "float4", &Serialize::s_write_float4, nullptr );
        this->addSerializator( "float4inv255", &Serialize::s_write_float4inv255, nullptr );
		this->addSerializator( "float6", &Serialize::s_write_float6, nullptr );
		this->addSerializator( "float8", &Serialize::s_write_float8, nullptr );
		this->addSerializator( "float12", &Serialize::s_write_float12, nullptr );
		this->addSerializator( "float16", &Serialize::s_write_float16, nullptr );
        this->addSerializator( "floats", &Serialize::s_write_floats, nullptr );
		this->addSerializator( "int8s", &Serialize::s_write_int8s, nullptr );
		this->addSerializator( "int16s", &Serialize::s_write_int16s, nullptr );
		this->addSerializator( "int32s", &Serialize::s_write_int32s, nullptr );
		this->addSerializator( "uint8s", &Serialize::s_write_uint8s, nullptr );
		this->addSerializator( "uint16s", &Serialize::s_write_uint16s, nullptr );
		this->addSerializator( "uint32s", &Serialize::s_write_uint32s, nullptr );
        this->addSerializator( "sha1bin", &Serialize::s_write_sha1bin, nullptr );
        
        
        this->addSerializator( "hexadecimal", &Serialize::s_write_hexadecimal, nullptr );
        this->addSerializator( "angle360", &Serialize::s_write_angle360, nullptr );
        this->addSerializator( "opacity255", &Serialize::s_write_opacity255, nullptr );

		m_hashable = &makeHash;
    }
	//////////////////////////////////////////////////////////////////////////
	void Xml2Metabuf::setHashable( MakeHash _hashable )
	{
		m_hashable = _hashable;
	}
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::addSerializator( const std::string & _type, ValueSerialization _serializator, void * _user )
    {
        SerializationDesc desc;
        desc.serialization = _serializator;
        desc.user = _user;

        m_serialization[_type] = desc;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::header( uint8_t * _binBuff, size_t _binSize, uint32_t _metaVersion, size_t & _writeSize )
    {
        m_buff.clear();

        size_t writeSize = 0;

        uint32_t magic = 3133062829u;
        this->write( magic );

        uint32_t version = METABUF_BIN_VERSION;
        this->write( version );

        uint32_t protocol = m_protocol->getVersion();
        this->write( protocol );

        this->write( _metaVersion );
        
        writeSize += m_buff.size();

        if( writeSize > _binSize )
        {
            m_error << "Xml2Metabuf::header write size " << writeSize << " > binSize " << _binSize << std::endl;

            return false;
        }

        std::copy( m_buff.begin(), m_buff.end(), _binBuff );
        
        _writeSize = writeSize;

        m_buff.clear();

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::convert( uint8_t * _binBuff, size_t _binSize, const void * _xmlBuff, size_t _xmlSize, size_t & _writeSize )
	{	
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_buffer( _xmlBuff, _xmlSize );

		if( result == false )
		{
            m_error << "Xml2Metabuf::convert xml parser error:" << std::endl << result.description() << std::endl;

			return false;
		}

        size_t writeSize = 0;

        m_buff.clear();

        pugi::xml_node root = doc.document_element();

        const char * root_name = root.name();

        const XmlNode * node_root = m_meta->getNode( root_name );

        if( node_root == nullptr )
        {
            m_error << "Xml2Metabuf::convert: invalid root node " << root_name << std::endl;

            return false;
        }

		uint32_t id = node_root->id;
		this->writeSize( id );

		if( this->writeNode_( node_root, root ) == false )
		{
            m_error << "Xml2Metabuf::convert: invalid write node " << root_name << std::endl;

			return false;
		}

        TBlobject buffBody;
        buffBody.swap( m_buff );        
        m_buff.clear();

        uint32_t stringCacheCount = (uint32_t)m_stringCache.size();
        this->write( stringCacheCount );
        
        for( TVectorStringCache::iterator
            it = m_stringCache.begin(),
            it_end = m_stringCache.end();
        it != it_end;
        ++it )
        {
            const std::string & str = *it;

            uint32_t strSize = (uint32_t)str.size();
            this->writeSize( strSize );

            const char * strBuff = str.c_str();

			int64_t hash = makeHash( strBuff, strSize );
            this->write( hash );

            this->writeCount( strBuff, strSize );
        }

        TBlobject buffStrCache;
        buffStrCache.swap( m_buff );
        m_buff.clear();

        TBlobject buffFinal;

        buffFinal.insert( buffFinal.end(), buffStrCache.begin(), buffStrCache.end() );
        buffFinal.insert( buffFinal.end(), buffBody.begin(), buffBody.end() );

        writeSize += buffFinal.size();

        if( writeSize > _binSize )
        {
            m_error << "Xml2Metabuf::convert: write buffer not enouge memory " << _binSize << " need " << writeSize << std::endl;

            return false;
        }

        _writeSize = writeSize;
        std::copy( buffFinal.begin(), buffFinal.end(), _binBuff );        

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	std::string Xml2Metabuf::getError() const
	{
		return m_error.str();
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNode_( const XmlNode * _node, const pugi::xml_node & _xml_node )
	{
        if( _xml_node.begin() == _xml_node.end() && _xml_node.attributes_begin() == _xml_node.attributes_end() )
        {
            return true;
        }

		if( this->writeNodeData_( _node, _xml_node ) == false )
		{
			m_error << "Xml2Metabuf::writeNodeData_: error write node " << _node->name << " attribute" << std::endl;

			return false;
		}
		
        if( this->writeNodeAttribute_( _node, _xml_node ) == false )
        {
            m_error << "Xml2Metabuf::writeNodeAttribute_: error write node " << _node->name << " attribute" << std::endl;

            return false;
        }

		if( this->writeNodeSingles_( _node, _xml_node ) == false )
		{
			m_error << "Xml2Metabuf::writeNodeSingles_: error write node " << _node->name << " includes" << std::endl;

			return false;
		}

        if( this->writeNodeIncludes_( _node, _xml_node ) == false )
        {
            m_error << "Xml2Metabuf::writeNodeIncludes_: error write node " << _node->name << " includes" << std::endl;

            return false;
        }

		if( this->writeNodeChildren_( _node, _xml_node ) == false )
		{
			m_error << "Xml2Metabuf::writeNodeChildren_: error write node " << _node->name << " includes" << std::endl;

			return false;
		}

        if( this->writeNodeGenerators_( _node, _xml_node ) == false )
        {
            m_error << "Xml2Metabuf::writeNodeGenerators_: error write node " << _node->name << " generators" << std::endl;

            return false;
        }

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeData_( const XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		if( _node->inheritance.empty() == false )
		{
			if( this->writeNodeData2_( _node->node_inheritance, _xml_node ) == false )
			{
				return false;
			}
		}

		if( this->writeNodeData2_( _node, _xml_node ) == false )
		{
			return false;
		}

		for( TMapMembers::const_iterator
			it = _node->members.begin(),
			it_end = _node->members.end();
		it != it_end;
		++it )
		{
			const XmlMember * member = &it->second;

			for( TMapAttributes::const_iterator
				it_attributes = member->attributes.begin(),
				it_attributes_end = member->attributes.end();
			it_attributes != it_attributes_end;
			++it_attributes )
			{
				const XmlAttribute * attr = &it_attributes->second;

				if( attr->required == false )
				{
					continue;
				}

				bool member_found = false;

				for( pugi::xml_node::iterator
					it_xml = _xml_node.begin(),
					it_xml_end = _xml_node.end();
				it_xml != it_xml_end;
				++it_xml )
				{
					const pugi::xml_node & child = *it_xml;

					const char * child_name = child.name();

					if( member->name != child_name )
					{
						continue;
					}

					pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

					if( xml_attr.empty() == true )
					{
						m_error << "Xml2Metabuf::writeNodeData_: '" << _node->name << "' not found required member '" << member->name << "' argument '" << attr->name << "'" << std::endl;

						return false;
					}

					if( this->writeNodeDataValue_( attr, xml_attr ) == false )
					{
						m_error << "Xml2Metabuf::writeNodeData_: '" << _node->name << "' not write member '" << member->name << "' argument '" << attr->name << "'" << std::endl;

						return false;
					}

					member_found = true;
					break;
				}

				if( member_found == false )
				{
					m_error << "Xml2Metabuf::writeNodeData_: '" << _node->name << "' member '" << member->name << "' not found required argument '" << attr->name << "'" << std::endl;

					return false;
				}

			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeData2_( const XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		for( TMapAttributes::const_iterator
			it = _node->attributes.begin(),
			it_end = _node->attributes.end();
		it != it_end;
		++it )
		{
			const XmlAttribute * attr = &it->second;

			if( attr->required == false )
			{
				continue;
			}

			pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

			if( xml_attr.empty() == true )
			{
				m_error << "Xml2Metabuf::writeNodeData2_: '" << _node->name << "' not found required argument '" << attr->name << "'" << std::endl;

				return false;
			}

			if( this->writeNodeDataValue_( attr, xml_attr ) == false )
			{
				m_error << "Xml2Metabuf::writeNodeData2_: '" << _node->name << "' not write argument '" << attr->name << "'" << std::endl;

				return false;
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::getNodeDataSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, uint32_t & _count ) const
	{
		uint32_t count = 0;

		for( TMapAttributes::const_iterator
			it = _node->attributes.begin(),
			it_end = _node->attributes.end();
		it != it_end;
		++it )
		{
			const XmlAttribute * attr = &it->second;

			if( attr->required == false )
			{
				continue;
			}

			pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

			if( xml_attr.empty() == true )
			{
				m_error << "Xml2Metabuf::getNodeDataSize_: node '" << _node->name << "' not found attribute '" << attr->name << "'" << std::endl;

				return false;
			}

			++count;
		}

		if( _node->inheritance.empty() == false )
		{
			for( TMapAttributes::const_iterator
				it = _node->node_inheritance->attributes.begin(),
				it_end = _node->node_inheritance->attributes.end();
			it != it_end;
			++it )
			{
				const XmlAttribute * attr = &it->second;

				if( attr->required == false )
				{
					continue;
				}

				pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

				if( xml_attr.empty() == true )
				{
					m_error << "Xml2Metabuf::getNodeDataSize_: node '" << _node->name << "' not found attribute '" << attr->name << "'" << std::endl;

					return false;
				}

				++count;
			}
		}

		for( TMapMembers::const_iterator
			it = _node->members.begin(),
			it_end = _node->members.end();
		it != it_end;
		++it )
		{
			const XmlMember * member = &it->second;

			for( TMapAttributes::const_iterator
				it_attributes = member->attributes.begin(),
				it_attributes_end = member->attributes.end();
			it_attributes != it_attributes_end;
			++it_attributes )
			{
				const XmlAttribute * attr = &it_attributes->second;

				if( attr->required == false )
				{
					continue;
				}

				bool member_found = false;

				for( pugi::xml_node::iterator
					it_xml = _xml_node.begin(),
					it_xml_end = _xml_node.end();
				it_xml != it_xml_end;
				++it_xml )
				{
					const pugi::xml_node & child = *it_xml;

					const char * child_name = child.name();

					if( member->name != child_name )
					{
						continue;
					}

					pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

					if( xml_attr.empty() == true )
					{
						continue;
					}

					++count;

					member_found = true;

					break;
				}

				if( member_found == false )
				{
					m_error << "Xml2Metabuf::getNodeDataSize_: '" << _node->name << "' member '" << member->name << "' not found required argument '" << attr->name << "'" << std::endl;

					return false;
				}
			}
		}

		_count = count;

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeAttribute_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        uint32_t attributeCount;
        if( this->getNodeAttributeSize_( _node, _xml_node, attributeCount ) == false )
        {
            return false;
        }

        this->writeSize( attributeCount );

        if( _node->inheritance.empty() == false )
        {
            if( this->writeNodeAttribute2_( _node->node_inheritance, _xml_node ) == false )
            {
                return false;
            }
        }

        if( this->writeNodeAttribute2_( _node, _xml_node ) == false )
        {
            return false;
        }

        for( TMapMembers::const_iterator
            it = _node->members.begin(),
            it_end = _node->members.end();
        it != it_end;
        ++it )
        {
            const XmlMember * member = &it->second;
            
            for( TMapAttributes::const_iterator
                it_attributes = member->attributes.begin(),
                it_attributes_end = member->attributes.end();
            it_attributes != it_attributes_end;
            ++it_attributes )
            {
                const XmlAttribute * attr = &it_attributes->second;

				if( attr->required == true )
				{
					continue;
				}

                for( pugi::xml_node::iterator
                    it_xml = _xml_node.begin(),
                    it_xml_end = _xml_node.end();
                it_xml != it_xml_end;
                ++it_xml )
                {
                    const pugi::xml_node & child = *it_xml;

                    const char * child_name = child.name();

                    if( member->name != child_name )
                    {
                        continue;
                    }
                    
                    pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

                    if( xml_attr.empty() == true )
                    {
                        continue;
                    }

                    if( this->writeNodeArgumentValue_( attr, xml_attr ) == false )
                    {
                        m_error << "Xml2Metabuf::writeNodeAttribute_: '" << _node->name << "' not write member '" << member->name << "' argument '" << attr->name << "'" << std::endl;

                        return false;
                    }

                    break;
                }
            }
        }

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeAttribute2_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        for( TMapAttributes::const_iterator
            it = _node->attributes.begin(),
            it_end = _node->attributes.end();
        it != it_end;
        ++it )
        {
            const XmlAttribute * attr = &it->second;

			if( attr->required == true )
			{
				continue;
			}

            pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

			if( xml_attr.empty() == true )
			{
				continue;
			}

            if( this->writeNodeArgumentValue_( attr, xml_attr ) == false )
            {
                m_error << "Xml2Metabuf::writeNodeAttribute_: '" << _node->name << "' not write argument '" << attr->name << "'" << std::endl;

                return false;
            }
        }

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	static bool s_getTypeEnumeratorIndex( const XmlType & _type, const char * _value, uint32_t & _index )
	{
        uint32_t enum_index = 0U;

		for( XmlType::TVectorEnumerators::const_iterator
			it = _type.enumerators.begin(),
			it_end = _type.enumerators.end();
		it != it_end;
		++it )
		{
			const XmlEnum & enumerat = *it;

			if( enumerat.name != _value )
			{
                ++enum_index;

				continue;
			}

            _index = enum_index;

			return true;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeDataValue_( const XmlAttribute * _attr, const pugi::xml_attribute & _xml_attr )
	{
		XmlType type;
		if( m_protocol->getType( _attr->type, type ) == false )
		{
			m_error << "Xml2Metabuf::writeNodeDataValue_: not found attribute '" << _attr->name << "' type '" << type.write << "'" << std::endl;

			return false;
		}

		TMapSerialization::const_iterator it_serialize = m_serialization.find( type.evict );

		if( it_serialize == m_serialization.end() )
		{
			m_error << "Xml2Metabuf::writeNodeDataValue_: not found serialize '" << type.evict << "' for attribute '" << _attr->name << "' type '" << type.write << "'" << std::endl;

			return false;
		}

		const SerializationDesc & desc = it_serialize->second;

		const char * attr_value = _xml_attr.value();

		if( type.is_enumerator == true )
		{
			uint32_t index;
			if( s_getTypeEnumeratorIndex( type, attr_value, index ) == false )
			{
				m_error << "Xml2Metabuf::writeNodeData_: not found enumerate '" << attr_value << "' for attribute '" << _attr->name << "' type '" << type.write << "'" << std::endl;

				return false;
			}

			char enumerator_attr_value[16];
			sprintf( enumerator_attr_value, "%d", index );

			if( (*desc.serialization)(this, enumerator_attr_value, desc.user) == false )
			{
				m_error << "Xml2Metabuf::writeNodeData_: serialize '" << type.evict << "' for attribute '" << _attr->name << "' error for value '" << attr_value << "' [enum]" << std::endl;

				return false;
			}
		}
		else
		{
			if( (*desc.serialization)(this, attr_value, desc.user) == false )
			{
				m_error << "Xml2Metabuf::writeNodeData_: serialize '" << type.evict << "' for attribute '" << _attr->name << "' error for value '" << attr_value << "'" << std::endl;

				return false;
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeArgumentValue_( const XmlAttribute * _attr, const pugi::xml_attribute & _xml_attr )
	{
		uint32_t id = _attr->id;
		this->writeSize( id );

		if( this->writeNodeDataValue_( _attr, _xml_attr ) == false )
		{
			m_error << "Xml2Metabuf::writeNodeArguments_: invalid write data '" << _attr->name << "' type '" << _attr->type << "'" << std::endl;

			return false;
		}
		
        return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::getNodeAttributeSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, uint32_t & _count ) const
    {    
        uint32_t count = 0;

        for( TMapAttributes::const_iterator
            it = _node->attributes.begin(),
            it_end = _node->attributes.end();
        it != it_end;
        ++it )
        {
            const XmlAttribute * attr = &it->second;

			if( attr->required == true )
			{
				continue;
			}

            pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

			if( xml_attr.empty() == true )
			{
				continue;
			}

            ++count;
        }

        if( _node->inheritance.empty() == false )
        {
            for( TMapAttributes::const_iterator
                it = _node->node_inheritance->attributes.begin(),
                it_end = _node->node_inheritance->attributes.end();
            it != it_end;
            ++it )
            {
                const XmlAttribute * attr = &it->second;

				if( attr->required == true )
				{
					continue;
				}

                pugi::xml_attribute xml_attr = _xml_node.attribute( attr->name.c_str() );

				if( xml_attr.empty() == true )
				{
					continue;
				}

                ++count;
            }
        }

        for( TMapMembers::const_iterator
            it = _node->members.begin(),
            it_end = _node->members.end();
        it != it_end;
        ++it )
        {
            const XmlMember * member = &it->second;

            for( TMapAttributes::const_iterator
                it_attributes = member->attributes.begin(),
                it_attributes_end = member->attributes.end();
            it_attributes != it_attributes_end;
            ++it_attributes )
            {
                const XmlAttribute * attr = &it_attributes->second;

				if( attr->required == true )
				{
					continue;
				}

                for( pugi::xml_node::iterator
                    it_xml = _xml_node.begin(),
                    it_xml_end = _xml_node.end();
                it_xml != it_xml_end;
                ++it_xml )
                {
                    const pugi::xml_node & child = *it_xml;

                    const char * child_name = child.name();

                    if( member->name != child_name )
                    {
                        continue;
                    }

                    pugi::xml_attribute xml_attr = child.attribute( attr->name.c_str() );

                    if( xml_attr.empty() == true )
                    {
                        continue;
                    }

                    ++count;
                    
                    break;
                }
            }
        }

        _count = count;

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeSingles_( const XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		uint32_t typeCount = 0;

		for( TMapNodes::const_iterator
			it = _node->singles.begin(),
			it_end = _node->singles.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			uint32_t count;
			this->getNodeSinglesSize_( _node, _xml_node, node->name, count );

			if( count == 0 )
			{
				continue;
			}

			if( count > 1 )
			{
				m_error << "Xml2Metabuf::writeNodeSingles_: error write node '" << _node->name << "' singles '" << node->name << "' have more one instance" << std::endl;

				return false;
			}

			++typeCount;
		}

		this->writeSize( typeCount );

		for( TMapNodes::const_iterator
			it = _node->singles.begin(),
			it_end = _node->singles.end();
			it != it_end;
			++it )
		{
			const XmlNode * node = it->second;

			uint32_t count;
			this->getNodeSinglesSize_( _node, _xml_node, node->name, count );

			if( count == 0 )
			{
				continue;
			}

			this->writeSize( node->id );

			for( pugi::xml_node::iterator
				it_xml = _xml_node.begin(),
				it_xml_end = _xml_node.end();
				it_xml != it_xml_end;
				++it_xml )
			{
				const pugi::xml_node & child = *it_xml;

				const char * child_name = child.name();

				if( node->name != child_name )
				{
					continue;
				}

				if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
				{
					continue;
				}

				if( this->writeNode_( node, child ) == false )
				{
					m_error << "Xml2Metabuf::writeNodeSingles_: error write node '" << _node->name << "' childrens '" << node->name << "'" << std::endl;

					return false;
				}
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::getNodeSinglesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, uint32_t & _count ) const
	{
		uint32_t count = 0;

		for( pugi::xml_node::iterator
			it_xml = _xml_node.begin(),
			it_xml_end = _xml_node.end();
			it_xml != it_xml_end;
			++it_xml )
		{
			const pugi::xml_node & child = *it_xml;

			const char * child_name = child.name();

			if( _type != child_name )
			{
				continue;
			}

			if( _node->getSingle( child_name ) == nullptr )
			{
				continue;
			}

			if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
			{
				continue;
			}

			++count;
		}

		_count = count;

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::writeNodeIncludes_( const XmlNode * _node, const pugi::xml_node & _xml_node )
    {
		uint32_t includesTypeCount = 0;

		for( TMapNodes::const_iterator
			it = _node->includes.begin(),
			it_end = _node->includes.end();
		it != it_end;
		++it )
		{
			const XmlNode * node_include = it->second;

			uint32_t includeCount;
			this->getNodeIncludesSize_( _node, _xml_node, node_include->name, includeCount );

			if( includeCount == 0 )
			{
				continue;
			}

			++includesTypeCount;
		}

		this->writeSize( includesTypeCount );

        for( TMapNodes::const_iterator
            it = _node->includes.begin(),
            it_end = _node->includes.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node_include = it->second;
            
            uint32_t incluidesCount;
            this->getNodeIncludesSize_( _node, _xml_node, node_include->name, incluidesCount );

			if( incluidesCount == 0 )
			{
				continue;
			}

			this->writeSize( incluidesCount );

            this->writeSize( node_include->id );

            for( pugi::xml_node::iterator
                it_xml = _xml_node.begin(),
                it_xml_end = _xml_node.end();
            it_xml != it_xml_end;
            ++it_xml )
            {
                const pugi::xml_node & child = *it_xml;

                const char * child_name = child.name();

                if( node_include->name != child_name )
                {
                    continue;
                }

                if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
                {
                    continue;
                }
                
                if( this->writeNode_( node_include, child ) == false )
                {
                    m_error << "Xml2Metabuf::writeNodeIncludes_: error write node '" << _node->name << "' includes '" << node_include->name << "'" << std::endl;

                    return false;
                }
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::getNodeIncludesSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _type, uint32_t & _count ) const
    {
        uint32_t count = 0;

        for( pugi::xml_node::iterator
            it_xml = _xml_node.begin(),
            it_xml_end = _xml_node.end();
        it_xml != it_xml_end;
        ++it_xml )
        {
            const pugi::xml_node & child = *it_xml;

            const char * child_name = child.name();

            if( _type != child_name )
            {
                continue;
            }

            if( _node->getInclude( child_name ) == nullptr )
            {
                continue;
            }

            if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
            {
                continue;
            }
            
            ++count;
        }

        _count = count;

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeChildren_( const XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		uint32_t childrenTypeCount = 0;
		
		for( TMapChildren::const_iterator
			it = _node->children.begin(),
			it_end = _node->children.end();
			it != it_end;
			++it )
		{
			const XmlChildren & children = it->second;

			uint32_t includeCount;
			this->getNodeChildrenSize_( _node, _xml_node, children.group, children.type, includeCount );

			if( includeCount == 0 )
			{
				continue;
			}

			++childrenTypeCount;
		}
				
		if( _node->node_inheritance != nullptr )
		{
			for( TMapChildren::const_iterator
				it = _node->node_inheritance->children.begin(),
				it_end = _node->node_inheritance->children.end();
				it != it_end;
				++it )
			{
				const XmlChildren & children = it->second;

				uint32_t includeCount;
				this->getNodeChildrenSize_( _node->node_inheritance, _xml_node, children.group, children.type, includeCount );

				if( includeCount == 0 )
				{
					continue;
				}

				++childrenTypeCount;
			}
		}

		this->writeSize( childrenTypeCount );

		for( TMapChildren::const_iterator
			it = _node->children.begin(),
			it_end = _node->children.end();
			it != it_end;
			++it )
		{
			const XmlChildren & children = it->second;

			const XmlNode * node_children = m_meta->getNode( children.type );

			uint32_t childrenCount;
			this->getNodeChildrenSize_( _node, _xml_node, children.group, children.type, childrenCount );

			if( childrenCount == 0 )
			{
				continue;
			}

			this->writeSize( childrenCount );

			this->writeSize( node_children->id );

			const char * group_value = children.group.c_str();

			pugi::xml_node group_child = _xml_node.child( group_value );

			for( pugi::xml_node::iterator
				it_xml = group_child.begin(),
				it_xml_end = group_child.end();
				it_xml != it_xml_end;
				++it_xml )
			{
				const pugi::xml_node & child = *it_xml;

				const char * child_name = child.name();

				if( node_children->name != child_name )
				{
					continue;
				}

				if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
				{
					continue;
				}

				pugi::xml_attribute attr_generator = child.attribute( node_children->generator.c_str() );

				const char * value_generator = attr_generator.value();

				const XmlNode * node_generator = node_children->node_scope->getGenerator( value_generator );

				if( node_generator == nullptr )
				{
					m_error << "Xml2Metabuf::writeNodeChildren_: error write node '" << _node->name << "' includes '" << node_children->node_scope->name << "' not found generator '" << value_generator << "'" << std::endl;

					return false;
				}

				if( node_generator->getNoWrite() == true )
				{
					continue;
				}

				if( this->writeNode_( node_generator, child ) == false )
				{
					m_error << "Xml2Metabuf::writeNodeChildren_: error write node '" << _node->name << "' children '" << node_generator->name << "'" << std::endl;

					return false;
				}
			}
		}

		if( _node->node_inheritance != nullptr )
		{
			for( TMapChildren::const_iterator
				it = _node->node_inheritance->children.begin(),
				it_end = _node->node_inheritance->children.end();
				it != it_end;
				++it )
			{
				const XmlChildren & children = it->second;

				const XmlNode * node_children = m_meta->getNode( children.type );

				uint32_t childrenCount;
				this->getNodeChildrenSize_( _node->node_inheritance, _xml_node, children.group, children.type, childrenCount );

				if( childrenCount == 0 )
				{
					continue;
				}

				this->writeSize( childrenCount );

				this->writeSize( node_children->id );

				const char * group_value = children.group.c_str();

				pugi::xml_node group_child = _xml_node.child( group_value );

				for( pugi::xml_node::iterator
					it_xml = group_child.begin(),
					it_xml_end = group_child.end();
					it_xml != it_xml_end;
					++it_xml )
				{
					const pugi::xml_node & child = *it_xml;

					const char * child_name = child.name();

					if( node_children->name != child_name )
					{
						continue;
					}

					if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
					{
						continue;
					}

					pugi::xml_attribute attr_generator = child.attribute( node_children->generator.c_str() );

					const char * value_generator = attr_generator.value();

					const XmlNode * node_generator = node_children->node_scope->getGenerator( value_generator );

					if( node_generator == nullptr )
					{
						m_error << "Xml2Metabuf::writeNodeChildren_: error write node '" << _node->name << "' includes '" << node_children->node_scope->name << "' not found generator '" << value_generator << "'" << std::endl;

						return false;
					}

					if( node_generator->getNoWrite() == true )
					{
						continue;
					}

					this->writeSize( node_generator->id );

					if( this->writeNode_( node_generator, child ) == false )
					{
						m_error << "Xml2Metabuf::writeNodeChildren_: error write node '" << _node->name << "' children '" << node_generator->name << "'" << std::endl;

						return false;
					}
				}
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::getNodeChildrenSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const std::string & _group, const std::string & _type, uint32_t & _count ) const
	{
		(void)_node;

		const char * group_value = _group.c_str();

		pugi::xml_node group_child = _xml_node.child( group_value );

		uint32_t count = 0;

		for( pugi::xml_node::iterator
			it_xml = group_child.begin(),
			it_xml_end = group_child.end();
			it_xml != it_xml_end;
			++it_xml )
		{
			const pugi::xml_node & child = *it_xml;

			const char * child_name = child.name();

			if( _type != child_name )
			{
				continue;
			}

			if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
			{
				continue;
			}

			++count;
		}

		_count = count;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metabuf::writeNodeGenerators_( const XmlNode * _node, const pugi::xml_node & _xml_node )
	{
		uint32_t generatorsTypeCount = 0;

		for( TMapNodes::const_iterator
			it = _node->inheritances.begin(),
			it_end = _node->inheritances.end();
			it != it_end;
			++it )
		{
			const XmlNode * node_inheritance = it->second;

			uint32_t generatorsCount;
			if( this->getNodeGeneratorSize_( _node, _xml_node, node_inheritance, generatorsCount ) == false )
			{
				return false;
			}

			if( generatorsCount == 0 )
			{
				continue;
			}

			++generatorsTypeCount;
		}

		this->writeSize( generatorsTypeCount );

		for( TMapNodes::const_iterator
			it = _node->inheritances.begin(),
			it_end = _node->inheritances.end();
			it != it_end;
			++it )
		{
			const XmlNode * node_inheritance = it->second;

			uint32_t generatorsCount;
			if( this->getNodeGeneratorSize_( _node, _xml_node, node_inheritance, generatorsCount ) == false )
			{
				return false;
			}

			if( generatorsCount == 0 )
			{
				continue;
			}

			this->writeSize( generatorsCount );

			this->writeSize( node_inheritance->id );

			for( pugi::xml_node::iterator
				it_xml = _xml_node.begin(),
				it_xml_end = _xml_node.end();
				it_xml != it_xml_end;
				++it_xml )
			{
				const pugi::xml_node & child = *it_xml;

				const char * child_name = child.name();

				if( node_inheritance->name != child_name )
				{
					continue;
				}

				if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
				{
					continue;
				}

				pugi::xml_attribute attr_generator = child.attribute( node_inheritance->generator.c_str() );

				const char * value_generator = attr_generator.value();

				const XmlNode * node_generator = _node->getGenerator( value_generator );

				if( node_generator == nullptr )
				{
					m_error << "Xml2Metabuf::writeNodeIncludes_: error write node '" << _node->name << "' includes '" << node_inheritance->name << "' not found generator '" << value_generator << "'" << std::endl;

					return false;
				}

				if( node_generator->getNoWrite() == true )
				{
					continue;
				}

				this->writeSize( node_generator->id );

				if( this->writeNode_( node_generator, child ) == false )
				{
					m_error << "Xml2Metabuf::writeNodeIncludes_: error write node '" << _node->name << "' includes '" << node_generator->name << "'" << std::endl;

					return false;
				}
			}
		}

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metabuf::getNodeGeneratorSize_( const XmlNode * _node, const pugi::xml_node & _xml_node, const XmlNode * _inheritance, uint32_t & _count ) const
    {
        uint32_t count = 0;

        for( pugi::xml_node::iterator
            it_xml = _xml_node.begin(),
            it_xml_end = _xml_node.end();
        it_xml != it_xml_end;
        ++it_xml )
        {
            const pugi::xml_node & child = *it_xml;

            const char * child_name = child.name();

            if( _inheritance->name != child_name )
            {
                continue;
            }

            if( _node->getInheritances( child_name ) == nullptr )
            {
                continue;
            }

            if( child.begin() == child.end() && child.attributes_begin() == child.attributes_end() )
            {
                continue;
            }

            pugi::xml_attribute attr_generator = child.attribute( _inheritance->generator.c_str() );

            const char * value_generator = attr_generator.value();

            const XmlNode * node_generator = _node->getGenerator( value_generator );

            if( node_generator == nullptr )
            {
                m_error << "Xml2Metabuf::getNodeGeneratorSize_: error node '" << _node->name << "' includes '" << _inheritance->name << "' not found generator '" << value_generator << "'" << std::endl;

                return false;
            }

            if( node_generator->getNoWrite() == true )
            {
                continue;
            }

            ++count;
        }

        _count = count;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::writeSize( uint32_t _value )
    {
        if( _value < 255 )
        {
            uint8_t size = (uint8_t)_value;
            this->write( size );
        }
        else
        {
            uint8_t size = 255;
            this->write( size );
            this->write( _value );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::writeString( const char * _value )
    {
        TVectorStringCache::iterator it_found = std::find( m_stringCache.begin(), m_stringCache.end(), _value );

        if( it_found == m_stringCache.end() )
        {
			it_found = m_stringCache.insert( m_stringCache.end(), _value );
        }

		TVectorStringCache::difference_type index = std::distance( m_stringCache.begin(), it_found );

		uint32_t write_index = (uint32_t)index;

		this->writeSize( write_index );
    }
    //////////////////////////////////////////////////////////////////////////
    void Xml2Metabuf::writeBuffer( const uint8_t * _buff, size_t _size )
    {
        m_buff.insert( m_buff.end(), _buff, _buff + _size );
    }
}