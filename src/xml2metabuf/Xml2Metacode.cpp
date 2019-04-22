#include "Xml2Metacode.hpp"
#include "XmlProtocol.hpp"

//#ifndef METABUF_METATYPE_HEADER
//#define METABUF_METATYPE_HEADER Metatype.h
//#endif

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    Xml2Metacode::Xml2Metacode( const XmlProtocol * _protocol )
        : m_protocol( _protocol )
        , m_indent( 0 )
    {

    }
    //////////////////////////////////////////////////////////////////////////
    std::string Xml2Metacode::getError()
    {
        return m_error.str();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::generate( std::string & _header, std::string & _source, const Xml2Settings & _settings )
    {
		m_settings = _settings;

        std::stringstream header;

        if( this->generateHeader( header ) == false )
        {
            return false;
        }

        _header = header.str();

        std::stringstream source;

        if( this->generateSource( source ) == false )
        {
            return false;
        }

        _source = source.str();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::generateHeader( std::stringstream & _ss )
    {
        this->write( _ss ) << "#pragma once" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "#include \"" << m_settings.metatype_h << "\"" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "#include \"metabuf/Metadata.hpp\"" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "namespace Metacode" << std::endl;
        this->write( _ss ) << "{" << std::endl;
        this->write( _ss ) << "    const size_t header_size = 20;" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "    typedef uint32_t enum_t;" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_magic();" << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_version();" << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_protocol_version();" << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_protocol_crc32();" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "    const char * getHeaderErrorMessage( Metabuf::HeaderError _error );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "    Metabuf::HeaderError readHeader( const void * _buff, size_t _size, size_t & _read, uint32_t & _readVersion, uint32_t & _needVersion, uint32_t & _readProtocol, uint32_t & _needProtocol, uint32_t _metaVersion, uint32_t & _readMetaVersion );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "    bool readStrings( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringCount );" << std::endl;
        this->write( _ss ) << "    const char * readString( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringSize, int64_t & _stringHash );" << std::endl;
        this->write( _ss ) << std::endl;

        m_indent += 4;

        const TMapMetas & metas = m_protocol->getMetas();

        for( TMapMetas::const_iterator
            it_meta = metas.begin(),
            it_meta_end = metas.end();
            it_meta != it_meta_end;
            ++it_meta )
        {
            const XmlMeta * meta = it_meta->second;

            if( this->writeHeaderMeta_( _ss, meta ) == false )
            {
                return false;
            }
        }

        m_indent -= 4;

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderMeta_( std::stringstream & _ss, const XmlMeta * _meta )
    {
        this->write( _ss ) << "namespace " << _meta->getWriteName() << std::endl;
        this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "    uint32_t getVersion();" << std::endl;
		this->write( _ss ) << std::endl;

        m_indent += 4;

        const TMapNodes & nodes = _meta->getNodes();

        for( TMapNodes::const_iterator
            it_node = nodes.begin(),
            it_node_end = nodes.end();
            it_node != it_node_end;
            ++it_node )
        {
            const XmlNode * node = it_node->second;

            if( this->writeHeaderNode_( _ss, _meta, node, true ) == false )
            {
                return false;
            }
        }

        m_indent -= 4;

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderNode_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node, bool _root )
    {
        this->write( _ss ) << "class " << _node->getWriteName() << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    : public " << node_inheritance->getWriteName() << std::endl;
        }
        else
        {
			if( _root == true )
			{
				this->write( _ss ) << "    : public Metabuf::Metaparse" << std::endl;
			}
			else
			{
				this->write( _ss ) << "    : public Metabuf::Metadata" << std::endl;
			}
        }

        this->write( _ss ) << "{ " << std::endl;

        //size_t id = _node->id;
        //s_writeStream( _stream, id );

        if( this->writeHeaderConstructor_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderAttributeSetup_( _ss, _node ) == false )
        {
            return false;
        }

		this->write( _ss ) << "public:" << std::endl;

		m_indent += 4;

		if( this->writeHeaderParse_( _ss, _node ) == false )
		{
			return false;
		}

		m_indent -= 4;

		this->write( _ss ) << std::endl;

        this->write( _ss ) << "protected:" << std::endl;

        m_indent += 4;

        if( this->writeHeaderDataReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderAttributeReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderSinglesReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderIncludesPreparation_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderIncludesReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderChildrenPreparation_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderChildrenReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderGeneratorsReader_( _ss, _node ) == false )
        {
            return false;
        }

        m_indent -= 4;

		this->write( _ss ) << std::endl;

		this->write( _ss ) << "public:" << std::endl;

        if( this->writeHeaderIncludesDefinition_( _ss, _meta, _node ) == false )
        {
            return false;
        }

        this->write( _ss ) << "protected:" << std::endl;

        if( this->writeHeaderAttribute_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderSingles_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderIncludes_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderChildren_( _ss, _meta, _node ) == false )
        {
            return false;
        }

        this->write( _ss ) << "};" << std::endl;
        this->write( _ss ) << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderConstructor_( std::stringstream & _ss, const XmlNode * _node )
    {
        this->write( _ss ) << "public:" << std::endl;
        this->write( _ss ) << "    " << _node->getWriteName() << "();" << std::endl;

		if( _node->generator.empty() == false )
		{
			this->write( _ss ) << "    virtual ~" << _node->getWriteName() << "();" << std::endl;
		}
		else if( _node->inheritances.empty() == false )
        {
            this->write( _ss ) << "    ~" << _node->getWriteName() << "();" << std::endl;
        }

        this->write( _ss ) << std::endl;

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderParse_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "bool parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );" << std::endl;

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderDataReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( this->hasNodeDataSize_( _node ) == false )
        {
            return true;
        }

        this->write( _ss ) << "void _parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderAttributeReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( this->hasNodeAttributeSize_( _node ) == false )
        {
            return true;
        }

        this->write( _ss ) << "void _parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderAttributeSetup_( std::stringstream & _ss, const XmlNode * _node )
    {
		if( _node->attributes.empty() == true && _node->members.empty() == true )
		{
			return true;
		}

        this->write( _ss ) << "public:" << std::endl;

        m_indent += 4;

        for( TMapAttributes::const_iterator
            it_attributes = _node->attributes.begin(),
            it_attributes_end = _node->attributes.end();
            it_attributes != it_attributes_end;
            ++it_attributes )
        {
            const XmlAttribute * attr = &it_attributes->second;

            XmlType type;
            m_protocol->getType( attr->type, type );

            if( attr->required == false )
            {
                this->write( _ss ) << "bool has_" << attr->name << "() const" << std::endl;
                this->write( _ss ) << "{" << std::endl;
                this->write( _ss ) << "    return " << attr->getWriteName() << "_successful;" << std::endl;
                this->write( _ss ) << "}" << std::endl;
                this->write( _ss ) << std::endl;

                if( type.is_ncr == true || type.is_enumerator == true )
                {
                    if( type.is_template == true )
                    {
                        this->write( _ss ) << "template<class T>" << std::endl;
                        this->write( _ss ) << "T getd_" << attr->name << "( T _default ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                        this->write( _ss ) << "    {" << std::endl;
                        this->write( _ss ) << "        return _default;" << std::endl;
                        this->write( _ss ) << "    }" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    return static_cast<T>(this->" << attr->getWriteName() << ");" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "template<class T>" << std::endl;
                        this->write( _ss ) << "bool get_" << attr->name << "( T * _value ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                        this->write( _ss ) << "    {" << std::endl;
                        this->write( _ss ) << "        return false;" << std::endl;
                        this->write( _ss ) << "    }" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    *_value = static_cast<T>(this->" << attr->getWriteName() << ");" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    return true;" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "template<class T>" << std::endl;
                        this->write( _ss ) << "bool get_" << attr->name << "( T * _value, const T & _default ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                        this->write( _ss ) << "    {" << std::endl;
                        this->write( _ss ) << "        *_value = _default;" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "        return false;" << std::endl;
                        this->write( _ss ) << "    }" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    *_value = static_cast<T>(this->" << attr->getWriteName() << ");" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    return true;" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                    }
                    else
                    {
                        this->write( _ss ) << type.write << " getd_" << attr->name << "( " << type.write << " _default ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                        this->write( _ss ) << "    {" << std::endl;
                        this->write( _ss ) << "        return _default;" << std::endl;
                        this->write( _ss ) << "    }" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    return this->" << attr->getWriteName() << ";" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "bool get_" << attr->name << "( " << type.write << " * _value ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                        this->write( _ss ) << "    {" << std::endl;
                        this->write( _ss ) << "        return false;" << std::endl;
                        this->write( _ss ) << "    }" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    *_value = this->" << attr->getWriteName() << ";" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    return true;" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "template<class C, class M>" << std::endl;
                        this->write( _ss ) << "bool getm_" << attr->name << "( C _self, M _method ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                        this->write( _ss ) << "    {" << std::endl;
                        this->write( _ss ) << "        return false;" << std::endl;
                        this->write( _ss ) << "    }" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    (_self->*_method)( this->" << attr->getWriteName() << " );" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    return true;" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "bool getd_" << attr->name << "( " << type.write << " * _value, const " << type.write << " & _default ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                        this->write( _ss ) << "    {" << std::endl;
                        this->write( _ss ) << "        *_value = _default;" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "        return false;" << std::endl;
                        this->write( _ss ) << "    }" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    *_value = this->" << attr->getWriteName() << ";" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "    return true;" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                    }
                }
                else
                {
                    this->write( _ss ) << "template<class C, class M>" << std::endl;
                    this->write( _ss ) << "bool getm_" << attr->name << "( C _self, M _method ) const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                    this->write( _ss ) << "    {" << std::endl;
                    this->write( _ss ) << "        return false;" << std::endl;
                    this->write( _ss ) << "    }" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    (_self->*_method)( this->" << attr->getWriteName() << " );" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    return true;" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "bool get_" << attr->name << "( " << type.write << " * _value ) const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                    this->write( _ss ) << "    {" << std::endl;
                    this->write( _ss ) << "        return false;" << std::endl;
                    this->write( _ss ) << "    }" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    *_value = this->" << attr->getWriteName() << ";" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    return true;" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "bool getd_" << attr->name << "( " << type.write << " * _value, const " << type.write << " & _default ) const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    if( " << attr->getWriteName() << "_successful == false )" << std::endl;
                    this->write( _ss ) << "    {" << std::endl;
                    this->write( _ss ) << "        *_value = _default;" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "        return false;" << std::endl;
                    this->write( _ss ) << "    }" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    *_value = this->" << attr->getWriteName() << ";" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    return true;" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                }
            }
            else
            {
                if( type.is_ncr == true || type.is_enumerator == true )
                {
                    if( type.is_template == true )
                    {
                        this->write( _ss ) << "template<class T>" << std::endl;
                        this->write( _ss ) << "T get_" << attr->name << "() const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    return static_cast<T>(this->" << attr->getWriteName() << ");" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                    }
                    else
                    {
                        this->write( _ss ) << "template<class C, class M>" << std::endl;
                        this->write( _ss ) << "void getm_" << attr->name << "( C _self, M _method ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    (_self->*_method)( this->" << attr->getWriteName() << " );" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << type.write << " get_" << attr->name << "() const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    return this->" << attr->getWriteName() << ";" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                    }
                }
                else
                {
                    this->write( _ss ) << "template<class C, class M>" << std::endl;
                    this->write( _ss ) << "void getm_" << attr->name << "( C _self, M _method ) const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    (_self->*_method)( this->" << attr->getWriteName() << " );" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "const " << type.write << " & get_" << attr->name << "() const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    return this->" << attr->getWriteName() << ";" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                }

                this->write( _ss ) << std::endl;
            }
        }

        for( TMapMembers::const_iterator
            it_members = _node->members.begin(),
            it_members_end = _node->members.end();
            it_members != it_members_end;
            ++it_members )
        {
            const XmlMember * member = &it_members->second;

            for( TMapAttributes::const_iterator
                it_attributes = member->attributes.begin(),
                it_attributes_end = member->attributes.end();
                it_attributes != it_attributes_end;
                ++it_attributes )
            {
                const XmlAttribute * attr = &it_attributes->second;

                XmlType type;
                m_protocol->getType( attr->type, type );

                if( attr->required == false )
                {
                    this->write( _ss ) << "bool has_" << member->name << "_" << attr->name << "() const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    return " << member->getWriteName() << "_" << attr->name << "_successful;" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "template<class C, class M>" << std::endl;
                    this->write( _ss ) << "bool getm_" << member->name << "_" << attr->name << "( C _self, M _method ) const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    if( " << member->getWriteName() << "_" << attr->name << "_successful == false )" << std::endl;
                    this->write( _ss ) << "    {" << std::endl;
                    this->write( _ss ) << "        return false;" << std::endl;
                    this->write( _ss ) << "    }" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    (_self->*_method)( this->" << member->getWriteName() << "_" << attr->name << " );" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    return true;" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "bool get_" << member->name << "_" << attr->name << "( " << type.write << " * _value ) const" << std::endl;
                    this->write( _ss ) << "{" << std::endl;
                    this->write( _ss ) << "    if( " << member->getWriteName() << "_" << attr->name << "_successful == false )" << std::endl;
                    this->write( _ss ) << "    {" << std::endl;
                    this->write( _ss ) << "        return false;" << std::endl;
                    this->write( _ss ) << "    }" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    *_value = this->" << member->getWriteName() << "_" << attr->name << ";" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "    return true;" << std::endl;
                    this->write( _ss ) << "}" << std::endl;
                    this->write( _ss ) << std::endl;
                }
                else
                {
                    if( type.is_ncr == true || type.is_enumerator == true )
                    {
                        if( type.is_template == true )
                        {
                            this->write( _ss ) << "template<class T>" << std::endl;
                            this->write( _ss ) << "T get_" << member->name << "_" << attr->name << "() const" << std::endl;
                            this->write( _ss ) << "{" << std::endl;
                            this->write( _ss ) << "    return static_cast<T>(this->" << member->getWriteName() << "_" << attr->name << ");" << std::endl;
                            this->write( _ss ) << "}" << std::endl;
                        }
                        else
                        {
                            this->write( _ss ) << "template<class C, class M>" << std::endl;
                            this->write( _ss ) << "void getm_" << member->name << "_" << attr->name << "( C _self, M _method ) const" << std::endl;
                            this->write( _ss ) << "{" << std::endl;
                            this->write( _ss ) << "    (_self->*_method)( this->" << member->getWriteName() << "_" << attr->name << " );" << std::endl;
                            this->write( _ss ) << "}" << std::endl;
                            this->write( _ss ) << std::endl;
                            this->write( _ss ) << type.write << " get_" << member->name << "_" << attr->name << "() const" << std::endl;
                            this->write( _ss ) << "{" << std::endl;
                            this->write( _ss ) << "    return this->" << member->getWriteName() << "_" << attr->name << ";" << std::endl;
                            this->write( _ss ) << "}" << std::endl;
                        }
                    }
                    else
                    {
                        this->write( _ss ) << "template<class C, class M>" << std::endl;
                        this->write( _ss ) << "void getm_" << member->name << "_" << attr->name << "( C _self, M _method ) const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    (_self->*_method)( this->" << member->getWriteName() << "_" << attr->name << " );" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                        this->write( _ss ) << std::endl;
                        this->write( _ss ) << "const " << type.write << " & get_" << member->name << "_" << attr->name << "() const" << std::endl;
                        this->write( _ss ) << "{" << std::endl;
                        this->write( _ss ) << "    return this->" << member->getWriteName() << "_" << attr->name << ";" << std::endl;
                        this->write( _ss ) << "}" << std::endl;
                    }

                    this->write( _ss ) << std::endl;
                }
            }
        }

        m_indent -= 4;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderIncludesDefinition_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node )
    {
        m_indent += 4;

        for( TMapNodes::const_iterator
            it_includes = _node->includes.begin(),
            it_includes_end = _node->includes.end();
            it_includes != it_includes_end;
            ++it_includes )
        {
            XmlNode * node = it_includes->second;

            this->writeHeaderNode_( _ss, _meta, node, false );
        }

        for( TMapNodes::const_iterator
            it_inheritances = _node->inheritances.begin(),
            it_inheritances_end = _node->inheritances.end();
            it_inheritances != it_inheritances_end;
            ++it_inheritances )
        {
            XmlNode * node = it_inheritances->second;

            this->writeHeaderNode_( _ss, _meta, node, false );
        }

        for( TMapNodes::const_iterator
            it_singles = _node->singles.begin(),
            it_singles_end = _node->singles.end();
            it_singles != it_singles_end;
            ++it_singles )
        {
            XmlNode * node = it_singles->second;

            this->writeHeaderNode_( _ss, _meta, node, false );
        }

        for( TMapNodes::const_iterator
            it_generators = _node->generators.begin(),
            it_generators_end = _node->generators.end();
            it_generators != it_generators_end;
            ++it_generators )
        {
            XmlNode * node = it_generators->second;

            this->writeHeaderNode_( _ss, _meta, node, false );
        }

        m_indent -= 4;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderAttribute_( std::stringstream & _ss, const XmlNode * _node )
    {
        this->write( _ss ) << "protected:" << std::endl;

        m_indent += 4;

        for( TMapAttributes::const_iterator
            it_attributes = _node->attributes.begin(),
            it_attributes_end = _node->attributes.end();
            it_attributes != it_attributes_end;
            ++it_attributes )
        {
            const XmlAttribute * attr = &it_attributes->second;

            if( attr->required == false )
            {
                this->write( _ss ) << "bool" << " " << attr->getWriteName() << "_successful;" << std::endl;
            }

            XmlType type;
            m_protocol->getType( attr->type, type );

            if( type.is_template == false )
            {
                this->write( _ss ) << type.write << " " << attr->getWriteName() << ";" << std::endl;
            }
            else
            {
                if( type.is_enumerator == true )
                {
                    this->write( _ss ) << "enum_t " << attr->getWriteName() << ";" << std::endl;
                }
                else
                {
                    m_error << "Xml2Metacode::writeHeaderAttribute_: invalid template non enumerator type '" << attr->name << "'" << std::endl;
                }
            }
        }

        for( TMapMembers::const_iterator
            it_members = _node->members.begin(),
            it_members_end = _node->members.end();
            it_members != it_members_end;
            ++it_members )
        {
            const XmlMember * member = &it_members->second;

            for( TMapAttributes::const_iterator
                it_attributes = member->attributes.begin(),
                it_attributes_end = member->attributes.end();
                it_attributes != it_attributes_end;
                ++it_attributes )
            {
                const XmlAttribute * attr = &it_attributes->second;

                if( attr->required == false )
                {
                    this->write( _ss ) << "bool" << " " << member->getWriteName() << "_" << attr->name << "_successful" << ";" << std::endl;
                }

                XmlType type;
                m_protocol->getType( attr->type, type );

                if( type.is_template == false )
                {
                    this->write( _ss ) << type.write << " " << member->getWriteName() << "_" << attr->name << ";" << std::endl;
                }
                else
                {
                    if( type.is_enumerator == true )
                    {
                        this->write( _ss ) << "enum_t " << member->getWriteName() << "_" << attr->name << ";" << std::endl;
                    }
                    else
                    {
                        m_error << "Xml2Metacode::writeHeaderAttribute_: invalid member template non enumerator type '" << attr->name << "'" << std::endl;
                    }
                }
            }
        }

        m_indent -= 4;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderSingles_( std::stringstream & _ss, const XmlNode * _node )
    {
        for( TMapNodes::const_iterator
            it_singles = _node->singles.begin(),
            it_singles_end = _node->singles.end();
            it_singles != it_singles_end;
            ++it_singles )
        {
            const XmlNode * node_single = it_singles->second;

            this->write( _ss ) << "public:" << std::endl;
            this->write( _ss ) << "    const " << node_single->getWriteName() << " & " << "get_Single_" << node_single->name << "() const" << std::endl;
            this->write( _ss ) << "    {" << std::endl;
            this->write( _ss ) << "        return this->single_" << node_single->getWriteName() << ";" << std::endl;
            this->write( _ss ) << "    }" << std::endl;
            this->write( _ss ) << std::endl;
            this->write( _ss ) << "protected:" << std::endl;
            this->write( _ss ) << "    " << node_single->getWriteName() << " single_" << node_single->getWriteName() << ";" << std::endl;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderSinglesReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( _node->singles.empty() == true )
        {
            return true;
        }

        this->write( _ss ) << "void _parseSingles( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderIncludes_( std::stringstream & _ss, const XmlNode * _node )
    {
        for( TMapNodes::const_iterator
            it_includes = _node->includes.begin(),
            it_includes_end = _node->includes.end();
            it_includes != it_includes_end;
            ++it_includes )
        {
            const XmlNode * node_include = it_includes->second;

            this->write( _ss ) << "public:" << std::endl;

            this->write( _ss ) << "    typedef Metabuf::Vector<" << node_include->getWriteName() << "> Vector" << node_include->getWriteName() << ";" << std::endl;
            this->write( _ss ) << std::endl;
            this->write( _ss ) << "    const Vector" << node_include->getWriteName() << " & " << "get_Includes_" << node_include->name << "() const" << std::endl;
            this->write( _ss ) << "    {" << std::endl;
            this->write( _ss ) << "        return this->includes_" << node_include->getWriteName() << ";" << std::endl;
            this->write( _ss ) << "    }" << std::endl;
            this->write( _ss ) << std::endl;
            this->write( _ss ) << "protected:" << std::endl;
            this->write( _ss ) << "    Vector" << node_include->getWriteName() << " includes_" << node_include->getWriteName() << ";" << std::endl;
        }

        for( TMapNodes::const_iterator
            it_inheritances = _node->inheritances.begin(),
            it_inheritances_end = _node->inheritances.end();
            it_inheritances != it_inheritances_end;
            ++it_inheritances )
        {
            const XmlNode * node_include = it_inheritances->second;

            this->write( _ss ) << "public:" << std::endl;
            this->write( _ss ) << "    typedef Metabuf::Vector<" << node_include->getWriteName() << " *> Vector" << node_include->getWriteName() << ";" << std::endl;
            this->write( _ss ) << std::endl;
            this->write( _ss ) << "    const Vector" << node_include->getWriteName() << " & " << "get_Includes_" << node_include->name << "() const" << std::endl;
            this->write( _ss ) << "    {" << std::endl;
            this->write( _ss ) << "        return this->includes_" << node_include->getWriteName() << ";" << std::endl;
            this->write( _ss ) << "    }" << std::endl;
            this->write( _ss ) << std::endl;
            this->write( _ss ) << "protected:" << std::endl;
            this->write( _ss ) << "    Vector" << node_include->getWriteName() << " includes_" << node_include->getWriteName() << ";" << std::endl;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node )
    {
        this->write( _ss ) << "void _preparationIncludes( uint32_t _id, uint32_t _count );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderIncludesReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        this->write( _ss ) << "void _parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderChildren_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node )
    {
        for( TMapChildren::const_iterator
            it_children = _node->children.begin(),
            it_children_end = _node->children.end();
            it_children != it_children_end;
            ++it_children )
        {
            const XmlChildren & children = it_children->second;

            this->write( _ss ) << "public:" << std::endl;

            const XmlNode * node_children = _meta->getNode( children.type );

            std::string ss_vector_children_name = "Vector" + node_children->getWriteName();

            this->write( _ss ) << "    typedef Metabuf::Vector<" << node_children->getWriteName() << " *> " << ss_vector_children_name << ";" << std::endl;
            this->write( _ss ) << std::endl;
            this->write( _ss ) << "    const " << ss_vector_children_name << " & " << "get_Children_" << node_children->name << "() const" << std::endl;
            this->write( _ss ) << "    {" << std::endl;
            this->write( _ss ) << "        return this->children_" << node_children->getWriteName() << ";" << std::endl;
            this->write( _ss ) << "    }" << std::endl;
            this->write( _ss ) << std::endl;
            this->write( _ss ) << "protected:" << std::endl;
            this->write( _ss ) << "    " << ss_vector_children_name << " children_" << node_children->getWriteName() << ";" << std::endl;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderChildrenPreparation_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( _node->children.empty() == true )
        {
            return true;
        }

        this->write( _ss ) << "void _preparationChildren( uint32_t _id, uint32_t _count );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderChildrenReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( _node->children.empty() == true )
        {
            return  true;
        }

        this->write( _ss ) << "void _parseChildren( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( _node->generators.empty() == true || _node->inheritances.empty() == true )
        {
            return true;
        }

        this->write( _ss ) << "void _parseGenerators( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData );" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::generateSource( std::stringstream & _ss )
    {
        uint32_t protocol_version = m_protocol->getVersion();
        uint32_t protocol_crc32 = m_protocol->getCrc32();

        this->write( _ss ) << "#include \"" << m_settings.metacode_h << "\"" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "namespace Metacode" << std::endl;
        this->write( _ss ) << "{" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_magic()" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        return 3133062829u;" << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_version()" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        return " << METABUF_BIN_VERSION << ";" << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_protocol_version()" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        return " << protocol_version << ";" << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    uint32_t get_metacode_protocol_crc32()" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        return " << protocol_crc32 << "; " << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    const char * getHeaderErrorMessage( Metabuf::HeaderError _error )" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        switch( _error )" << std::endl;
        this->write( _ss ) << "        {" << std::endl;
        this->write( _ss ) << "        case Metabuf::HEADER_SUCCESSFUL: return \"Successful\";" << std::endl;
        this->write( _ss ) << "        case Metabuf::HEADER_INVALID_MAGIC: return \"invalid magic header\";" << std::endl;
        this->write( _ss ) << "        case Metabuf::HEADER_INVALID_VERSION: return \"invalid version\";" << std::endl;
        this->write( _ss ) << "        case Metabuf::HEADER_INVALID_PROTOCOL_VERSION: return \"invalid protocol version\";" << std::endl;
        this->write( _ss ) << "        case Metabuf::HEADER_INVALID_PROTOCOL_CRC32: return \"invalid protocol crc32\";" << std::endl;
        this->write( _ss ) << "        case Metabuf::HEADER_INVALID_METAVERSION: return \"invalid meta version\";" << std::endl;
        this->write( _ss ) << "        default: return \"invalid error\";" << std::endl;
        this->write( _ss ) << "        }" << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    Metabuf::HeaderError readHeader( const void * _buff, size_t _size, size_t & _read, uint32_t & _readVersion, uint32_t & _needVersion, uint32_t & _readProtocol, uint32_t & _needProtocol, uint32_t _metaVersion, uint32_t & _readMetaVersion )" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        uint32_t metacode_magic = get_metacode_magic();" << std::endl;
        this->write( _ss ) << "        uint32_t metacode_version = get_metacode_version();" << std::endl;
        this->write( _ss ) << "        uint32_t metacode_protocol_version = get_metacode_protocol_version();" << std::endl;
        this->write( _ss ) << "        uint32_t metacode_protocol_crc32 = get_metacode_protocol_crc32();" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        Metabuf::Reader ar(_buff, _size, _read);" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        uint32_t head;" << std::endl;
        this->write( _ss ) << "        ar.readPOD( head );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        if( head != metacode_magic )" << std::endl;
        this->write( _ss ) << "        {" << std::endl;
        this->write( _ss ) << "            return Metabuf::HEADER_INVALID_MAGIC;" << std::endl;
        this->write( _ss ) << "        }" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        uint32_t read_version;" << std::endl;
        this->write( _ss ) << "        ar.readPOD( read_version );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        uint32_t read_protocol_version;" << std::endl;
        this->write( _ss ) << "        ar.readPOD( read_protocol_version );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        uint32_t read_protocol_crc32;" << std::endl;
        this->write( _ss ) << "        ar.readPOD( read_protocol_crc32 );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        uint32_t read_meta_version;" << std::endl;
        this->write( _ss ) << "        ar.readPOD( read_meta_version );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        _readVersion = read_version;" << std::endl;
        this->write( _ss ) << "        _needVersion = metacode_version;" << std::endl;
        this->write( _ss ) << "        _readProtocol = read_protocol_version;" << std::endl;
        this->write( _ss ) << "        _needProtocol = metacode_protocol_version;" << std::endl;
        this->write( _ss ) << "        _readMetaVersion = read_meta_version;" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        if( read_version != metacode_version )" << std::endl;
        this->write( _ss ) << "        {" << std::endl;
        this->write( _ss ) << "            return Metabuf::HEADER_INVALID_VERSION;" << std::endl;
        this->write( _ss ) << "        }" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        if( read_protocol_version != metacode_protocol_version )" << std::endl;
        this->write( _ss ) << "        {" << std::endl;
        this->write( _ss ) << "            return Metabuf::HEADER_INVALID_PROTOCOL_VERSION;" << std::endl;
        this->write( _ss ) << "        }" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        if( read_protocol_crc32 != metacode_protocol_crc32 )" << std::endl;
        this->write( _ss ) << "        {" << std::endl;
        this->write( _ss ) << "            return Metabuf::HEADER_INVALID_PROTOCOL_CRC32;" << std::endl;
        this->write( _ss ) << "        }" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        if( read_meta_version != _metaVersion )" << std::endl;
        this->write( _ss ) << "        {" << std::endl;
        this->write( _ss ) << "            return Metabuf::HEADER_INVALID_METAVERSION;" << std::endl;
        this->write( _ss ) << "        }" << std::endl;
		this->write( _ss ) << std::endl;
        this->write( _ss ) << "        return Metabuf::HEADER_SUCCESSFUL;" << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    bool readStrings( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringCount )" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        Metabuf::Reader ar(_buff, _size, _read);" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        uint32_t count;" << std::endl;
        this->write( _ss ) << "        ar.readPOD( count );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        _stringCount = count;" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        return true;" << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "    const char * readString( const void * _buff, size_t _size, size_t & _read, uint32_t & _stringSize, int64_t & _stringHash )" << std::endl;
        this->write( _ss ) << "    {" << std::endl;
        this->write( _ss ) << "        Metabuf::Reader ar(_buff, _size, _read);" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        uint32_t size;" << std::endl;
        this->write( _ss ) << "        ar.readSize( size );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        int64_t hash;" << std::endl;
        this->write( _ss ) << "        ar.readPOD( hash );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        const char * value = ar.current_buff<char>();" << std::endl;
        this->write( _ss ) << "        ar.skip( size );" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        _stringSize = size;" << std::endl;
        this->write( _ss ) << "        _stringHash = hash;" << std::endl;
        this->write( _ss ) << std::endl;
        this->write( _ss ) << "        return value;" << std::endl;
        this->write( _ss ) << "    }" << std::endl;
        this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;

        m_indent += 4;

        const TMapMetas & metas = m_protocol->getMetas();

        for( TMapMetas::const_iterator
            it_meta = metas.begin(),
            it_meta_end = metas.end();
            it_meta != it_meta_end;
            ++it_meta )
        {
            const XmlMeta * meta = it_meta->second;

            if( this->writeSourceMeta_( _ss, meta ) == false )
            {
                return false;
            }
        }

        m_indent -= 4;

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceMeta_( std::stringstream & _ss, const XmlMeta * _meta )
    {
        this->write( _ss ) << "namespace " << _meta->getWriteName() << std::endl;
        this->write( _ss ) << "{ " << std::endl;
		this->write( _ss ) << "    uint32_t getVersion()" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        return " << _meta->getVersion() << ";" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << std::endl;

        m_indent += 4;

        const TMapNodes & nodes = _meta->getNodes();

        for( TMapNodes::const_iterator
            it_nodes = nodes.begin(),
            it_nodes_end = nodes.end();
            it_nodes != it_nodes_end;
            ++it_nodes )
        {
            const XmlNode * node = it_nodes->second;

            if( this->writeSourceNode_( _ss, _meta, node, true ) == false )
            {
                return false;
            }
        }

        m_indent -= 4;

        this->write( _ss ) << "} " << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceNode_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node, bool _root )
    {
        if( this->writeSourceConstructor_( _ss, _meta, _node, _root ) == false )
        {
            return false;
        }

		if( this->writeSourceParse_( _ss, _node ) == false )
		{
			return false;
		}

        if( this->writeSourceDataReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeSourceAttributeReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeSourceSinglesReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( writeSourceIncludesPreparation_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeSourceIncludesReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( writeSourceChildrenPreparation_( _ss, _meta, _node ) == false )
        {
            return false;
        }

        if( writeSourceChildrenReader_( _ss, _meta, _node ) == false )
        {
            return false;
        }

        if( this->writeSourceGeneratorsReader_( _ss, _node ) == false )
        {
            return false;
        }

        if( this->writeSourceIncludesDefinition_( _ss, _meta, _node ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceConstructor_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node, bool _root )
    {
        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "//cppcheck-suppress uninitMemberVar" << std::endl;
        this->write( _ss ) << _node->getScope() << "::" << _node->getWriteName() << "()" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    : " << node_inheritance->getWriteName() << "()" << std::endl;
        }
        else
        {
			if( _root == true )
			{
				this->write( _ss ) << "    : Metabuf::Metaparse()" << std::endl;
			}
			else
			{
				this->write( _ss ) << "    : Metabuf::Metadata()" << std::endl;
			}
        }

        for( TMapAttributes::const_iterator
            it_attributtes = _node->attributes.begin(),
            it_attributtes_end = _node->attributes.end();
            it_attributtes != it_attributtes_end;
            ++it_attributtes )
        {
            const XmlAttribute * attr = &it_attributtes->second;

            if( attr->required == false )
            {
                this->write( _ss ) << "    , " << attr->getWriteName() << "_successful(false)" << std::endl;
            }
        }

        for( TMapMembers::const_iterator
            it_members = _node->members.begin(),
            it_members_end = _node->members.end();
            it_members != it_members_end;
            ++it_members )
        {
            const XmlMember * member = &it_members->second;

            for( TMapAttributes::const_iterator
                it_attributtes = member->attributes.begin(),
                it_attributtes_end = member->attributes.end();
                it_attributtes != it_attributtes_end;
                ++it_attributtes )
            {
                const XmlAttribute * attr = &it_attributtes->second;

                if( attr->required == false )
                {
                    this->write( _ss ) << "    , " << member->getWriteName() << "_" << attr->name << "_successful(false)" << std::endl;
                }
            }
        }

        this->write( _ss ) << "{" << std::endl;
        this->write( _ss ) << "}" << std::endl;

		if( _node->inheritances.empty() == false || _node->generator.empty() == false )
        {
            this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
            this->write( _ss ) << _node->getScope() << "::~" << _node->getWriteName() << "()" << std::endl;
            this->write( _ss ) << "{" << std::endl;

			if( _node->inheritances.empty() == false )
			{
				for( TMapNodes::const_iterator
					it_inheritances = _node->inheritances.begin(),
					it_inheritances_end = _node->inheritances.end();
					it_inheritances != it_inheritances_end;
					++it_inheritances )
				{
					const XmlNode * node_include = it_inheritances->second;

					this->write( _ss ) << "    for( Vector" << node_include->getWriteName() << "::const_iterator" << std::endl;
					this->write( _ss ) << "        it = includes_" << node_include->getWriteName() << ".begin()," << std::endl;
					this->write( _ss ) << "        it_end = includes_" << node_include->getWriteName() << ".end();" << std::endl;
					this->write( _ss ) << "    it != it_end;" << std::endl;
					this->write( _ss ) << "    ++it )" << std::endl;
					this->write( _ss ) << "    {" << std::endl;
					this->write( _ss ) << "        delete *it;" << std::endl;
					this->write( _ss ) << "    }" << std::endl;
				}
			}

            this->write( _ss ) << "}" << std::endl;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::hasNodeDataSize_( const XmlNode * _node )
    {
        for( TMapAttributes::const_iterator
            it_attributes = _node->attributes.begin(),
            it_attributes_end = _node->attributes.end();
            it_attributes != it_attributes_end;
            ++it_attributes )
        {
            const XmlAttribute * attr = &it_attributes->second;

            if( attr->required == false )
            {
                continue;
            }

            return true;
        }

        for( TMapMembers::const_iterator
            it_members = _node->members.begin(),
            it_members_end = _node->members.end();
            it_members != it_members_end;
            ++it_members )
        {
            const XmlMember * member = &it_members->second;

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

                return true;
            }
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::hasNodeAttributeSize_( const XmlNode * _node )
    {
        for( TMapAttributes::const_iterator
            it_attributes = _node->attributes.begin(),
            it_attributes_end = _node->attributes.end();
            it_attributes != it_attributes_end;
            ++it_attributes )
        {
            const XmlAttribute * attr = &it_attributes->second;

            if( attr->required == true )
            {
                continue;
            }

            return true;
        }

        for( TMapMembers::const_iterator
            it_members = _node->members.begin(),
            it_members_end = _node->members.end();
            it_members != it_members_end;
            ++it_members )
        {
            const XmlMember * member = &it_members->second;

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

                return true;
            }
        }

        return false;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceParse_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "bool " << _node->getScope() << "::parse( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )" << std::endl;
		this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "    (void)_buff;" << std::endl;
		this->write( _ss ) << "    (void)_size;" << std::endl;
		this->write( _ss ) << "    (void)_read;" << std::endl;
		this->write( _ss ) << "    (void)_userData;" << std::endl;

		if( this->hasNodeDataSize_( _node ) == true )
		{
			this->write( _ss ) << "    this->_parseData( _buff, _size, _read, _userData );" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( this->hasNodeAttributeSize_( _node ) == true )
		{
			this->write( _ss ) << "    uint32_t attributeCount;" << std::endl;
			this->write( _ss ) << "    Metabuf::readSize( _buff, _size, _read, attributeCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "    for( uint32_t i = 0; i != attributeCount; ++i )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;
			this->write( _ss ) << "        uint32_t id;" << std::endl;
			this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, id );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        this->_parseArguments( _buff, _size, _read, id, _userData );" << std::endl;
			this->write( _ss ) << "    }" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( _node->singles.empty() == false )
		{
			this->write( _ss ) << "    uint32_t singleTypeCount;" << std::endl;
			this->write( _ss ) << "    Metabuf::readSize( _buff, _size, _read, singleTypeCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "    for( uint32_t i = 0; i != singleTypeCount; ++i )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;
			this->write( _ss ) << "        uint32_t id;" << std::endl;
			this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, id );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        this->_parseSingles( _buff, _size, _read, id );" << std::endl;
			this->write( _ss ) << "    }" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( _node->includes.empty() == false )
		{
			if( _node->includes.size() == 1 && _node->inheritance.empty() == true )
			{				
				const XmlNode * node = _node->includes.begin()->second;

				this->write( _ss ) << "    uint32_t includeCount;" << std::endl;
				this->write( _ss ) << "    Metabuf::readSize( _buff, _size, _read, includeCount );" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "    if( includeCount != 0 )" << std::endl;
				this->write( _ss ) << "    {" << std::endl;
				this->write( _ss ) << "        includes_" << node->getWriteName() << ".reserve( includeCount );" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "        for( uint32_t j = 0; j != includeCount; ++j )" << std::endl;
				this->write( _ss ) << "        {" << std::endl;
				this->write( _ss ) << "            includes_" << node->getWriteName() << ".emplace_back( " << node->getScope() << "() );" << std::endl;
				this->write( _ss ) << "            " << node->getScope() << " & metadata = includes_" << node->getWriteName() << ".back();" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "            metadata.parse( _buff, _size, _read, _userData );" << std::endl;
				this->write( _ss ) << "        }" << std::endl;
				this->write( _ss ) << "    }" << std::endl;
				this->write( _ss ) << std::endl;
			}
			else
			{
				this->write( _ss ) << "    uint32_t includeTypeCount;" << std::endl;
				this->write( _ss ) << "    Metabuf::readSize( _buff, _size, _read, includeTypeCount );" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "    for( uint32_t i = 0; i != includeTypeCount; ++i )" << std::endl;
				this->write( _ss ) << "    {" << std::endl;
				this->write( _ss ) << "        uint32_t includeCount;" << std::endl;
				this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, includeCount );" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "        uint32_t id;" << std::endl;
				this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, id );" << std::endl;
				this->write( _ss ) << std::endl;
				
				if( _node->inheritance.empty() == false )
				{
					this->write( _ss ) << "        this->_preparationIncludes( id, includeCount );" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "        for( uint32_t j = 0; j != includeCount; ++j )" << std::endl;
					this->write( _ss ) << "        {" << std::endl;
					this->write( _ss ) << "            this->_parseIncludes( _buff, _size, _read, id, _userData );" << std::endl;
					this->write( _ss ) << "        }" << std::endl;
				}
				else
				{
					this->write( _ss ) << "        switch( id )" << std::endl;
					this->write( _ss ) << "        {" << std::endl;

					for( TMapNodes::const_iterator
						it_includes = _node->includes.begin(),
						it_includes_end = _node->includes.end();
						it_includes != it_includes_end;
						++it_includes )
					{
						const XmlNode * node = it_includes->second;

						this->write( _ss ) << "        case " << node->id << ":" << std::endl;
						this->write( _ss ) << "            {" << std::endl;
						this->write( _ss ) << "                includes_" << node->getWriteName() << ".reserve( includeCount );" << std::endl;
						this->write( _ss ) << std::endl;
						this->write( _ss ) << "                for( uint32_t j = 0; j != includeCount; ++j )" << std::endl;
						this->write( _ss ) << "                {" << std::endl;
						this->write( _ss ) << "                    includes_" << node->getWriteName() << ".emplace_back( " << node->getScope() << "() );" << std::endl;
						this->write( _ss ) << "                    " << node->getScope() << " & metadata = includes_" << node->getWriteName() << ".back();" << std::endl;
						this->write( _ss ) << std::endl;
						this->write( _ss ) << "                    metadata.parse( _buff, _size, _read, _userData );" << std::endl;
						this->write( _ss ) << "                }" << std::endl;
						this->write( _ss ) << "            }break;" << std::endl;
					}

					this->write( _ss ) << "        }" << std::endl;

				}

				this->write( _ss ) << "    }" << std::endl;
				this->write( _ss ) << std::endl;
			}
		}
		
		if( _node->children.empty() == false )
		{
			this->write( _ss ) << "    uint32_t childrenTypeCount;" << std::endl;
			this->write( _ss ) << "    Metabuf::readSize( _buff, _size, _read, childrenTypeCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "    for( uint32_t i = 0; i != childrenTypeCount; ++i )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;
			this->write( _ss ) << "        uint32_t childrenCount;" << std::endl;
			this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, childrenCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        uint32_t id;" << std::endl;
			this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, id );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        this->_preparationChildren( id, childrenCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        for( uint32_t j = 0; j != childrenCount; ++j )" << std::endl;
			this->write( _ss ) << "        {" << std::endl;
			this->write( _ss ) << "            uint32_t generator_id;" << std::endl;
			this->write( _ss ) << "            Metabuf::readSize( _buff, _size, _read, generator_id );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "            this->_parseChildren( _buff, _size, _read, generator_id, _userData );" << std::endl;
			this->write( _ss ) << "        }" << std::endl;
			this->write( _ss ) << "    }" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( _node->generators.empty() == false && _node->inheritances.empty() == false )
		{
			this->write( _ss ) << "    uint32_t generatorTypeCount;" << std::endl;
			this->write( _ss ) << "    Metabuf::readSize( _buff, _size, _read, generatorTypeCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "    for( uint32_t i = 0; i != generatorTypeCount; ++i )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;
			this->write( _ss ) << "        uint32_t generatorCount;" << std::endl;
			this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, generatorCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        uint32_t id;" << std::endl;
			this->write( _ss ) << "        Metabuf::readSize( _buff, _size, _read, id );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        this->_preparationIncludes( id, generatorCount );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "        for( uint32_t j = 0; j != generatorCount; ++j )" << std::endl;
			this->write( _ss ) << "        {" << std::endl;
			this->write( _ss ) << "            uint32_t generator_id;" << std::endl;
			this->write( _ss ) << "            Metabuf::readSize( _buff, _size, _read, generator_id );" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "            this->_parseGenerators( _buff, _size, _read, generator_id, _userData );" << std::endl;
			this->write( _ss ) << "        }" << std::endl;
			this->write( _ss ) << "    }" << std::endl;
			this->write( _ss ) << std::endl;
		}

		this->write( _ss ) << "    return true;" << std::endl;
		this->write( _ss ) << "}" << std::endl;

		return this;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceDataReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( this->hasNodeDataSize_( _node ) == false )
        {
            return true;
        }

        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "void " << _node->getScope() << "::_parseData( const uint8_t * _buff, size_t _size, size_t & _read, void * _userData )" << std::endl;
        this->write( _ss ) << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseData( _buff, _size, _read, _userData );" << std::endl;
            this->write( _ss ) << std::endl;
        }

        if( this->hasNodeDataSize_( _node ) == true )
        {
            for( TMapAttributes::const_iterator
                it_attributes = _node->attributes.begin(),
                it_attributes_end = _node->attributes.end();
                it_attributes != it_attributes_end;
                ++it_attributes )
            {
                const XmlAttribute * attr = &it_attributes->second;

                if( attr->required == false )
                {
                    continue;
                }

                this->write( _ss ) << "    Metabuf::read( _buff, _size, _read, _userData, this->" << attr->getWriteName() << " );" << std::endl;
            }

            for( TMapMembers::const_iterator
                it_members = _node->members.begin(),
                it_members_end = _node->members.end();
                it_members != it_members_end;
                ++it_members )
            {
                const XmlMember * member = &it_members->second;

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

                    this->write( _ss ) << "    Metabuf::read( _buff, _size, _read, _userData, this->" << member->getWriteName() << "_" << attr->name << " );" << std::endl;
                }
            }
        }

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceAttributeReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( this->hasNodeAttributeSize_( _node ) == false )
        {
            return true;
        }

        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "void " << _node->getScope() << "::_parseArguments( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )" << std::endl;
        this->write( _ss ) << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseArguments( _buff, _size, _read, _id, _userData );" << std::endl;
            this->write( _ss ) << std::endl;
        }

        if( this->hasNodeAttributeSize_( _node ) == true )
        {
            this->write( _ss ) << "    switch( _id )" << std::endl;
            this->write( _ss ) << "    {" << std::endl;

            for( TMapAttributes::const_iterator
                it_attributes = _node->attributes.begin(),
                it_attributes_end = _node->attributes.end();
                it_attributes != it_attributes_end;
                ++it_attributes )
            {
                const XmlAttribute * attr = &it_attributes->second;

                if( attr->required == true )
                {
                    continue;
                }

                this->write( _ss ) << "    case " << attr->id << ":" << std::endl;
                this->write( _ss ) << "        {" << std::endl;
                this->write( _ss ) << "            Metabuf::read( _buff, _size, _read, _userData, this->" << attr->getWriteName() << " );" << std::endl;
                this->write( _ss ) << std::endl;
                this->write( _ss ) << "            this->" << attr->getWriteName() << "_successful = true;" << std::endl;
                this->write( _ss ) << "        }break;" << std::endl;
            }

            for( TMapMembers::const_iterator
                it_members = _node->members.begin(),
                it_members_end = _node->members.end();
                it_members != it_members_end;
                ++it_members )
            {
                const XmlMember * member = &it_members->second;

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

                    this->write( _ss ) << "    case " << attr->id << ":" << std::endl;
                    this->write( _ss ) << "        {" << std::endl;
                    this->write( _ss ) << "            Metabuf::read( _buff, _size, _read, _userData, this->" << member->getWriteName() << "_" << attr->name << " );" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "            this->" << member->getWriteName() << "_" << attr->name << "_successful = true;" << std::endl;
                    this->write( _ss ) << "        }break;" << std::endl;
                }
            }

            this->write( _ss ) << "    default:" << std::endl;
            this->write( _ss ) << "        break;" << std::endl;
            this->write( _ss ) << "    }" << std::endl;
        }

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceSinglesReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( _node->singles.empty() == true )
        {
            return true;
        }

        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "void " << _node->getScope() << "::_parseSingles( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id )" << std::endl;
        this->write( _ss ) << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseSingles( _buff, _size, _read, _id );" << std::endl;

            this->write( _ss ) << std::endl;
        }

        this->write( _ss ) << "    switch( _id )" << std::endl;
        this->write( _ss ) << "    {" << std::endl;

        for( TMapNodes::const_iterator
            it = _node->singles.begin(),
            it_end = _node->singles.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            this->write( _ss ) << "    case " << node->id << ":" << std::endl;
            this->write( _ss ) << "        {" << std::endl;
            this->write( _ss ) << "            single_" << node->getWriteName() << ".parse( _buff, _size, _read, _userData );" << std::endl;
            this->write( _ss ) << "        }break;" << std::endl;
        }

        this->write( _ss ) << "    default:" << std::endl;
        this->write( _ss ) << "        break;" << std::endl;
        this->write( _ss ) << "    }" << std::endl;

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node )
    {	
        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "void " << _node->getScope() << "::_preparationIncludes( uint32_t _id, uint32_t _count )" << std::endl;
        this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "    (void)_id;" << std::endl;
		this->write( _ss ) << "    (void)_count;" << std::endl;

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    " << node_inheritance->getScope() << "::_preparationIncludes( _id, _count );" << std::endl;

			if( _node->includes.empty() == false )
			{
				this->write( _ss ) << std::endl;
			}
		}

        if( _node->includes.empty() == false || _node->inheritances.empty() == false )
        {
            this->write( _ss ) << "    switch( _id )" << std::endl;
            this->write( _ss ) << "    {" << std::endl;

            for( TMapNodes::const_iterator
                it_includes = _node->includes.begin(),
                it_includes_end = _node->includes.end();
                it_includes != it_includes_end;
                ++it_includes )
            {
                const XmlNode * node = it_includes->second;

                this->write( _ss ) << "    case " << node->id << ":" << std::endl;
                this->write( _ss ) << "        {" << std::endl;
                this->write( _ss ) << "            includes_" << node->getWriteName() << ".reserve( _count );" << std::endl;
                this->write( _ss ) << "        }break;" << std::endl;
            }

            for( TMapNodes::const_iterator
                it_inheritances = _node->inheritances.begin(),
                it_inheritances_end = _node->inheritances.end();
                it_inheritances != it_inheritances_end;
                ++it_inheritances )
            {
                const XmlNode * node = it_inheritances->second;

                this->write( _ss ) << "    case " << node->id << ":" << std::endl;
                this->write( _ss ) << "        {" << std::endl;
                this->write( _ss ) << "            includes_" << node->getWriteName() << ".reserve( _count );" << std::endl;
                this->write( _ss ) << "        }break;" << std::endl;
            }

            this->write( _ss ) << "    default:" << std::endl;
            this->write( _ss ) << "        break;" << std::endl;
            this->write( _ss ) << "    }" << std::endl;
        }

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceIncludesReader_( std::stringstream & _ss, const XmlNode * _node )
    {
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "void " << _node->getScope() << "::_parseIncludes( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )" << std::endl;
		this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "    (void)_buff;" << std::endl;
		this->write( _ss ) << "    (void)_size;" << std::endl;
		this->write( _ss ) << "    (void)_read;" << std::endl;
		this->write( _ss ) << "    (void)_id;" << std::endl;
		this->write( _ss ) << "    (void)_userData;" << std::endl;

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseIncludes( _buff, _size, _read, _id, _userData );" << std::endl;

			this->write( _ss ) << std::endl;
		}

		if( _node->includes.empty() == false )
		{
			this->write( _ss ) << "    switch( _id )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;

			for( TMapNodes::const_iterator
				it_includes = _node->includes.begin(),
				it_includes_end = _node->includes.end();
				it_includes != it_includes_end;
				++it_includes )
			{
				const XmlNode * node = it_includes->second;

				this->write( _ss ) << "    case " << node->id << ":" << std::endl;
				this->write( _ss ) << "        {" << std::endl;
				this->write( _ss ) << "            includes_" << node->getWriteName() << ".emplace_back( " << node->getScope() << "() );" << std::endl;
				this->write( _ss ) << "            " << node->getScope() << " & metadata = includes_" << node->getWriteName() << ".back();" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "            metadata.parse( _buff, _size, _read, _userData );" << std::endl;
				this->write( _ss ) << "        }break;" << std::endl;
			}

			this->write( _ss ) << "    default:" << std::endl;
			this->write( _ss ) << "        break;" << std::endl;
			this->write( _ss ) << "    }" << std::endl;
		}

		this->write( _ss ) << "}" << std::endl;
		

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceChildrenPreparation_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node )
    {
        if( _node->children.empty() == true )
        {
            return true;
        }

        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "void " << _node->getScope() << "::_preparationChildren( uint32_t _id, uint32_t _count )" << std::endl;
        this->write( _ss ) << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    " << node_inheritance->getScope() << "::_preparationChildren( _id, _count );" << std::endl;

            this->write( _ss ) << std::endl;
        }

        this->write( _ss ) << "    switch( _id )" << std::endl;
        this->write( _ss ) << "    {" << std::endl;

        for( TMapChildren::const_iterator
            it_children = _node->children.begin(),
            it_children_end = _node->children.end();
            it_children != it_children_end;
            ++it_children )
        {
            const XmlChildren & children = it_children->second;

            const XmlNode * node = _meta->getNode( children.type );

            this->write( _ss ) << "    case " << node->id << ":" << std::endl;
            this->write( _ss ) << "        {" << std::endl;
            this->write( _ss ) << "            children_" << node->getWriteName() << ".reserve( _count );" << std::endl;
            this->write( _ss ) << "        }break;" << std::endl;
        }

        this->write( _ss ) << "    default:" << std::endl;
        this->write( _ss ) << "        break;" << std::endl;
        this->write( _ss ) << "    }" << std::endl;

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceChildrenReader_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node )
    {
        if( _node->children.empty() == true )
        {
            return  true;
        }

        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "void " << _node->getScope() << "::_parseChildren( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id )" << std::endl;
        this->write( _ss ) << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseChildren( _buff, _size, _read, _id );" << std::endl;

            this->write( _ss ) << std::endl;
        }

        this->write( _ss ) << "    switch( _id )" << std::endl;
        this->write( _ss ) << "    {" << std::endl;

        for( TMapChildren::const_iterator
            it_includes = _node->children.begin(),
            it_includes_end = _node->children.end();
            it_includes != it_includes_end;
            ++it_includes )
        {
            const XmlChildren & children = it_includes->second;

            const XmlNode * node_children = _meta->getNode( children.type );

            for( TMapNodes::const_iterator
                it_generators = node_children->node_scope->generators.begin(),
                it_generators_end = node_children->node_scope->generators.end();
                it_generators != it_generators_end;
                ++it_generators )
            {
                const XmlNode * node_generator = it_generators->second;

                if( node_generator->inheritance != node_children->name )
                {
                    continue;
                }

                this->write( _ss ) << "    case " << node_generator->id << ":" << std::endl;
                this->write( _ss ) << "        {" << std::endl;
                this->write( _ss ) << "            " << node_generator->getScope() << " * metadata = new " << node_generator->getScope() << " ();" << std::endl;
                this->write( _ss ) << "            metadata->parse( _buff, _size, _read, _userData );" << std::endl;
                this->write( _ss ) << std::endl;
                this->write( _ss ) << "            children_" << node_children->getWriteName() << ".push_back(metadata);" << std::endl;
                this->write( _ss ) << "        }break;" << std::endl;
            }
        }

        this->write( _ss ) << "    default:" << std::endl;
        this->write( _ss ) << "        break;" << std::endl;
        this->write( _ss ) << "    }" << std::endl;

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node )
    {
        if( _node->generators.empty() == true || _node->inheritances.empty() == true )
        {
            return true;
        }

        this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write( _ss ) << "void " << _node->getScope() << "::_parseGenerators( const uint8_t * _buff, size_t _size, size_t & _read, uint32_t _id, void * _userData )" << std::endl;
        this->write( _ss ) << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseGenerators( _buff, _size, _read, _id, _userData );" << std::endl;

            if( _node->includes.empty() == false )
            {
                this->write( _ss ) << std::endl;
            }
        }

        if( _node->generators.empty() == false && _node->inheritances.empty() == false )
        {
            this->write( _ss ) << "    switch( _id )" << std::endl;
            this->write( _ss ) << "    {" << std::endl;

            for( TMapNodes::const_iterator
                it_inheritances = _node->inheritances.begin(),
                it_inheritances_end = _node->inheritances.end();
                it_inheritances != it_inheritances_end;
                ++it_inheritances )
            {
                const XmlNode * node_inheritance = it_inheritances->second;

                for( TMapNodes::const_iterator
                    it_generators = _node->generators.begin(),
                    it_generators_end = _node->generators.end();
                    it_generators != it_generators_end;
                    ++it_generators )
                {
                    const XmlNode * node_generator = it_generators->second;

                    if( node_generator->inheritance != node_inheritance->name )
                    {
                        continue;
                    }

                    this->write( _ss ) << "    case " << node_generator->id << ":" << std::endl;
                    this->write( _ss ) << "        {" << std::endl;
                    this->write( _ss ) << "            " << node_generator->getScope() << " * metadata = new " << node_generator->getScope() << " ();" << std::endl;
                    this->write( _ss ) << "            metadata->parse( _buff, _size, _read, _userData );" << std::endl;
                    this->write( _ss ) << std::endl;
                    this->write( _ss ) << "            includes_" << node_inheritance->getWriteName() << ".push_back(metadata);" << std::endl;
                    this->write( _ss ) << "        }break;" << std::endl;
                }
            }

            this->write( _ss ) << "    default:" << std::endl;
            this->write( _ss ) << "        break;" << std::endl;
            this->write( _ss ) << "    }" << std::endl;
        }

        this->write( _ss ) << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceIncludesDefinition_( std::stringstream & _ss, const XmlMeta * _meta, const XmlNode * _node )
    {
        for( TMapNodes::const_iterator
            it = _node->singles.begin(),
            it_end = _node->singles.end();
            it != it_end;
            ++it )
        {
            XmlNode * node_include = it->second;

            if( this->writeSourceNode_( _ss, _meta, node_include, false ) == false )
            {
                return false;
            }
        }

        for( TMapNodes::const_iterator
            it = _node->includes.begin(),
            it_end = _node->includes.end();
            it != it_end;
            ++it )
        {
            XmlNode * node_include = it->second;

            if( this->writeSourceNode_( _ss, _meta, node_include, false ) == false )
            {
                return false;
            }
        }

        for( TMapNodes::const_iterator
            it = _node->inheritances.begin(),
            it_end = _node->inheritances.end();
            it != it_end;
            ++it )
        {
            XmlNode * node_include = it->second;

            if( this->writeSourceNode_( _ss, _meta, node_include, false ) == false )
            {
                return false;
            }
        }

        for( TMapNodes::const_iterator
            it = _node->generators.begin(),
            it_end = _node->generators.end();
            it != it_end;
            ++it )
        {
            XmlNode * node_include = it->second;

            if( this->writeSourceNode_( _ss, _meta, node_include, false ) == false )
            {
                return false;
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    std::stringstream & Xml2Metacode::write( std::stringstream & _ss )
    {
        for( uint32_t i = 0; i != m_indent; ++i )
        {
            _ss << " ";
        }

        return _ss;
    }
}