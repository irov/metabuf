#	include "Xml2Metacode.hpp"
#	include "XmlProtocol.hpp"

namespace Metabuf
{
	//////////////////////////////////////////////////////////////////////////
	Xml2Metacode::Xml2Metacode( XmlProtocol * _protocol )
		: m_protocol(_protocol)
		, m_indent(0)
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
        std::stringstream ss_header;
        m_out.swap(ss_header);
        if( this->generateHeader( _header ) == false )
        {
            return false;
        }

        _header = m_out.str();

        std::stringstream ss_source;
        
        m_out.swap(ss_source);

        if( this->generateSource( _header ) == false )
        {
            return false;
        }

        _source = m_out.str();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::generateHeader( std::string & _header )
    {
        this->write() << "#   pragma once" << std::endl;
        this->write() << std::endl;
        this->write() << "#   include <metabuf/Metabuf.hpp>" << std::endl;
        this->write() << std::endl;
        this->write() << "#   include \"Metatype.h\"" << std::endl;
        this->write() << std::endl;
        this->write() << "#   include <vector>" << std::endl;
        this->write() << "#   include <algorithm>" << std::endl;
        this->write() << std::endl;
        this->write() << "namespace Metacode" << std::endl;
        this->write() << "{" << std::endl;

        m_indent += 4;

        const TMapNodes & nodes = m_protocol->getNodes();

        for( TMapNodes::const_iterator
            it = nodes.begin(),
            it_end = nodes.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node = it->second;

            if( this->writeHeaderNode_( node ) == false )
            {
                return false;
            }
        }

        m_indent -= 4;

        this->write() << "}" << std::endl;
        
        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderNode_( const XmlNode * _node )
	{
		this->write() << "class " << _node->getName() << std::endl;
                
		if( _node->inheritance.empty() == false )
		{
            const XmlNode * node_inheritance = _node->node_inheritance;

			this->write() << "    : public " << node_inheritance->getName() << std::endl;
		}
		else
		{
			this->write() << "    : public Metabuf::Metadata" << std::endl;
		}

		this->write() << "{ " << std::endl;

		//size_t id = _node->id;
		//s_writeStream( _stream, id );

		if( this->writeHeaderConstructor_( _node ) == false )
		{
			return false;
		}

		if( this->writeHeaderAttributeSetup_( _node ) == false )
		{
			return false;
		}

        this->write() << "protected:" << std::endl;

        m_indent += 4;

		if( this->writeHeaderAttributeReader_( _node ) == false )
		{
			return false;
		}

        if( this->writeHeaderIncludesPreparation_( _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderIncludesReader_( _node ) == false )
        {
            return false;
        }

        if( this->writeHeaderGeneratorsReader_( _node ) == false )
        {
            return false;
        }

        m_indent -= 4;

        if( this->writeHeaderIncludesDefinition_( _node ) == false )
        {
            return false;
        }

        this->write() << "protected:" << std::endl;
		if( this->writeHeaderAttribute_( _node ) == false )
		{
			return false;
		}
        
		if( this->writeHeaderIncludes_( _node ) == false )
		{
			return false;
		}

		this->write() << "};" << std::endl;
		this->write() << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderConstructor_( const XmlNode * _node )
	{
		this->write() << "public:" << std::endl;
		
		this->write() << "    " << _node->getName() << "()" << std::endl;

		if( _node->inheritance.empty() == false )
		{
            const XmlNode * node_inheritance = _node->node_inheritance;

			this->write() << "        : " << node_inheritance->getName() << "()" << std::endl;
		}
		else
		{
			this->write() << "        : Metabuf::Metadata()" << std::endl;
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
				this->write() << "        , " << attr->name << "_successful(false)" << std::endl;
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
					this->write() << "        , " << member->name << "_" << attr->name << "_successful(false)" << std::endl;
				}
			}
		}

		this->write() << "    {" << std::endl;
		this->write() << "    }" << std::endl;

        if( _node->inheritances.empty() == false )
        {
            this->write() << "    ~" << _node->getName() << "()" << std::endl;
            this->write() << "    {" << std::endl;

            for( TMapNodes::const_iterator
                it = _node->inheritances.begin(),
                it_end = _node->inheritances.end();
            it != it_end;
            ++it )
            {
                const XmlNode * node_include = it->second;

                this->write() << "        for( TVector" << node_include->getName() << "::const_iterator" << std::endl;
                this->write() << "            it = includes_" << node_include->getName() << ".begin()," << std::endl;
                this->write() << "            it_end = includes_" << node_include->getName() << ".end();" << std::endl;
                this->write() << "        it != it_end;" << std::endl;
                this->write() << "        ++it )" << std::endl;
                this->write() << "        {" << std::endl;
                this->write() << "            delete *it;" << std::endl;
                this->write() << "        }" << std::endl;
            }

            this->write() << "    }" << std::endl;
        }
        else if( _node->generator.empty() == false )
        {
            this->write() << "    virtual ~" << _node->getName() << "()" << std::endl;
            this->write() << "    {" << std::endl;
            this->write() << "    }" << std::endl;
        }
		
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderAttributeReader_( const XmlNode * _node )
	{
		this->write() << "bool _parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id ) override;" << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderAttributeSetup_( const XmlNode * _node )
	{
		this->write() << "public:" << std::endl;

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
		    	this->write() << "bool" << " " << "get_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
    			this->write() << "{" << std::endl;	    	
				this->write() << "    if( " << attr->name << "_successful == false )" << std::endl;
				this->write() << "    {" << std::endl;
				this->write() << "        return false;" << std::endl;
				this->write() << "    }" << std::endl;
				this->write() << std::endl;
                this->write() << "    _value = this->" << attr->name << ";" << std::endl;
                this->write() << std::endl;
                this->write() << "    return true;" << std::endl;
                this->write() << "}" << std::endl;
                this->write() << std::endl;
                this->write() << "bool" << " " << "swap_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
                this->write() << "{" << std::endl;	    	
                this->write() << "    if( " << attr->name << "_successful == false )" << std::endl;
                this->write() << "    {" << std::endl;
                this->write() << "        return false;" << std::endl;
                this->write() << "    }" << std::endl;
                this->write() << std::endl;
                this->write() << "    std::swap( _value, this->" << attr->name << ");" << std::endl;
                this->write() << std::endl;
                this->write() << "    return true;" << std::endl;
                this->write() << "}" << std::endl;
                this->write() << std::endl;
			}
            else
            {
                this->write() << "const " << attr->type << " & get_" << attr->name << "() const" << std::endl;
                this->write() << "{" << std::endl;	    	
                this->write() << "    return this->" << attr->name << ";" << std::endl;
                this->write() << "}" << std::endl;
                this->write() << std::endl;
                this->write() << "void" << " " << "swap_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
                this->write() << "{" << std::endl;
                this->write() << "    std::swap( _value, this->" << attr->name << ");" << std::endl;
                this->write() << "}" << std::endl;
                this->write() << std::endl;
            }


			this->write() << "template<class C, class M>" << std::endl;
			this->write() << "void" << " " << "method_" << attr->name << "( C * _self, M _method ) const" << std::endl;
			this->write() << "{" << std::endl;

			if( attr->required == false )
			{
				this->write() << "    if( " << attr->name << "_successful == false )" << std::endl;
				this->write() << "    {" << std::endl;
				this->write() << "        return;" << std::endl;
				this->write() << "    }" << std::endl;
				this->write() << std::endl;
			}

			this->write() << "    (_self->*_method)( this->" << attr->name << " );" << std::endl;
			this->write() << "}" << std::endl;

			this->write() << std::endl;
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
				    this->write() << "bool" << " " << "get_" << member->name << "_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
				    this->write() << "{" << std::endl;
					this->write() << "    if( " << member->name << "_" << attr->name << "_successful == false )" << std::endl;
					this->write() << "    {" << std::endl;
					this->write() << "        return false;" << std::endl;
					this->write() << "    }" << std::endl;
					this->write() << std::endl;
                    this->write() << "    _value = this->" << member->name << "_" << attr->name << ";" << std::endl;
                    this->write() << std::endl;
                    this->write() << "    return true;" << std::endl;
                    this->write() << "}" << std::endl;
                    this->write() << std::endl;
                    this->write() << "bool" << " " << "swap_" << member->name << "_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
                    this->write() << "{" << std::endl;
                    this->write() << "    if( " << member->name << "_" << attr->name << "_successful == false )" << std::endl;
                    this->write() << "    {" << std::endl;
                    this->write() << "        return false;" << std::endl;
                    this->write() << "    }" << std::endl;
                    this->write() << std::endl;
                    this->write() << "    std::swap(_value, this->" << member->name << "_" << attr->name << ");" << std::endl;
                    this->write() << std::endl;
                    this->write() << "    return true;" << std::endl;
                    this->write() << "}" << std::endl;
                    this->write() << std::endl;
				}
                else
                {
                    this->write() << "const " << attr->type << " & get_" << member->name << "_" << attr->name << "() const" << std::endl;
                    this->write() << "{" << std::endl;
                    this->write() << "    return this->" << member->name << "_" << attr->name << ";" << std::endl;
                    this->write() << "}" << std::endl;
                    this->write() << std::endl;
                    this->write() << "void" << " " << "swap_" << member->name << "_" << attr->name << "( " << attr->type << " & _value ) const" << std::endl;
                    this->write() << "{" << std::endl;
                    this->write() << "    std::swap(_value, this->" << member->name << "_" << attr->name << ");" << std::endl;
                    this->write() << "}" << std::endl;
                    this->write() << std::endl;
                }


				this->write() << "template<class C, class M>" << std::endl;
				this->write() << "void" << " " << "method_" << member->name << "_" << attr->name << "( C * _self, M _method )" << std::endl;
				this->write() << "{" << std::endl;

				if( attr->required == false )
				{
					this->write() << "    if( " << member->name << "_" << attr->name << "_successful == false )" << std::endl;
					this->write() << "    {" << std::endl;
					this->write() << "        return;" << std::endl;
					this->write() << "    }" << std::endl;
					this->write() << std::endl;
				}

				this->write() << "    (_self->*_method)( this->" << member->name << "_" << attr->name << " );" << std::endl;
				this->write() << "}" << std::endl;
				this->write() << std::endl;
			}
		}

