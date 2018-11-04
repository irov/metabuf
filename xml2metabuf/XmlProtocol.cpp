#include "XmlProtocol.hpp"

#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <memory.h>

namespace Metabuf
{
    //////////////////////////////////////////////////////////////////////////
    std::string XmlAttribute::getWriteName() const
    {
        std::string write_name;
        write_name = "m_" + this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlMember::getWriteName() const
    {
        std::string write_name;
        write_name = "m_" + this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlAttribute * XmlMember::getAttribute( const std::string & _name ) const
    {
        TMapAttributes::const_iterator it_found = this->attributes.find( _name );

        if( it_found == this->attributes.end() )
        {
            return nullptr;
        }

        const XmlAttribute * attr = &it_found->second;

        return attr;
    }
    //////////////////////////////////////////////////////////////////////////
    XmlNode::XmlNode()
        : id( 0 )
        , node_inheritance( nullptr )
        , node_scope( nullptr )
        , enumerator( 0 )
        , noWrite( false )
    {

    }
    //////////////////////////////////////////////////////////////////////////
    XmlNode::~XmlNode()
    {
        for( TMapNodes::const_iterator
            it = includes.begin(),
            it_end = includes.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            delete node;
        }

        for( TMapNodes::const_iterator
            it = inheritances.begin(),
            it_end = inheritances.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            delete node;
        }

        for( TMapNodes::const_iterator
            it = generators.begin(),
            it_end = generators.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            delete node;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlAttribute * XmlNode::getAttribute( const std::string & _name ) const
    {
        TMapAttributes::const_iterator it_found = this->attributes.find( _name );

        if( it_found == this->attributes.end() )
        {
            if( this->node_inheritance == nullptr )
            {
                return nullptr;
            }
            else
            {
                const XmlAttribute * attr_inheritance = this->node_inheritance->getAttribute( _name );

                return attr_inheritance;
            }
        }

        const XmlAttribute * attr = &it_found->second;

        return attr;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlMember * XmlNode::getMember( const std::string & _name ) const
    {
        TMapMembers::const_iterator it_found = this->members.find( _name );

        if( it_found == this->members.end() )
        {
            if( this->node_inheritance == nullptr )
            {
                return nullptr;
            }
            else
            {
                const XmlMember * member_inheritance = this->node_inheritance->getMember( _name );

                return member_inheritance;
            }
        }

        const XmlMember * member = &it_found->second;

        return member;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getSingle( const std::string & _name ) const
    {
        TMapNodes::const_iterator it_found = this->singles.find( _name );

        if( it_found == this->singles.end() )
        {
            return nullptr;
        }

        XmlNode * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getInclude( const std::string & _name ) const
    {
        TMapNodes::const_iterator it_found = this->includes.find( _name );

        if( it_found == this->includes.end() )
        {
            return nullptr;
        }

        XmlNode * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getInheritances( const std::string & _name ) const
    {
        TMapNodes::const_iterator it_found = this->inheritances.find( _name );

        if( it_found == this->inheritances.end() )
        {
            return nullptr;
        }

        XmlNode * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getGenerator( const std::string & _name ) const
    {
        TMapNodes::const_iterator it_found = this->generators.find( _name );

        if( it_found == this->generators.end() )
        {
            return nullptr;
        }

        const XmlNode * node = it_found->second;

        return node;
    }
    //////////////////////////////////////////////////////////////////////////
    const std::string & XmlNode::getName() const
    {
        return this->name;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlNode::getWriteName() const
    {
        std::string write_name;
        write_name += "Meta_" + this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlNode::getScope() const
    {
        std::string write_scope;

        if( this->node_scope == nullptr )
        {
            write_scope += this->getWriteName();
            return write_scope;
        }

        write_scope += this->node_scope->getScope();
        write_scope += "::";
        write_scope += this->getWriteName();

        return write_scope;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlNode::getNoWrite() const
    {
        return noWrite;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlNode::hasNode( const std::string & _type ) const
    {
        TMapNodes::const_iterator it_singles_found = singles.find( _type );

        if( it_singles_found != singles.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = singles.begin(),
            it_end = singles.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        TMapNodes::const_iterator it_includes_found = includes.find( _type );

        if( it_includes_found != includes.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = includes.begin(),
            it_end = includes.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        TMapNodes::const_iterator it_inheritances_found = inheritances.find( _type );

        if( it_inheritances_found != inheritances.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = inheritances.begin(),
            it_end = inheritances.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlNode::getNode( const std::string & _type ) const
    {
        TMapNodes::const_iterator it_singles_found = singles.find( _type );

        if( it_singles_found != singles.end() )
        {
            const XmlNode * node = it_singles_found->second;

            return node;
        }

        for( TMapNodes::const_iterator
            it = singles.begin(),
            it_end = singles.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            const XmlNode * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        TMapNodes::const_iterator it_includes_found = includes.find( _type );

        if( it_includes_found != includes.end() )
        {
            const XmlNode * node = it_includes_found->second;

            return node;
        }

        for( TMapNodes::const_iterator
            it = includes.begin(),
            it_end = includes.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            const XmlNode * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        TMapNodes::const_iterator it_inheritances_found = inheritances.find( _type );

        if( it_inheritances_found != inheritances.end() )
        {
            const XmlNode * node = it_inheritances_found->second;

            return node;
        }

        for( TMapNodes::const_iterator
            it = inheritances.begin(),
            it_end = inheritances.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            const XmlNode * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        return nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    XmlMeta::XmlMeta()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    XmlMeta::~XmlMeta()
    {
        for( TMapNodes::const_iterator
            it = nodes.begin(),
            it_end = nodes.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            delete node;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t XmlMeta::getVersion() const
    {
        return this->version;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlMeta::getWriteName() const
    {
        std::string write_name;
        write_name += "Meta_" + this->name;

        return write_name;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlMeta::hasNode( const std::string & _type ) const
    {
        TMapNodes::const_iterator it_found = nodes.find( _type );

        if( it_found != nodes.end() )
        {
            return true;
        }

        for( TMapNodes::const_iterator
            it = nodes.begin(),
            it_end = nodes.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            if( node->hasNode( _type ) == true )
            {
                return true;
            }
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlNode * XmlMeta::getNode( const std::string & _type ) const
    {
        TMapNodes::const_iterator it_found = nodes.find( _type );

        if( it_found != nodes.end() )
        {
            const XmlNode * found_node = it_found->second;

            return found_node;
        }

        for( TMapNodes::const_iterator
            it = nodes.begin(),
            it_end = nodes.end();
            it != it_end;
            ++it )
        {
            const XmlNode * node = it->second;

            const XmlNode * found_node = node->getNode( _type );

            if( found_node != nullptr )
            {
                return found_node;
            }
        }

        return nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    const TMapNodes & XmlMeta::getNodes() const
    {
        return nodes;
    }
    //////////////////////////////////////////////////////////////////////////
    XmlProtocol::XmlProtocol()
        : m_enumerator( 0 )
        , m_version( 0 )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    XmlProtocol::~XmlProtocol()
    {
        for( TMapMetas::const_iterator
            it = m_metas.begin(),
            it_end = m_metas.end();
            it != it_end;
            ++it )
        {
            const XmlMeta * meta = it->second;

            delete meta;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t XmlProtocol::getVersion() const
    {
        return m_version;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t XmlProtocol::getCrc32() const
    {
        return m_crc32;
    }
    //////////////////////////////////////////////////////////////////////////
    std::string XmlProtocol::getError() const
    {
        return m_error.str();
    }
    //////////////////////////////////////////////////////////////////////////
    const uint32_t Crc32Table[256] = {
        0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
        0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
        0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
        0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
        0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
        0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
        0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
        0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
        0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
        0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
        0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
        0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
        0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
        0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
        0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
        0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
        0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
        0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
        0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
        0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
        0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
        0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
        0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
        0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
        0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
        0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
        0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
        0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
        0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
        0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
        0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
        0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
        0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
        0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
        0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
        0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
        0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
        0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
        0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
        0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
        0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
        0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
        0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
        0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };
    //////////////////////////////////////////////////////////////////////////
    static uint32_t make_crc32( const void * _buffer, size_t _size )
    {
        uint32_t crc = 0xFFFFFFFF;

        const uint8_t * byte_buffer = reinterpret_cast<const uint8_t *>(_buffer);

        while( _size-- )
        {
            crc = (crc >> 8) ^ Crc32Table[(crc ^ *byte_buffer++) & 0xFF];
        }

        return crc ^ 0xFFFFFFFF;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::readProtocol( const void * _buff, size_t _size )
    {
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load_buffer( _buff, _size );

        if( result == false )
        {
            m_error << "XmlProtocol::readProtocol:" << result.description() << std::endl;

            return false;
        }

        pugi::xml_node root = doc.document_element();

        if( root.empty() == true )
        {
            return true;
        }

        pugi::xml_attribute xml_version = root.attribute( "Version" );

        const char * version_value = xml_version.value();

        if( sscanf( version_value, "%u", &m_version ) != 1 )
        {
            m_error << "XmlProtocol::readProtocol: not set version!" << std::endl;

            return false;
        }

        for( pugi::xml_node::iterator
            it = root.begin(),
            it_end = root.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Type" ) == 0 )
            {
                if( this->readType_( element ) == false )
                {
                    return false;
                }
            }
        }

        for( pugi::xml_node::iterator
            it = root.begin(),
            it_end = root.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Enum" ) == 0 )
            {
                if( this->readEnum_( element ) == false )
                {
                    return false;
                }
            }
        }

        for( pugi::xml_node::iterator
            it = root.begin(),
            it_end = root.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Meta" ) == 0 )
            {
                if( this->readMeta_( element ) == false )
                {
                    return false;
                }
            }
        }

        std::stringstream ss;
        doc.save( ss );

        std::string ss_str = ss.str();

        m_crc32 = make_crc32( ss_str.c_str(), ss_str.size() );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::readEnum_( const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute Name = _xml_node.attribute( "Name" );
        pugi::xml_attribute Type = _xml_node.attribute( "Type" );
        pugi::xml_attribute Template = _xml_node.attribute( "Template" );

        if( Name.empty() == true )
        {
            m_error << "XmlProtocol::readEnum_: Name not set" << std::endl;

            return false;
        }

        const char * type_name = Name.value();
        const char * type_write = Type.value();


        XmlType type;
        type.is_enumerator = true;
        type.write = (strlen( type_write ) == 0) ? type_name : type_write;
        type.evict = "uint32_t";

        if( Template.empty() == false )
        {
            const char * type_Template = Template.value();

            uint32_t type_Template_value;
            if( sscanf( type_Template, "%d", &type_Template_value ) != 1 )
            {
                m_error << "XmlProtocol::readEnum_: Template invalid" << std::endl;

                return false;
            }

            type.is_template = (type_Template_value != 0);
        }
        else
        {
            type.is_template = false;
        }

        uint32_t enum_index = 0;

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            XmlEnum e;
            e.write = element_name;

            pugi::xml_attribute EnumName = element.attribute( "Name" );

            if( EnumName.empty() == true )
            {
                e.name = element_name;
            }
            else
            {
                const char * enum_name_value = EnumName.value();

                e.name = enum_name_value;
            }

            pugi::xml_attribute EnumIndex = element.attribute( "Index" );

            if( EnumIndex.empty() == false )
            {
                const char * enum_index_value = EnumIndex.value();

                uint32_t value;
                if( sscanf( enum_index_value, "%u", &value ) != 1 )
                {
                    m_error << "XmlProtocol::readEnum_: Enum '" << type_name << "' invalid index '" << enum_index_value << "'" << std::endl;

                    return false;
                }

                enum_index = value;
            }

            e.index = enum_index;
            ++enum_index;

            type.enumerators.push_back( e );
        }

        m_types.insert( std::make_pair( type_name, type ) );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::readType_( const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute Name = _xml_node.attribute( "Name" );
        pugi::xml_attribute Type = _xml_node.attribute( "Type" );
        pugi::xml_attribute Evict = _xml_node.attribute( "Evict" );
        pugi::xml_attribute NCR = _xml_node.attribute( "NCR" );
        pugi::xml_attribute Template = _xml_node.attribute( "Template" );

        if( Name.empty() == true || Evict.empty() == true )
        {
            m_error << "XmlProtocol::readType_: Name or Evict not set" << std::endl;

            return false;
        }

        const char * type_name = Name.value();
        const char * type_write = Type.value();
        const char * type_evict = Evict.value();

        XmlType type;
        type.is_enumerator = false;
        type.write = (strlen( type_write ) == 0) ? type_name : type_write;
        type.evict = type_evict;

        if( NCR.empty() == false )
        {
            const char * type_NCR = NCR.value();

            uint32_t type_NCR_value;
            if( sscanf( type_NCR, "%d", &type_NCR_value ) != 1 )
            {
                m_error << "XmlProtocol::readType_: NCR invalid" << std::endl;

                return false;
            }

            type.is_ncr = (type_NCR_value != 0);
        }
        else
        {
            type.is_ncr = false;
        }

        if( Template.empty() == false )
        {
            const char * type_Template = Template.value();

            uint32_t type_Template_value;
            if( sscanf( type_Template, "%d", &type_Template_value ) != 1 )
            {
                m_error << "XmlProtocol::readType_: Template invalid" << std::endl;

                return false;
            }

            type.is_template = (type_Template_value != 0);
        }
        else
        {
            type.is_template = false;
        }

        m_types.insert( std::make_pair( type_name, type ) );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::readMeta_( const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute MetaName = _xml_node.attribute( "Name" );
        pugi::xml_attribute MetaVersion = _xml_node.attribute( "Version" );

        XmlMeta * metaXml = new XmlMeta();

        metaXml->name = MetaName.as_string();
        metaXml->version = MetaVersion.as_uint();

        m_metas.insert( std::make_pair( metaXml->name, metaXml ) );

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( element_name == nullptr )
            {
                return false;
            }

            if( strcmp( element_name, "Node" ) == 0 )
            {
                if( this->readNode_( metaXml, nullptr, element ) == false )
                {
                    return false;
                }
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::readNode_( XmlMeta * _meta, XmlNode * _node, const pugi::xml_node & _xml_node )
    {
        pugi::xml_attribute NodeName = _xml_node.attribute( "Name" );
        pugi::xml_attribute NodeGenerator = _xml_node.attribute( "Generator" );
        pugi::xml_attribute NodeInheritance = _xml_node.attribute( "Inheritance" );
        pugi::xml_attribute NodeNoWrite = _xml_node.attribute( "NoWrite" );
        pugi::xml_attribute NodeSingle = _xml_node.attribute( "Single" );

        XmlNode * nodeXml = new XmlNode();

        if( _node == nullptr )
        {
            _meta->nodes.insert( std::make_pair( NodeName.value(), nodeXml ) );
        }
        else
        {
            if( NodeGenerator.empty() == false )
            {
                if( NodeSingle.empty() == false && strcmp( NodeSingle.value(), "1" ) == 0 )
                {
                    m_error << "XmlProtocol::readNode_: node " << NodeName.value() << " inheritances no support single" << std::endl;

                    return false;
                }

                _node->inheritances.insert( std::make_pair( NodeName.value(), nodeXml ) );
            }
            else if( NodeInheritance.empty() == false )
            {
                if( NodeSingle.empty() == false && strcmp( NodeSingle.value(), "1" ) == 0 )
                {
                    m_error << "XmlProtocol::readNode_: node " << NodeName.value() << " inheritance no support single" << std::endl;

                    return false;
                }

                _node->generators.insert( std::make_pair( NodeName.value(), nodeXml ) );
            }
            else
            {
                if( NodeSingle.empty() == false && strcmp( NodeSingle.value(), "1" ) == 0 )
                {
                    if( _node->singles.find( NodeName.value() ) != _node->singles.end() )
                    {
                        m_error << "XmlProtocol::readNode_: node " << NodeName.value() << " have multiply single element " << NodeSingle.value() << std::endl;

                        return false;
                    }

                    _node->singles.insert( std::make_pair( NodeName.value(), nodeXml ) );
                }
                else
                {
                    _node->includes.insert( std::make_pair( NodeName.value(), nodeXml ) );
                }
            }
        }

        if( NodeNoWrite.empty() == false )
        {
            nodeXml->noWrite = true;
        }

        if( _node == nullptr )
        {
            nodeXml->id = ++m_enumerator;
        }
        else
        {
            nodeXml->id = ++_node->enumerator;
        }

        nodeXml->enumerator = 0;
        nodeXml->node_inheritance = nullptr;
        nodeXml->node_scope = _node;
        nodeXml->name = NodeName.value();

        if( NodeGenerator.empty() == false )
        {
            nodeXml->generator = NodeGenerator.value();
        }

        if( NodeInheritance.empty() == false )
        {
            nodeXml->inheritance = NodeInheritance.value();

            nodeXml->node_inheritance = _node->getInheritances( nodeXml->inheritance );

            if( nodeXml->node_inheritance == nullptr )
            {
                m_error << "XmlProtocol::readNode_: node " << NodeName.value() << " not found inheritance " << NodeInheritance.value() << std::endl;

                return false;
            }

            nodeXml->enumerator = nodeXml->node_inheritance->enumerator;
        }

        for( pugi::xml_node::iterator
            it = _xml_node.begin(),
            it_end = _xml_node.end();
            it != it_end;
            ++it )
        {
            const pugi::xml_node & element = *it;

            const char * element_name = element.name();

            if( strcmp( element_name, "Attribute" ) == 0 )
            {
                pugi::xml_attribute AttributeName = element.attribute( "Name" );
                pugi::xml_attribute AttributeType = element.attribute( "Type" );
                pugi::xml_attribute AttributeRequired = element.attribute( "Required" );

                XmlAttribute & attributeXml = nodeXml->attributes[AttributeName.value()];

                attributeXml.id = ++nodeXml->enumerator;
                attributeXml.name = AttributeName.value();
                attributeXml.type = AttributeType.value();
                attributeXml.required = AttributeRequired.empty() == false;

                if( this->hasType( attributeXml.type ) == false )
                {
                    m_error << "XmlProtocol::readNode_: Attribute " << NodeName.value() << " not found type " << AttributeType.value() << std::endl;

                    return false;
                }
            }
            else if( strcmp( element_name, "Member" ) == 0 )
            {
                pugi::xml_attribute MemberNode = element.attribute( "Node" );
                pugi::xml_attribute MemberName = element.attribute( "Name" );
                pugi::xml_attribute MemberType = element.attribute( "Type" );
                //pugi::xml_attribute Evict = element.attribute("Evict");
                pugi::xml_attribute MemberRequired = element.attribute( "Required" );

                XmlMember & memberXml = nodeXml->members[MemberNode.value()];

                memberXml.name = MemberNode.value();

                XmlAttribute & attributeXml = memberXml.attributes[MemberName.value()];

                attributeXml.id = ++nodeXml->enumerator;
                attributeXml.name = MemberName.value();
                attributeXml.type = MemberType.value();
                attributeXml.required = MemberRequired.empty() == false;

                if( this->hasType( attributeXml.type ) == false )
                {
                    m_error << "XmlProtocol::readNode_: Member " << NodeName.value() << " not found type " << MemberType.value() << std::endl;

                    return false;
                }
            }
            else if( strcmp( element_name, "Children" ) == 0 )
            {
                pugi::xml_attribute ChildrenGroup = element.attribute( "Group" );
                pugi::xml_attribute ChildrenType = element.attribute( "Type" );

                const char * ChildrenGroupValue = ChildrenGroup.value();

                XmlChildren & childrenXml = nodeXml->children[ChildrenGroupValue];

                childrenXml.group = ChildrenGroup.value();
                childrenXml.type = ChildrenType.value();

                if( _meta->hasNode( childrenXml.type ) == false )
                {
                    m_error << "XmlProtocol::readNode_: Children " << NodeName.value() << " not found type " << ChildrenType.value() << std::endl;

                    return false;
                }
            }
            else if( strcmp( element_name, "Node" ) == 0 )
            {
                if( this->readNode_( _meta, nodeXml, element ) == false )
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::hasType( const std::string & _name ) const
    {
        TMapTypes::const_iterator it_found = m_types.find( _name );

        if( it_found == m_types.end() )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::getType( const std::string & _name, XmlType & _type ) const
    {
        TMapTypes::const_iterator it_found = m_types.find( _name );

        if( it_found == m_types.end() )
        {
            return false;
        }

        _type = it_found->second;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool XmlProtocol::hasMeta( const std::string & _type ) const
    {
        TMapMetas::const_iterator it_found = m_metas.find( _type );

        if( it_found == m_metas.end() )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    const XmlMeta * XmlProtocol::getMeta( const std::string & _type ) const
    {
        TMapMetas::const_iterator it_found = m_metas.find( _type );

        if( it_found == m_metas.end() )
        {
            return nullptr;
        }

        const XmlMeta * found_node = it_found->second;

        return found_node;
    }
    //////////////////////////////////////////////////////////////////////////
    const TMapMetas & XmlProtocol::getMetas() const
    {
        return m_metas;
    }
}