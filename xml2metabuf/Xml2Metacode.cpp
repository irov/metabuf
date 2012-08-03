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
        this->write() << "#   include <Metabuf.hpp>" << std::endl;
        this->write() << std::endl;
        this->write() << "#   include <vector>" << std::endl;
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
            const XmlNode & node = it->second;

            if( this->writeHeaderNode_( &node ) == false )
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
            const XmlNode * node_inheritance = m_protocol->getNode( _node->inheritance );

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

        if( this->writeHeaderIncludesReader_( _node ) == false )
        {
            return false;
        }

        m_indent -= 4;

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
            const XmlNode * node_inheritance = m_protocol->getNode( _node->inheritance );

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
				this->write() << "        , " << attr->name << "_successful(false);" << std::endl;
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
		
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderAttributeReader_( const XmlNode * _node )
	{
		this->write() << "void _parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id ) override;" << std::endl;

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

			this->write() << "void" << " " << "setup_" << attr->name << "( " << attr->type << " & _value )" << std::endl;
			this->write() << "{" << std::endl;
			
			if( attr->required == false )
			{
				this->write() << "    if( " << attr->name << "_successful == false )" << std::endl;
				this->write() << "    {" << std::endl;
				this->write() << "        return;" << std::endl;
				this->write() << "    }" << std::endl;
				this->write() << std::endl;
			}

			this->write() << "    _value = this->" << attr->name << ";" << std::endl;
			this->write() << "}" << std::endl;
			this->write() << std::endl;

			this->write() << "template<class C, class M>" << std::endl;
			this->write() << "void" << " " << "setup_" << attr->name << "( C * _self, M _method )" << std::endl;
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

				this->write() << "void" << " " << "setup_" << member->name << "_" << attr->name << "( " << attr->type << " & _value )" << std::endl;
				this->write() << "{" << std::endl;

				if( attr->required == false )
				{
					this->write() << "    if( " << member->name << "_" << attr->name << "_successful == false )" << std::endl;
					this->write() << "    {" << std::endl;
					this->write() << "        return;" << std::endl;
					this->write() << "    }" << std::endl;
					this->write() << std::endl;
				}

				this->write() << "    _value = this->" << member->name << "_" << attr->name << ";" << std::endl;
				this->write() << "}" << std::endl;
				this->write() << std::endl;

				this->write() << "template<class C, class M>" << std::endl;
				this->write() << "void" << " " << "setup_" << member->name << "_" << attr->name << "( C * _self, M _method )" << std::endl;
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

			this->write() << attr->type << " " << attr->name << ";" << std::endl;
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

				this->write() << attr->type << " " << member->name << "_" << attr->name << ";" << std::endl;
			}
		}

		m_indent -= 4;

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludes_( const XmlNode * _node )
	{
		m_indent += 4;

		for( TVectorIncludes::const_iterator
			it = _node->includes.begin(),
			it_end = _node->includes.end();
		it != it_end;
		++it )
		{
			const std::string & nodeName = *it;

            const XmlNode * node = m_protocol->getNode( nodeName );

			this->write() << "typedef std::vector<class " << node->getName() << " *> TVector" << node->getName() << ";" << std::endl;
			this->write() << "TVector" << node->getName() << " includes_" << node->getName() << ";" << std::endl;
		}
		
		m_indent -= 4;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeHeaderIncludesReader_( const XmlNode * _node )
	{
		this->write() << "void _parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _id ) override;" << std::endl;

        return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::generateSource( std::string & _header )
    {
        this->write() << "#   include \"Metatype.hpp\"" << std::endl;
        this->write() << "#   include \"Metacode.hpp\"" << std::endl;
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
            const XmlNode & node = it->second;

            if( this->writeSourceNode_( &node ) == false )
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

        if( this->writeSourceIncludesReader_( _node ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceAttributeReader_( const XmlNode * _node )
    {
        this->write() << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write() << "void " << _node->getName() << "::_parseArguments( char * _buff, size_t _size, size_t & _read, size_t _id )" << std::endl;
        this->write() << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = m_protocol->getNode( _node->inheritance );

            this->write() << "    " << node_inheritance->getName()<< "::_parseArguments( _buff, _size, _read, _id );" << std::endl;
            this->write() << std::endl;
        }

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
                this->write() << "           this->" << attr->name << "_successful = true;" << std::endl;
            }

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

                this->write() << "        }" << std::endl;
            }
        }

        this->write() << "    }" << std::endl;
        this->write() << "}" << std::endl;
        this->write() << std::endl;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Xml2Metacode::writeSourceIncludesReader_( const XmlNode * _node )
    {
        this->write() << "//////////////////////////////////////////////////////////////////////////" << std::endl;
        this->write() << "void " << _node->getName() << "::_parseIncludes( char * _buff, size_t _size, size_t & _read, size_t _id )" << std::endl;
        this->write() << "{" << std::endl;

        if( _node->inheritance.empty() == false )
        {
            const XmlNode * node_inheritance = m_protocol->getNode( _node->inheritance );

            this->write() << "    " << node_inheritance->getName() << "::_parseIncludes( _buff, _size, _read, _id );" << std::endl;
            this->write() << std::endl;
        }

        this->write() << "    switch( _id )" << std::endl;
        this->write() << "    {" << std::endl;

        for( TVectorIncludes::const_iterator
            it = _node->includes.begin(),
            it_end = _node->includes.end();
        it != it_end;
        ++it )
        {
            const std::string & nodeName = *it;

            const XmlNode * node = m_protocol->getNode( nodeName );

            this->write() << "    case " << node->id << ":" << std::endl;
            this->write() << "        {" << std::endl;
            this->write() << "            " << node->getName() << " * metadata = new " << node->getName() << "();" << std::endl;
            this->write() << "            metadata->parse( _buff, _size, _read );" << std::endl;
            this->write() << std::endl;
            this->write() << "            includes_" << node->getName() << ".push_back(metadata);" << std::endl;            
            this->write() << "        }" << std::endl;
        }

        this->write() << "    }" << std::endl;
        this->write() << "}" << std::endl;
        this->write() << std::endl;

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