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
	bool Xml2Metacode::generate( std::string & _out )
	{	
		this->write() << "#   pragma once" << std::endl;
		this->write() << std::endl;
		this->write() << "#   include <Metabuf.hpp>" << std::endl;
		this->write() << std::endl;
		this->write() << "namespace Menge" << std::endl;
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

			if( this->writeNode_( &node ) == false )
			{
				return false;
			}
		}
				
		m_indent -= 4;

		m_out << "}" << std::endl;

		_out = m_out.str();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeNode_( const XmlNode * _node )
	{
		this->write() << "class " << _node->name << std::endl;

		if( _node->inheritance.empty() == false )
		{
			this->write() << "    : public " << _node->inheritance << std::endl;
		}
		else
		{
			this->write() << "    : public Metabuf::Metadata" << std::endl;
		}

		this->write() << "{ " << std::endl;

		//size_t id = _node->id;
		//s_writeStream( _stream, id );

		if( this->writeConstructor_( _node ) == false )
		{
			return false;
		}

		if( this->writeNodeSetup_( _node ) == false )
		{
			return false;
		}

		if( this->writeNodeReader_( _node ) == false )
		{
			return false;
		}

		if( this->writeNodeMember_( _node ) == false )
		{
			return false;
		}

		this->write() << "} " << std::endl;
		this->write() << std::endl;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeConstructor_( const XmlNode * _node )
	{
		this->write() << "public:" << std::endl;
		
		this->write() << "    " << _node->name << "()" << std::endl;

		if( _node->inheritance.empty() == false )
		{
			this->write() << "        : " << _node->inheritance << "()" << std::endl;
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
	bool Xml2Metacode::writeNodeReader_( const XmlNode * _node )
	{
		this->write() << "protected:" << std::endl;
		m_indent += 4;

		this->write() << "void _parse( char * _buff, size_t _size, size_t & _read, size_t _id ) override" << std::endl;
		this->write() << "{" << std::endl;
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

		m_indent -= 4;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Xml2Metacode::writeNodeSetup_( const XmlNode * _node )
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
	bool Xml2Metacode::writeNodeMember_( const XmlNode * _node )
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
				this->write() << "bool" << " " << attr->name << "_successful" << std::endl;
			}

			this->write() << attr->type << " " << attr->name << std::endl;
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
	std::stringstream & Xml2Metacode::write()
	{
		for( size_t i = 0; i != m_indent; ++i )
		{
			m_out << " ";
		}
		
		return m_out;
	}
}