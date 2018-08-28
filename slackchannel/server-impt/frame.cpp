#include "frame.hpp"
#include <cstdio>
#include <cstring>


SlackProto::Frame::Frame() : m_dataSegmentLength(0)
{
    memset( m_buffer , 0 , FRAME_FULL_MAX_LENGTH );
}

const char*
SlackProto::Frame::getFramePtr() const
{
    return this->m_buffer;
}

char*
SlackProto::Frame::getFramePtr()
{
    return this->m_buffer;
}

size_t
SlackProto::Frame::getFrameLength() const
{
    size_t len = FRAME_HEADER_LENGTH + this->m_dataSegmentLength;
    return len;
}

const char*
SlackProto::Frame::getDataSegmentPtr() const
{
    return this->m_buffer + FRAME_HEADER_LENGTH;
}

char*
SlackProto::Frame::getDataSegmentPtr()
{
    return this->m_buffer +  FRAME_HEADER_LENGTH;
}

size_t
SlackProto::Frame::getDataSegmentLength() const
{
    return this->m_dataSegmentLength;
}

void
SlackProto::Frame::setDataSegmentLength( int new_length )
{
    this->m_dataSegmentLength = new_length;

    if ( this->m_dataSegmentLength >  FRAME_BODY_MAX_LENGTH )
    {
        this->m_dataSegmentLength = FRAME_BODY_MAX_LENGTH;
    }
}

bool
SlackProto::Frame::decodeHeader()
{
    using namespace std; // For strncat and atoi.

    char header[ FRAME_HEADER_LENGTH  + 1 ] = "";

    strncat( header, this->m_buffer, FRAME_HEADER_LENGTH );
    this->m_dataSegmentLength = atoi( header );

    if ( this->m_dataSegmentLength >  FRAME_BODY_MAX_LENGTH )
    {
        this->m_dataSegmentLength = 0;
        return false;
    }

    return true;
}

void
SlackProto::Frame::encodeHeader()
{
    using namespace std; // For sprintf and memcpy.

    char header[ FRAME_HEADER_LENGTH + 1 ] = "";

    sprintf( header, "%4d", this->m_dataSegmentLength );
    memcpy( this->m_buffer, header, FRAME_HEADER_LENGTH );
}