		m_indent -= 4;

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderIncludesDefinition_( const XmlNode * _node )
    {
        this->write() << "public:" << std::endl;
        
        m_indent += 4;

        for( TMapNodes::const_iterator
            it = _node->includes.begin(),
            it_end = _node->includes.end();
        it != it_end;
        ++it )
        {
            XmlNode * node_include = it->second;

            this->writeHeaderNode_( node_include );
        }

        for( TMapNodes::const_iterator
            it = _node->inheritances.begin(),
            it_end = _node->inheritances.end();
        it != it_end;
        ++it )
        {
            XmlNode * node_inheritance = it->second;

            this->writeHeaderNode_( node_inheritance );
        }

        for( TMapNodes::const_iterator
            it = _node->generators.begin(),
            it_end = _node->generators.end();
        it != it_end;
        ++it )
        {
            XmlNode * node_generator = it->second;

            this->writeHeaderNode_( node_generator );
        }
        
        m_indent -= 4;

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderAttribute_( const XmlNode * _node )
	{
		this->write() << "protected:" << std::endl;

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
				this->write() << "bool" << " " << attr->name << "_successful;" << std::endl;
			}

			this->write() << "mutable " << attr->type << " " << attr->name << ";" << std::endl;
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
					this->write() << "bool" << " " << member->name << "_" << attr->name << "_successful" << ";" << std::endl;
				}

