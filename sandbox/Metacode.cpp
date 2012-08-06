#   include "Metacode.h"

namespace Metacode
{
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::generateMetadata( size_t _id )
    {
        switch( _id )
        {
        case 13:
            {
                return new Meta_ResourceAnimation();
                break;
            }
        case 7:
            {
                return new Meta_ResourceCursorICO();
                break;
            }
        case 6:
            {
                return new Meta_ResourceCursorSystem();
                break;
            }
        case 3:
            {
                return new Meta_ResourceEmitterContainer();
                break;
            }
        case 4:
            {
                return new Meta_ResourceFont();
                break;
            }
        case 5:
            {
                return new Meta_ResourceGlyph();
                break;
            }
        case 2:
            {
                return new Meta_ResourceImageDefault();
                break;
            }
        case 14:
            {
                return new Meta_ResourceImageSolid();
                break;
            }
        case 15:
            {
                return new Meta_ResourceInternalObject();
                break;
            }
        case 11:
            {
                return new Meta_ResourceMovie();
                break;
            }
        case 9:
            {
                return new Meta_ResourcePlaylist();
                break;
            }
        case 8:
            {
                return new Meta_ResourceSound();
                break;
            }
        case 12:
            {
                return new Meta_ResourceVideo();
                break;
            }
        case 10:
            {
                return new Meta_ResourceWindow();
                break;
            }
        }
    
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 1:
            {
                Metabuf::Metadata * metadata = this->generateMetadata( _generators );
    
                Meta_DataBlock::Meta_Resource * metadata2 = static_cast<Meta_DataBlock::Meta_Resource *>(metadata);
                metadata2->parseNode( _buff, _size, _read );
    
                includes_Meta_Resource.push_back(metadata2);
                return false;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_Resource::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_Resource::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Name );
                return true;
                break;
            }
        case 2:
            {
                this->read( _buff, _size, _read, this->Type );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_Resource::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceAnimation::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceAnimation::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceAnimation::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
    
        switch( _includes )
        {
        case 3:
            {
                Meta_DataBlock::Meta_ResourceAnimation::Meta_Sequence metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Sequence.push_back(metadata);
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceAnimation::Meta_Sequence::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceAnimation::Meta_Sequence::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 2:
            {
                this->read( _buff, _size, _read, this->Delay );
                return true;
                break;
            }
        case 1:
            {
                this->read( _buff, _size, _read, this->ResourceImageName );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceAnimation::Meta_Sequence::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceCursorICO::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceCursorICO::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceCursorICO::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceCursorSystem::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceCursorSystem::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceCursorSystem::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceEmitterContainer::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceEmitterContainer::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                return true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->Folder_Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceEmitterContainer::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceFont::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceFont::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 6:
            {
                this->read( _buff, _size, _read, this->Color_Value );
                this->Color_Value_successful = true;
                return true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->Image_Path );
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->OutlineImage_Path );
                this->OutlineImage_Path_successful = true;
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->ResourceGlyph_Name );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceFont::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceGlyph::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceGlyph::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->GlyphPath_Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceGlyph::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceImageDefault::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceImageDefault::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 4:
            {
                this->read( _buff, _size, _read, this->File_Codec );
                this->File_Codec_successful = true;
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->File_MaxSize );
                this->File_MaxSize_successful = true;
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceImageDefault::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceImageSolid::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceImageSolid::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 4:
            {
                this->read( _buff, _size, _read, this->Color_Value );
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->Size_Value );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceImageSolid::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceInternalObject::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceInternalObject::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 4:
            {
                this->read( _buff, _size, _read, this->Internal_Group );
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->Internal_Name );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceInternalObject::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceMovie::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 4:
            {
                this->read( _buff, _size, _read, this->Duration_Value );
                this->Duration_Value_successful = true;
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->FrameDuration_Value );
                this->FrameDuration_Value_successful = true;
                return true;
                break;
            }
        case 6:
            {
                this->read( _buff, _size, _read, this->Height_Value );
                this->Height_Value_successful = true;
                return true;
                break;
            }
        case 7:
            {
                this->read( _buff, _size, _read, this->KeyFramesPackPath_Path );
                this->KeyFramesPackPath_Path_successful = true;
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->Width_Value );
                this->Width_Value_successful = true;
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
    
        switch( _includes )
        {
        case 10:
            {
                Meta_DataBlock::Meta_ResourceMovie::Meta_MovieCamera3D metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_MovieCamera3D.push_back(metadata);
                return true;
                break;
            }
        case 8:
            {
                Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer2D metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_MovieLayer2D.push_back(metadata);
                return true;
                break;
            }
        case 9:
            {
                Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer3D metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_MovieLayer3D.push_back(metadata);
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceMovie::Meta_MovieCamera3D::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::Meta_MovieCamera3D::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 4:
            {
                this->read( _buff, _size, _read, this->CameraAspect );
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->CameraFOV );
                return true;
                break;
            }
        case 2:
            {
                this->read( _buff, _size, _read, this->CameraInterest );
                return true;
                break;
            }
        case 1:
            {
                this->read( _buff, _size, _read, this->CameraPosition );
                return true;
                break;
            }
        case 6:
            {
                this->read( _buff, _size, _read, this->Height );
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->Width );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::Meta_MovieCamera3D::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer2D::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer2D::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 8:
            {
                this->read( _buff, _size, _read, this->BlendingMode );
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->In );
                return true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->Index );
                return true;
                break;
            }
        case 1:
            {
                this->read( _buff, _size, _read, this->Name );
                return true;
                break;
            }
        case 6:
            {
                this->read( _buff, _size, _read, this->Out );
                return true;
                break;
            }
        case 2:
            {
                this->read( _buff, _size, _read, this->Parent );
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->Source );
                return true;
                break;
            }
        case 7:
            {
                this->read( _buff, _size, _read, this->StartInterval );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer2D::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer3D::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer3D::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 8:
            {
                this->read( _buff, _size, _read, this->BlendingMode );
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->In );
                return true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->Index );
                return true;
                break;
            }
        case 1:
            {
                this->read( _buff, _size, _read, this->Name );
                return true;
                break;
            }
        case 6:
            {
                this->read( _buff, _size, _read, this->Out );
                return true;
                break;
            }
        case 2:
            {
                this->read( _buff, _size, _read, this->Parent );
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->Source );
                return true;
                break;
            }
        case 7:
            {
                this->read( _buff, _size, _read, this->StartInterval );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceMovie::Meta_MovieLayer3D::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourcePlaylist::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourcePlaylist::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->Loop_Value );
                this->Loop_Value_successful = true;
                return true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->Shuffle_Value );
                this->Shuffle_Value_successful = true;
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourcePlaylist::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
    
        switch( _includes )
        {
        case 5:
            {
                Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Tracks.push_back(metadata);
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 1:
            {
                Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks::Meta_Track metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Track.push_back(metadata);
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks::Meta_Track::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks::Meta_Track::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 2:
            {
                this->read( _buff, _size, _read, this->Codec );
               this->Codec_successful = true;
                return true;
                break;
            }
        case 1:
            {
                this->read( _buff, _size, _read, this->File );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourcePlaylist::Meta_Tracks::Meta_Track::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceSound::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceSound::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 6:
            {
                this->read( _buff, _size, _read, this->DefaultVolume_Value );
                this->DefaultVolume_Value_successful = true;
                return true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->File_Codec );
                this->File_Codec_successful = true;
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->File_Converter );
                this->File_Converter_successful = true;
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                return true;
                break;
            }
        case 7:
            {
                this->read( _buff, _size, _read, this->IsStreamable_Value );
                this->IsStreamable_Value_successful = true;
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceSound::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceVideo::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceVideo::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 4:
            {
                this->read( _buff, _size, _read, this->File_Alpha );
                this->File_Alpha_successful = true;
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->File_Codec );
                this->File_Codec_successful = true;
                return true;
                break;
            }
        case 3:
            {
                this->read( _buff, _size, _read, this->File_Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceVideo::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_DataBlock::Meta_ResourceWindow::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceWindow::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        if( Meta_DataBlock::Meta_Resource::_parseArguments( _buff, _size, _read, _id ) == true )
        {
            return true;
        }
    
        switch( _id )
        {
        case 3:
            {
                this->read( _buff, _size, _read, this->WindowBackground_ResourceImageName );
                this->WindowBackground_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 17:
            {
                this->read( _buff, _size, _read, this->WindowBottom_Offset );
                this->WindowBottom_Offset_successful = true;
                return true;
                break;
            }
        case 9:
            {
                this->read( _buff, _size, _read, this->WindowBottom_ResourceImageName );
                this->WindowBottom_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 19:
            {
                this->read( _buff, _size, _read, this->WindowLeft_Offset );
                this->WindowLeft_Offset_successful = true;
                return true;
                break;
            }
        case 11:
            {
                this->read( _buff, _size, _read, this->WindowLeft_ResourceImageName );
                this->WindowLeft_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 18:
            {
                this->read( _buff, _size, _read, this->WindowLeftBottom_Offset );
                this->WindowLeftBottom_Offset_successful = true;
                return true;
                break;
            }
        case 10:
            {
                this->read( _buff, _size, _read, this->WindowLeftBottom_ResourceImageName );
                this->WindowLeftBottom_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 12:
            {
                this->read( _buff, _size, _read, this->WindowLeftTop_Offset );
                this->WindowLeftTop_Offset_successful = true;
                return true;
                break;
            }
        case 4:
            {
                this->read( _buff, _size, _read, this->WindowLeftTop_ResourceImageName );
                this->WindowLeftTop_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 15:
            {
                this->read( _buff, _size, _read, this->WindowRight_Offset );
                this->WindowRight_Offset_successful = true;
                return true;
                break;
            }
        case 7:
            {
                this->read( _buff, _size, _read, this->WindowRight_ResourceImageName );
                this->WindowRight_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 16:
            {
                this->read( _buff, _size, _read, this->WindowRightBottom_Offset );
                this->WindowRightBottom_Offset_successful = true;
                return true;
                break;
            }
        case 8:
            {
                this->read( _buff, _size, _read, this->WindowRightBottom_ResourceImageName );
                this->WindowRightBottom_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 14:
            {
                this->read( _buff, _size, _read, this->WindowRightTop_Offset );
                this->WindowRightTop_Offset_successful = true;
                return true;
                break;
            }
        case 6:
            {
                this->read( _buff, _size, _read, this->WindowRightTop_ResourceImageName );
                this->WindowRightTop_ResourceImageName_successful = true;
                return true;
                break;
            }
        case 13:
            {
                this->read( _buff, _size, _read, this->WindowTop_Offset );
                this->WindowTop_Offset_successful = true;
                return true;
                break;
            }
        case 5:
            {
                this->read( _buff, _size, _read, this->WindowTop_ResourceImageName );
                this->WindowTop_ResourceImageName_successful = true;
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_DataBlock::Meta_ResourceWindow::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        if( Meta_DataBlock::Meta_Resource::_parseIncludes( _buff, _size, _read, _includes, _generators ) == true )
        {
            return true;
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 2:
            {
                Meta_Pak::Meta_Resources metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Resources.push_back(metadata);
                return true;
                break;
            }
        case 1:
            {
                Meta_Pak::Meta_Scripts metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Scripts.push_back(metadata);
                return true;
                break;
            }
        case 3:
            {
                Meta_Pak::Meta_Texts metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Texts.push_back(metadata);
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Resources::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Resources::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Resources::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 1:
            {
                Meta_Pak::Meta_Resources::Meta_Resource metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Resource.push_back(metadata);
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Resources::Meta_Resource::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Resources::Meta_Resource::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Resources::Meta_Resource::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Scripts::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Scripts::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Scripts::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Texts::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Texts::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Texts::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        switch( _includes )
        {
        case 1:
            {
                Meta_Pak::Meta_Texts::Meta_Text metadata;
                metadata.parseNode( _buff, _size, _read );
    
                includes_Meta_Text.push_back(metadata);
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    Metabuf::Metadata * Meta_Pak::Meta_Texts::Meta_Text::generateMetadata( size_t _id )
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Texts::Meta_Text::_parseArguments( const char * _buff, size_t _size, size_t & _read, size_t _id )
    {
        switch( _id )
        {
        case 1:
            {
                this->read( _buff, _size, _read, this->Path );
                return true;
                break;
            }
        }
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    bool Meta_Pak::Meta_Texts::Meta_Text::_parseIncludes( const char * _buff, size_t _size, size_t & _read, size_t _includes, size_t _generators )
    {
        return false;
    }
    
}
