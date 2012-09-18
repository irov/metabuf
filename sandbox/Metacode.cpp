#   include "Metacode.h"

namespace Metacode
{
    bool readHeader( const char * _buff, size_t _size, size_t & _read, size_t & _readVersion, size_t & _needVersion )
    {
       Metabuf::ArchiveReader ar(_buff, _size, _read);

       unsigned int head;
       ar >> head;

       if( head != 3133062829 )
       {
           return false;
       }

       unsigned int version;
       ar >> version;

       _readVersion = version;
       _needVersion = 0;
       if( version != 0 )
       {
           return false;
       }

       return true;
    }

}