				this->write() << "mutable " << attr->type << " " << member->name << "_" << attr->name << ";" << std::endl;
			}
		}

		m_indent -= 4;

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludes_( const XmlNode * _node )
	{
		for( TMapNodes::const_iterator
			it = _node->includes.begin(),
			it_end = _node->includes.end();
		it != it_end;
		++it )
		{
            const XmlNode * node_include = it->second;

            this->write() << "public:" << std::endl;

            this->write() << "    typedef std::vector<" << node_include->getName() << "> TVector" << node_include->getName() << ";" << std::endl;
            this->write() << std::endl;
            this->write() << "    const TVector" << node_include->getName() << " & " << "get_Includes" << node_include->name << "() const" << std::endl;
            this->write() << "    {" << std::endl;
            this->write() << "        return this->includes_" << node_include->getName() << ";" << std::endl;
            this->write() << "    }" << std::endl;
            this->write() << std::endl;
            this->write() << "protected:" << std::endl;
            this->write() << "    TVector" << node_include->getName() << " includes_" << node_include->getName() << ";" << std::endl;
		}

        for( TMapNodes::const_iterator
            it = _node->inheritances.begin(),
            it_end = _node->inheritances.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node_include = it->second;

            this->write() << "public:" << std::endl;
            this->write() << "    typedef std::vector<" << node_include->getName() << " *> TVector" << node_include->getName() << ";" << std::endl;
            this->write() << std::endl;
            this->write() << "    const TVector" << node_include->getName() << " & " << "get_Includes" << node_include->name << "() const" << std::endl;
            this->write() << "    {" << std::endl;
            this->write() << "        return this->includes_" << node_include->getName() << ";" << std::endl;
            this->write() << "    }" << std::endl;
            this->write() << std::endl;
            this->write() << "protected:" << std::endl;
            this->write() << "    TVector" << node_include->getName() << " includes_" << node_include->getName() << ";" << std::endl;
        }

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderIncludesPreparation_( const XmlNode * _node )
    {
        this->write() << "bool _preparationIncludes( size_t _includes, size_t _count ) override;" << std::endl;

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludesReader_( const XmlNode * _node )
	{
		this->write() << "bool _parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes ) override;" << std::endl;

        return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeHeaderGeneratorsReader_( const XmlNode * _node )
    {
        this->write() << "bool _parseGenerators( const char * _buff, size_t _size, size_t & _read, size_t _generators ) override;" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::generateSource( std::string & _header )
    {        
        this->write() << "#   include \"Metacode.h\"" << std::endl;
        this->write() << std::endl;
        this->write() << "namespace Metacode" << std::endl;
        this->write() << "{" << std::endl;

        m_indent += 4;

        const TMapNodes & nodes = m_protocol->getNodes();

        for( TMapNodes::const_iterator
            it = nodes.begin(),
            it_end = nodes.end();
        it != it_end;
        ++it )
        {
            const XmlNode * node = it->second;

            if( this->writeSourceNode_( node ) == false )
            {
                return false;
            }
        }

        m_indent -= 4;

        this->write() << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceNode_( const XmlNode * _node )
    {
        if( this->writeSourceAttributeReader_( _node ) == false )
        {
            return false;
        }

        if( writeSourceIncludesPreparation_( _node ) == false )
        {
            return false;
        }

        if( this->writeSourceIncludesReader_( _node ) == false )
        {
            return false;
        }

        if( this->writeSourceGeneratorsReader_( _node ) == false )
        {
            return false;
        }
        
        if( this->writeSourceIncludesDefinition_( _node ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceAttributeReader_( const XmlNode * _node )
    {
        this->write() << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write() << "bool " << _node->getScope() << "::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )" << std::endl;
        this->write() << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write() << "    if( " << node_inheritance->getScope() << "::_parseArguments( _buff, _size, _read, _id ) == true )" << std::endl;
            this->write() << "    {" << std::endl;
            this->write() << "        return true;" << std::endl;
            this->write() << "    }" << std::endl;
            this->write() << std::endl;
        }

        if( _node->attributes.empty() == false || _node->members.empty() == false )
        {
            this->write() << "    switch( _id )" << std::endl;
            this->write() << "    {" << std::endl;

            for( TMapAttributes::const_iterator
                it = _node->attributes.begin(),
                it_end = _node->attributes.end();
            it != it_end;
            ++it )
            {
                const XmlAttribute * attr = &it->second;

                this->write() << "    case " << attr->id << ":" << std::endl;
                this->write() << "        {" << std::endl;
                this->write() << "            this->read( _buff, _size, _read, this->" << attr->name << " );" << std::endl;

                if( attr->required == false )
                {
                    this->write() << "            this->" << attr->name << "_successful = true;" << std::endl;
                }
                this->write() << "            return true;" << std::endl;
                this->write() << "            break;" << std::endl;
                this->write() << "        }" << std::endl;
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

                    this->write() << "    case " << attr->id << ":" << std::endl;
                    this->write() << "        {" << std::endl;
                    this->write() << "            this->read( _buff, _size, _read, this->" << member->name << "_" << attr->name << " );" << std::endl;

                    if( attr->required == false )
                    {
                        this->write() << "            this->" << member->name << "_" << attr->name << "_successful = true;" << std::endl;
                    }
                    this->write() << "            return true;" << std::endl;
                    this->write() << "            break;" << std::endl;
                    this->write() << "        }" << std::endl;
                }
            }

            this->write() << "    }" << std::endl;
        }
        
        this->write() << "    return false;" << std::endl;
        this->write() << "}" << std::endl;
        this->write() << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceIncludesPreparation_( const XmlNode * _node )
    {
        this->write() << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write() << "bool " << _node->getScope() << "::_preparationIncludes( size_t _includes, size_t _count )" << std::endl;
        this->write() << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write() << "    if( " << node_inheritance->getScope() << "::_preparationIncludes( _includes, _count ) == true )" << std::endl;
            this->write() << "    {" << std::endl;
            this->write() << "        return true;" << std::endl;
            this->write() << "    }" << std::endl;

            if( _node->includes.empty() == false )
            {
                this->write() << std::endl;
            }
        }

        if( _node->includes.empty() == false || _node->inheritances.empty() == false )
        {            
            this->write() << "    switch( _includes )" << std::endl;
            this->write() << "    {" << std::endl;

            for( TMapNodes::const_iterator
                it = _node->includes.begin(),
                it_end = _node->includes.end();
            it != it_end;
            ++it )
            {
                const XmlNode * node = it->second;

                this->write() << "    case " << node->id << ":" << std::endl;
                this->write() << "        {" << std::endl;
                this->write() << "            includes_" << node->getName() << ".reserve( _count );" << std::endl;
                this->write() << "            return true;" << std::endl;
                this->write() << "            break;" << std::endl;
                this->write() << "        }" << std::endl;
            }

            for( TMapNodes::const_iterator
                it = _node->inheritances.begin(),
                it_end = _node->inheritances.end();
            it != it_end;
            ++it )
            {
                const XmlNode * node = it->second;

                this->write() << "    case " << node->id << ":" << std::endl;
                this->write() << "        {" << std::endl;
                this->write() << "            includes_" << node->getName() << ".reserve( _count );" << std::endl;
                this->write() << "            return true;" << std::endl;
                this->write() << "            break;" << std::endl;
                this->write() << "        }" << std::endl;
            }

            this->write() << "    }" << std::endl;
        }

        this->write() << "    return false;" << std::endl;
        this->write() << "}" << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceIncludesReader_( const XmlNode * _node )
    {
        this->write() << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write() << "bool " << _node->getScope() << "::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes )" << std::endl;
        this->write() << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write() << "    if( " << node_inheritance->getScope() << "::_parseIncludes( _buff, _size, _read, _includes ) == true )" << std::endl;
            this->write() << "    {" << std::endl;
            this->write() << "        return true;" << std::endl;
            this->write() << "    }" << std::endl;

            if( _node->includes.empty() == false )
            {
                this->write() << std::endl;
            }
        }

        if( _node->includes.empty() == false )
        {            
            this->write() << "    switch( _includes )" << std::endl;
            this->write() << "    {" << std::endl;

            for( TMapNodes::const_iterator
                it = _node->includes.begin(),
                it_end = _node->includes.end();
            it != it_end;
            ++it )
            {
                const XmlNode * node = it->second;
                                  
                this->write() << "    case " << node->id << ":" << std::endl;
                this->write() << "        {" << std::endl;
                this->write() << "            " << node->getScope() << " metadata;" << std::endl;
                this->write() << "            metadata.parse( _buff, _size, _read );" << std::endl;
                this->write() << std::endl;
                this->write() << "            includes_" << node->getName() << ".push_back(metadata);" << std::endl;
                this->write() << "            return true;" << std::endl;
                this->write() << "            break;" << std::endl;
                this->write() << "        }" << std::endl;
            }

            this->write() << "    }" << std::endl;
        }

        this->write() << "    return false;" << std::endl;
        this->write() << "}" << std::endl;
        this->write() << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceGeneratorsReader_( const XmlNode * _node )
    {
        this->write() << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write() << "bool " << _node->getScope() << "::_parseGenerators( const char * _buff, size_t _size, size_t & _read, size_t _generators )" << std::endl;
        this->write() << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = _node->node_inheritance;

            this->write() << "    if( " << node_inheritance->getScope() << "::_parseGenerators( _buff, _size, _read, _generators ) == true )" << std::endl;
            this->write() << "    {" << std::endl;
            this->write() << "        return true;" << std::endl;
            this->write() << "    }" << std::endl;

            if( _node->includes.empty() == false )
            {
                this->write() << std::endl;
            }
        }

        if( _node->generators.empty() == false )
        {            
            this->write() << "    switch( _generators )" << std::endl;
            this->write() << "    {" << std::endl;

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

                    this->write() << "    case " << node_generator->id << ":" << std::endl;
                    this->write() << "        {" << std::endl;
                    this->write() << "            " << node_generator->getScope() << " * metadata = new " << node_generator->getScope() << " ();" << std::endl;
                    this->write() << "            metadata->parse( _buff, _size, _read );" << std::endl;
                    this->write() << std::endl;
                    this->write() << "            includes_" << node_inheritance->getName() << ".push_back(metadata);" << std::endl;            
                    this->write() << "            return true;" << std::endl;
                    this->write() << "            break;" << std::endl;
                    this->write() << "        }" << std::endl;
                }
            }

            this->write() << "    }" << std::endl;
        }

        this->write() << "    return false;" << std::endl;
        this->write() << "}" << std::endl;
        this->write() << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceIncludesDefinition_( const XmlNode * _node )
    {
        for( TMapNodes::const_iterator
            it = _node->includes.begin(),
            it_end = _node->includes.end();
        it != it_end;
        ++it )
        {
            XmlNode * node_include = it->second;

            this->writeSourceNode_( node_include );
        }

        for( TMapNodes::const_iterator
            it = _node->inheritances.begin(),
            it_end = _node->inheritances.end();
        it != it_end;
        ++it )
        {
            XmlNode * node_include = it->second;

            this->writeSourceNode_( node_include );
        }

        for( TMapNodes::const_iterator
            it = _node->generators.begin(),
            it_end = _node->generators.end();
        it != it_end;
        ++it )
        {
            XmlNode * node_include = it->second;

            this->writeSourceNode_( node_include );
        }
        
        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	std::stringstream & Xml2Metacode::write()
	{
		for( size_t i = 0; i != m_indent; ++i )
		{
			m_out << " ";
		}
		
		return m_out;
	}
}