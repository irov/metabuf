#	include "Xml2Metacode.hpp"
#	include "XmlProtocol.hpp"

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	Xml2Metacode::Xml2Metacode( XmlProtocol * _protocol )
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
	bool Xml2Metacode::generate( std::string & _header, std::string & _source )
	{
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
		this->write( _ss ) << "#   pragma once" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "#   include \"Metatype.h\"" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "#   include <metabuf/Metabuf.hpp>" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "namespace Metacode" << std::endl;
		this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "    uint32_t get_metacode_magic();" << std::endl;
		this->write( _ss ) << "    uint32_t get_metacode_version();" << std::endl;
		this->write( _ss ) << "    bool readHeader( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _readVersion, uint32_t & _needVersion );" << std::endl;
		this->write( _ss ) << "    bool readStrings( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _stringCount );" << std::endl;
		this->write( _ss ) << "    const char * readString( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _stringSize );" << std::endl;
		this->write( _ss ) << std::endl;

		m_indent += 4;

		const TMapNodes & nodes = m_protocol->getNodes();

		for( TMapNodes::const_iterator
			it = nodes.begin(),
			it_end = nodes.end();
		it != it_end;
		++it )
		{
			const XmlNode * node = it->second;

			if( this->writeHeaderNode_( _ss, node ) == false )
			{
				return false;
			}
		}

		m_indent -= 4;

		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderNode_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "class " << _node->getName() << std::endl;

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    : public " << node_inheritance->getName() << std::endl;
		}
		else
		{
			this->write( _ss ) << "    : public Metabuf::Metadata" << std::endl;
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

		if( this->writeHeaderIncludesPreparation_( _ss, _node ) == false )
		{
			return false;
		}

		if( this->writeHeaderIncludesReader_( _ss, _node ) == false )
		{
			return false;
		}

		if( this->writeHeaderGeneratorsReader_( _ss, _node ) == false )
		{
			return false;
		}

		m_indent -= 4;

		if( this->writeHeaderIncludesDefinition_( _ss, _node ) == false )
		{
			return false;
		}

		this->write( _ss ) << "protected:" << std::endl;
		if( this->writeHeaderAttribute_( _ss, _node ) == false )
		{
			return false;
		}

		if( this->writeHeaderIncludes_( _ss, _node ) == false )
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
		this->write( _ss ) << "    " << _node->getName() << "();" << std::endl;

		if( _node->inheritances.empty() == false )
		{
			this->write( _ss ) << "    ~" << _node->getName() << "();" << std::endl;
		}

		this->write( _ss ) << std::endl;
		this->write( _ss ) << "public:" << std::endl;
		this->write( _ss ) << "    uint32_t getId() const override;" << std::endl;
		this->write( _ss ) << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderDataReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		(void)_node;

		this->write( _ss ) << "void _parseData( const unsigned char * _buff, size_t _size, size_t & _read ) override;" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderAttributeReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		(void)_node;

		this->write( _ss ) << "void _parseArguments( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _id ) override;" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderAttributeSetup_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "public:" << std::endl;

		m_indent += 4;

		for( TMapAttributes::const_iterator
			it = _node->attributes.begin(),
			it_end = _node->attributes.end();
		it != it_end;
		++it )
		{
			const XmlAttribute * attr = &it->second;

			if( attr->required == false )
			{
				this->write( _ss ) << "bool" << " " << "has_" << attr->name << "() const" << std::endl;
				this->write( _ss ) << "{" << std::endl;
				this->write( _ss ) << "    return " << attr->name << "_successful;" << std::endl;
				this->write( _ss ) << "}" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "bool" << " " << "get_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
				this->write( _ss ) << "{" << std::endl;
				this->write( _ss ) << "    if( " << attr->name << "_successful == false )" << std::endl;
				this->write( _ss ) << "    {" << std::endl;
				this->write( _ss ) << "        return false;" << std::endl;
				this->write( _ss ) << "    }" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "    _value = this->" << attr->name << ";" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "    return true;" << std::endl;
				this->write( _ss ) << "}" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "bool" << " " << "swap_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
				this->write( _ss ) << "{" << std::endl;
				this->write( _ss ) << "    if( " << attr->name << "_successful == false )" << std::endl;
				this->write( _ss ) << "    {" << std::endl;
				this->write( _ss ) << "        return false;" << std::endl;
				this->write( _ss ) << "    }" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "    std::swap( _value, this->" << attr->name << ");" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "    return true;" << std::endl;
				this->write( _ss ) << "}" << std::endl;
				this->write( _ss ) << std::endl;
			}
			else
			{
				XmlType type;
				m_protocol->getType( attr->type, type );

				if( type.is_ncr == true || type.is_enumerator == true )
				{
					this->write( _ss ) << attr->type << " get_" << attr->name << "() const" << std::endl;
					this->write( _ss ) << "{" << std::endl;
					this->write( _ss ) << "    return this->" << attr->name << ";" << std::endl;
					this->write( _ss ) << "}" << std::endl;
				}
				else
				{
					this->write( _ss ) << "const " << attr->type << " & get_" << attr->name << "() const" << std::endl;
					this->write( _ss ) << "{" << std::endl;
					this->write( _ss ) << "    return this->" << attr->name << ";" << std::endl;
					this->write( _ss ) << "}" << std::endl;
				}

				this->write( _ss ) << std::endl;
				this->write( _ss ) << "void" << " " << "swap_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
				this->write( _ss ) << "{" << std::endl;
				this->write( _ss ) << "    std::swap( _value, this->" << attr->name << ");" << std::endl;
				this->write( _ss ) << "}" << std::endl;
				this->write( _ss ) << std::endl;
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
				it = member->attributes.begin(),
				it_end = member->attributes.end();
			it != it_end;
			++it )
			{
				const XmlAttribute * attr = &it->second;

				if( attr->required == false )
				{
					this->write( _ss ) << "bool" << " " << "has_" << member->name << "_" << attr->name << "() const" << std::endl;
					this->write( _ss ) << "{" << std::endl;
					this->write( _ss ) << "    return " << member->name << "_" << attr->name << "_successful;" << std::endl;
					this->write( _ss ) << "}" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "bool" << " " << "get_" << member->name << "_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
					this->write( _ss ) << "{" << std::endl;
					this->write( _ss ) << "    if( " << member->name << "_" << attr->name << "_successful == false )" << std::endl;
					this->write( _ss ) << "    {" << std::endl;
					this->write( _ss ) << "        return false;" << std::endl;
					this->write( _ss ) << "    }" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "    _value = this->" << member->name << "_" << attr->name << ";" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "    return true;" << std::endl;
					this->write( _ss ) << "}" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "bool" << " " << "swap_" << member->name << "_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
					this->write( _ss ) << "{" << std::endl;
					this->write( _ss ) << "    if( " << member->name << "_" << attr->name << "_successful == false )" << std::endl;
					this->write( _ss ) << "    {" << std::endl;
					this->write( _ss ) << "        return false;" << std::endl;
					this->write( _ss ) << "    }" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "    std::swap(_value, this->" << member->name << "_" << attr->name << ");" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "    return true;" << std::endl;
					this->write( _ss ) << "}" << std::endl;
					this->write( _ss ) << std::endl;
				}
				else
				{
					XmlType type;
					m_protocol->getType( attr->type, type );

					if( type.is_ncr == true || type.is_enumerator == true )
					{
						this->write( _ss ) << attr->type << " get_" << member->name << "_" << attr->name << "() const" << std::endl;
						this->write( _ss ) << "{" << std::endl;
						this->write( _ss ) << "    return this->" << member->name << "_" << attr->name << ";" << std::endl;
						this->write( _ss ) << "}" << std::endl;
					}
					else
					{
						this->write( _ss ) << "const " << attr->type << " & get_" << member->name << "_" << attr->name << "() const" << std::endl;
						this->write( _ss ) << "{" << std::endl;
						this->write( _ss ) << "    return this->" << member->name << "_" << attr->name << ";" << std::endl;
						this->write( _ss ) << "}" << std::endl;
					}

					this->write( _ss ) << std::endl;
					this->write( _ss ) << "void" << " " << "swap_" << member->name << "_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
					this->write( _ss ) << "{" << std::endl;
					this->write( _ss ) << "    std::swap(_value, this->" << member->name << "_" << attr->name << ");" << std::endl;
					this->write( _ss ) << "}" << std::endl;
					this->write( _ss ) << std::endl;
				}
			}
		}

		m_indent -= 4;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludesDefinition_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "public:" << std::endl;

		m_indent += 4;

		for( TMapNodes::const_iterator
			it = _node->includes.begin(),
			it_end = _node->includes.end();
		it != it_end;
		++it )
		{
			XmlNode * node_include = it->second;

			this->writeHeaderNode_( _ss, node_include );
		}

		for( TMapNodes::const_iterator
			it = _node->inheritances.begin(),
			it_end = _node->inheritances.end();
		it != it_end;
		++it )
		{
			XmlNode * node_inheritance = it->second;

			this->writeHeaderNode_( _ss, node_inheritance );
		}

		for( TMapNodes::const_iterator
			it = _node->generators.begin(),
			it_end = _node->generators.end();
		it != it_end;
		++it )
		{
			XmlNode * node_generator = it->second;

			this->writeHeaderNode_( _ss, node_generator );
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
			it = _node->attributes.begin(),
			it_end = _node->attributes.end();
		it != it_end;
		++it )
		{
			const XmlAttribute * attr = &it->second;

			if( attr->required == false )
			{
				this->write( _ss ) << "bool" << " " << attr->name << "_successful;" << std::endl;
			}

			this->write( _ss ) << "mutable " << attr->type << " " << attr->name << ";" << std::endl;
		}

		for( TMapMembers::const_iterator
			it = _node->members.begin(),
			it_end = _node->members.end();
		it != it_end;
		++it )
		{
			const XmlMember * member = &it->second;

			for( TMapAttributes::const_iterator
				it = member->attributes.begin(),
				it_end = member->attributes.end();
			it != it_end;
			++it )
			{
				const XmlAttribute * attr = &it->second;

				if( attr->required == false )
				{
					this->write( _ss ) << "bool" << " " << member->name << "_" << attr->name << "_successful" << ";" << std::endl;
				}

				this->write( _ss ) << "mutable " << attr->type << " " << member->name << "_" << attr->name << ";" << std::endl;
			}
		}

		m_indent -= 4;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludes_( std::stringstream & _ss, const XmlNode * _node )
	{
		for( TMapNodes::const_iterator
			it = _node->includes.begin(),
			it_end = _node->includes.end();
		it != it_end;
		++it )
		{
			const XmlNode * node_include = it->second;

			this->write( _ss ) << "public:" << std::endl;

			this->write( _ss ) << "    typedef stdex::auto_array<" << node_include->getName() << "> TVector" << node_include->getName() << ";" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "    const TVector" << node_include->getName() << " & " << "get_Includes" << node_include->name << "() const" << std::endl;
			this->write( _ss ) << "    {" << std::endl;
			this->write( _ss ) << "        return this->includes_" << node_include->getName() << ";" << std::endl;
			this->write( _ss ) << "    }" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "protected:" << std::endl;
			this->write( _ss ) << "    TVector" << node_include->getName() << " includes_" << node_include->getName() << ";" << std::endl;
		}

		for( TMapNodes::const_iterator
			it = _node->inheritances.begin(),
			it_end = _node->inheritances.end();
		it != it_end;
		++it )
		{
			const XmlNode * node_include = it->second;

			this->write( _ss ) << "public:" << std::endl;
			this->write( _ss ) << "    typedef stdex::auto_array<" << node_include->getName() << " *> TVector" << node_include->getName() << ";" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "    const TVector" << node_include->getName() << " & " << "get_Includes" << node_include->name << "() const" << std::endl;
			this->write( _ss ) << "    {" << std::endl;
			this->write( _ss ) << "        return this->includes_" << node_include->getName() << ";" << std::endl;
			this->write( _ss ) << "    }" << std::endl;
			this->write( _ss ) << std::endl;
			this->write( _ss ) << "protected:" << std::endl;
			this->write( _ss ) << "    TVector" << node_include->getName() << " includes_" << node_include->getName() << ";" << std::endl;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node )
	{
		(void)_node;

		this->write( _ss ) << "void _preparationIncludes( uint32_t _includes, uint32_t _count ) override;" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludesReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		(void)_node;

		this->write( _ss ) << "void _parseIncludes( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _includes ) override;" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		(void)_node;

		this->write( _ss ) << "void _parseGenerators( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _generators ) override;" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::generateSource( std::stringstream & _ss )
	{
		int version = m_protocol->getVersion();

		this->write( _ss ) << "#   include \"Metacode.h\"" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "namespace Metacode" << std::endl;
		this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    static const uint32_t metacode_magic = 3133062829u;" << std::endl;
		this->write( _ss ) << "    static const uint32_t metacode_version = " << version << ";" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    uint32_t get_metacode_magic()" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        return metacode_magic;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    uint32_t get_metacode_version()" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        return metacode_version;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    static bool readHeader2( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _readVersion, uint32_t & _needVersion )" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        stdex::memory_reader ar(_buff, _size, _read);" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        uint32_t head;" << std::endl;
		this->write( _ss ) << "        ar.readPOD( head );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        if( head != metacode_magic )" << std::endl;
		this->write( _ss ) << "        {" << std::endl;
		this->write( _ss ) << "            return false;" << std::endl;
		this->write( _ss ) << "        }" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        uint32_t version;" << std::endl;
		this->write( _ss ) << "        ar.readPOD( version );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        _readVersion = version;" << std::endl;
		this->write( _ss ) << "        _needVersion = metacode_version;" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        if( version != metacode_version )" << std::endl;
		this->write( _ss ) << "        {" << std::endl;
		this->write( _ss ) << "            return false;" << std::endl;
		this->write( _ss ) << "        }" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        return true;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    bool readHeader( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _readVersion, uint32_t & _needVersion )" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        bool successful = readHeader2( _buff, _size, _read, _readVersion, _needVersion );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        return successful;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    static bool readStrings2( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _stringCount )" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        stdex::memory_reader ar(_buff, _size, _read);" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        uint32_t count;" << std::endl;
		this->write( _ss ) << "        ar.readPOD( count );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        _stringCount = count;" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        return true;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    bool readStrings( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _stringCount )" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        bool successful = readStrings2( _buff, _size, _read, _stringCount );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        return successful;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    static const char * readString2( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _stringSize )" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        stdex::memory_reader ar(_buff, _size, _read);" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        uint32_t size;" << std::endl;
		this->write( _ss ) << "        ar.readSize( size );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        const char * value = ar.current_buff<char>();" << std::endl;
		this->write( _ss ) << "        ar.skip( size );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        _stringSize = size;" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        return value;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "    const char * readString( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t & _stringSize )" << std::endl;
		this->write( _ss ) << "    {" << std::endl;
		this->write( _ss ) << "        const char * value = readString2( _buff, _size, _read, _stringSize );" << std::endl;
		this->write( _ss ) << std::endl;
		this->write( _ss ) << "        return value;" << std::endl;
		this->write( _ss ) << "    }" << std::endl;
		this->write( _ss ) << "    //////////////////////////////////////////////////////////////////////////" << std::endl;

		m_indent += 4;

		const TMapNodes & nodes = m_protocol->getNodes();

		for( TMapNodes::const_iterator
			it = nodes.begin(),
			it_end = nodes.end();
		it != it_end;
		++it )
		{
			const XmlNode * node = it->second;

			if( this->writeSourceNode_( _ss, node ) == false )
			{
				return false;
			}
		}

		m_indent -= 4;

		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceNode_( std::stringstream & _ss, const XmlNode * _node )
	{
		if( this->writeSourceConstructor_( _ss, _node ) == false )
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

		if( writeSourceIncludesPreparation_( _ss, _node ) == false )
		{
			return false;
		}

		if( this->writeSourceIncludesReader_( _ss, _node ) == false )
		{
			return false;
		}

		if( this->writeSourceGeneratorsReader_( _ss, _node ) == false )
		{
			return false;
		}

		if( this->writeSourceIncludesDefinition_( _ss, _node ) == false )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceConstructor_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << _node->getScope() << "::" << _node->getName() << "()" << std::endl;

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    : " << node_inheritance->getName() << "()" << std::endl;
		}
		else
		{
			this->write( _ss ) << "    : Metabuf::Metadata()" << std::endl;
		}

		for( TMapAttributes::const_iterator
			it = _node->attributes.begin(),
			it_end = _node->attributes.end();
		it != it_end;
		++it )
		{
			const XmlAttribute * attr = &it->second;

			if( attr->required == false )
			{
				this->write( _ss ) << "    , " << attr->name << "_successful(false)" << std::endl;
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
				it = member->attributes.begin(),
				it_end = member->attributes.end();
			it != it_end;
			++it )
			{
				const XmlAttribute * attr = &it->second;

				if( attr->required == false )
				{
					this->write( _ss ) << "    , " << member->name << "_" << attr->name << "_successful(false)" << std::endl;
				}
			}
		}

		this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "}" << std::endl;

		if( _node->inheritances.empty() == false )
		{
			this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
			this->write( _ss ) << _node->getScope() << "::~" << _node->getName() << "()" << std::endl;
			this->write( _ss ) << "{" << std::endl;

			for( TMapNodes::const_iterator
				it = _node->inheritances.begin(),
				it_end = _node->inheritances.end();
			it != it_end;
			++it )
			{
				const XmlNode * node_include = it->second;

				this->write( _ss ) << "    for( TVector" << node_include->getName() << "::const_iterator" << std::endl;
				this->write( _ss ) << "        it = includes_" << node_include->getName() << ".begin()," << std::endl;
				this->write( _ss ) << "        it_end = includes_" << node_include->getName() << ".end();" << std::endl;
				this->write( _ss ) << "    it != it_end;" << std::endl;
				this->write( _ss ) << "    ++it )" << std::endl;
				this->write( _ss ) << "    {" << std::endl;
				this->write( _ss ) << "        delete *it;" << std::endl;
				this->write( _ss ) << "    }" << std::endl;
			}

			this->write( _ss ) << "}" << std::endl;
		}

		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "uint32_t " << _node->getScope() << "::getId() const" << std::endl;
		this->write( _ss ) << "{" << std::endl;
		this->write( _ss ) << "    return " << _node->id << ";" << std::endl;
		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::hasNodeDataSize_( const XmlNode * _node )
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

			return true;
		}

		for( TMapMembers::const_iterator
			it = _node->members.begin(),
			it_end = _node->members.end();
		it != it_end;
		++it )
		{
			const XmlMember * member = &it->second;

			for( TMapAttributes::const_iterator
				it = member->attributes.begin(),
				it_end = member->attributes.end();
			it != it_end;
			++it )
			{
				const XmlAttribute * attr = &it->second;

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

			return true;
		}

		for( TMapMembers::const_iterator
			it = _node->members.begin(),
			it_end = _node->members.end();
		it != it_end;
		++it )
		{
			const XmlMember * member = &it->second;

			for( TMapAttributes::const_iterator
				it = member->attributes.begin(),
				it_end = member->attributes.end();
			it != it_end;
			++it )
			{
				const XmlAttribute * attr = &it->second;

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
	bool Xml2Metacode::writeSourceDataReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "void " << _node->getScope() << "::_parseData( const unsigned char * _buff, size_t _size, size_t & _read )" << std::endl;
		this->write( _ss ) << "{" << std::endl;
		
		if( _node->inheritance.empty() == true && this->hasNodeDataSize_( _node ) == false )
		{
			this->write( _ss ) << "    (void)_buff;" << std::endl;
			this->write( _ss ) << "    (void)_size;" << std::endl;
			this->write( _ss ) << "    (void)_read;" << std::endl;
			this->write( _ss ) << "}" << std::endl;
			this->write( _ss ) << std::endl;

			return true;
		}

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseData( _buff, _size, _read );" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( this->hasNodeDataSize_( _node ) == true )
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

				this->write( _ss ) << "    this->read( _buff, _size, _read, this->" << attr->name << " );" << std::endl;
			}

			for( TMapMembers::const_iterator
				it = _node->members.begin(),
				it_end = _node->members.end();
			it != it_end;
			++it )
			{
				const XmlMember * member = &it->second;

				for( TMapAttributes::const_iterator
					it = member->attributes.begin(),
					it_end = member->attributes.end();
				it != it_end;
				++it )
				{
					const XmlAttribute * attr = &it->second;

					if( attr->required == false )
					{
						continue;
					}

					this->write( _ss ) << "    this->read( _buff, _size, _read, this->" << member->name << "_" << attr->name << " );" << std::endl;
				}
			}
		}

		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceAttributeReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "void " << _node->getScope() << "::_parseArguments( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _id )" << std::endl;
		this->write( _ss ) << "{" << std::endl;

		if( _node->inheritance.empty() == true && this->hasNodeAttributeSize_( _node ) == false )
		{
			this->write( _ss ) << "    (void)_buff;" << std::endl;
			this->write( _ss ) << "    (void)_size;" << std::endl;
			this->write( _ss ) << "    (void)_read;" << std::endl;
			this->write( _ss ) << "    (void)_id;" << std::endl;
			this->write( _ss ) << "}" << std::endl;
			this->write( _ss ) << std::endl;

			return true;
		}

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseArguments( _buff, _size, _read, _id );" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( this->hasNodeAttributeSize_( _node ) == true )
		{
			this->write( _ss ) << "    switch( _id )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;

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

				this->write( _ss ) << "    case " << attr->id << ":" << std::endl;
				this->write( _ss ) << "        {" << std::endl;
				this->write( _ss ) << "            this->read( _buff, _size, _read, this->" << attr->name << " );" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "            this->" << attr->name << "_successful = true;" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "        }break;" << std::endl;
			}

			for( TMapMembers::const_iterator
				it = _node->members.begin(),
				it_end = _node->members.end();
			it != it_end;
			++it )
			{
				const XmlMember * member = &it->second;

				for( TMapAttributes::const_iterator
					it = member->attributes.begin(),
					it_end = member->attributes.end();
				it != it_end;
				++it )
				{
					const XmlAttribute * attr = &it->second;

					if( attr->required == true )
					{
						continue;
					}

					this->write( _ss ) << "    case " << attr->id << ":" << std::endl;
					this->write( _ss ) << "        {" << std::endl;
					this->write( _ss ) << "            this->read( _buff, _size, _read, this->" << member->name << "_" << attr->name << " );" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "            this->" << member->name << "_" << attr->name << "_successful = true;" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "        }break;" << std::endl;
				}
			}

			this->write( _ss ) << "    }" << std::endl;
		}

		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceIncludesPreparation_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "void " << _node->getScope() << "::_preparationIncludes( uint32_t _includes, uint32_t _count )" << std::endl;
		this->write( _ss ) << "{" << std::endl;

		if( _node->inheritance.empty() == true && _node->includes.empty() == true && _node->inheritances.empty() == true )
		{
			this->write( _ss ) << "    (void)_includes;" << std::endl;
			this->write( _ss ) << "    (void)_count;" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    " << node_inheritance->getScope() << "::_preparationIncludes( _includes, _count );" << std::endl;

			if( _node->includes.empty() == false )
			{
				this->write( _ss ) << std::endl;
			}
		}

		if( _node->includes.empty() == false || _node->inheritances.empty() == false )
		{
			this->write( _ss ) << "    switch( _includes )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;

			for( TMapNodes::const_iterator
				it = _node->includes.begin(),
				it_end = _node->includes.end();
			it != it_end;
			++it )
			{
				const XmlNode * node = it->second;

				this->write( _ss ) << "    case " << node->id << ":" << std::endl;
				this->write( _ss ) << "        {" << std::endl;
				this->write( _ss ) << "            includes_" << node->getName() << ".reserve( _count );" << std::endl;
				this->write( _ss ) << "        }break;" << std::endl;
			}

			for( TMapNodes::const_iterator
				it = _node->inheritances.begin(),
				it_end = _node->inheritances.end();
			it != it_end;
			++it )
			{
				const XmlNode * node = it->second;

				this->write( _ss ) << "    case " << node->id << ":" << std::endl;
				this->write( _ss ) << "        {" << std::endl;
				this->write( _ss ) << "            includes_" << node->getName() << ".reserve( _count );" << std::endl;
				this->write( _ss ) << "        }break;" << std::endl;
			}

			this->write( _ss ) << "    }" << std::endl;
		}

		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceIncludesReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "void " << _node->getScope() << "::_parseIncludes( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _includes )" << std::endl;
		this->write( _ss ) << "{" << std::endl;

		if( _node->inheritance.empty() == true && _node->includes.empty() == true )
		{
			this->write( _ss ) << "    (void)_buff;" << std::endl;
			this->write( _ss ) << "    (void)_size;" << std::endl;
			this->write( _ss ) << "    (void)_read;" << std::endl;
			this->write( _ss ) << "    (void)_includes;" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseIncludes( _buff, _size, _read, _includes );" << std::endl;

			if( _node->includes.empty() == false )
			{
				this->write( _ss ) << std::endl;
			}
		}

		if( _node->includes.empty() == false )
		{
			this->write( _ss ) << "    switch( _includes )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;

			for( TMapNodes::const_iterator
				it = _node->includes.begin(),
				it_end = _node->includes.end();
			it != it_end;
			++it )
			{
				const XmlNode * node = it->second;

				this->write( _ss ) << "    case " << node->id << ":" << std::endl;
				this->write( _ss ) << "        {" << std::endl;
				this->write( _ss ) << "            " << node->getScope() << " & metadata = includes_" << node->getName() << ".emplace_back();" << std::endl;
				this->write( _ss ) << std::endl;
				this->write( _ss ) << "            metadata.parse( _buff, _size, _read, m_userData );" << std::endl;
				this->write( _ss ) << "        }break;" << std::endl;
			}

			this->write( _ss ) << "    }" << std::endl;
		}

		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceGeneratorsReader_( std::stringstream & _ss, const XmlNode * _node )
	{
		this->write( _ss ) << "//////////////////////////////////////////////////////////////////////////" << std::endl;
		this->write( _ss ) << "void " << _node->getScope() << "::_parseGenerators( const unsigned char * _buff, size_t _size, size_t & _read, uint32_t _generators )" << std::endl;
		this->write( _ss ) << "{" << std::endl;

		if( _node->inheritance.empty() == true && _node->generators.empty() == true )
		{
			this->write( _ss ) << "    (void)_buff;" << std::endl;
			this->write( _ss ) << "    (void)_size;" << std::endl;
			this->write( _ss ) << "    (void)_read;" << std::endl;
			this->write( _ss ) << "    (void)_generators;" << std::endl;
			this->write( _ss ) << std::endl;
		}

		if( _node->inheritance.empty() == false )
		{
			const XmlNode * node_inheritance = _node->node_inheritance;

			this->write( _ss ) << "    " << node_inheritance->getScope() << "::_parseGenerators( _buff, _size, _read, _generators );" << std::endl;

			if( _node->includes.empty() == false )
			{
				this->write( _ss ) << std::endl;
			}
		}

		if( _node->generators.empty() == false )
		{
			this->write( _ss ) << "    switch( _generators )" << std::endl;
			this->write( _ss ) << "    {" << std::endl;

			for( TMapNodes::const_iterator
				it = _node->inheritances.begin(),
				it_end = _node->inheritances.end();
			it != it_end;
			++it )
			{
				const XmlNode * node_inheritance = it->second;

				for( TMapNodes::const_iterator
					it = _node->generators.begin(),
					it_end = _node->generators.end();
				it != it_end;
				++it )
				{
					const XmlNode * node_generator = it->second;

					if( node_generator->inheritance != node_inheritance->name )
					{
						continue;
					}

					this->write( _ss ) << "    case " << node_generator->id << ":" << std::endl;
					this->write( _ss ) << "        {" << std::endl;
					this->write( _ss ) << "            " << node_generator->getScope() << " * metadata = new " << node_generator->getScope() << " ();" << std::endl;
					this->write( _ss ) << "            metadata->parse( _buff, _size, _read, m_userData );" << std::endl;
					this->write( _ss ) << std::endl;
					this->write( _ss ) << "            includes_" << node_inheritance->getName() << ".push_back(metadata);" << std::endl;
					this->write( _ss ) << "        }break;" << std::endl;
				}
			}

			this->write( _ss ) << "    }" << std::endl;
		}

		this->write( _ss ) << "}" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeSourceIncludesDefinition_( std::stringstream & _ss, const XmlNode * _node )
	{
		for( TMapNodes::const_iterator
			it = _node->includes.begin(),
			it_end = _node->includes.end();
		it != it_end;
		++it )
		{
			XmlNode * node_include = it->second;

			if( this->writeSourceNode_( _ss, node_include ) == false )
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

			if( this->writeSourceNode_( _ss, node_include ) == false )
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

			if( this->writeSourceNode_( _ss, node_include ) == false )
			{
				return false;
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	std::stringstream & Xml2Metacode::write( std::stringstream & _ss )
	{
		for( size_t i = 0; i != m_indent; ++i )
		{
			_ss << " ";
		}

		return _ss;
	}
}