#include "frame.hpp"
#include <cstdio>
#include <cstring>


SlackChannel::Frame::Frame() : m_data_seg_len(0)
{
    memset( m_buffer , 0 , FRAME_FULL_MAX_LENGTH );
}

const char*
SlackChannel::Frame::get_frame_ptr() const
{
    return this->m_buffer;
}

char*
SlackChannel::Frame::get_frame_ptr()
{
    return this->m_buffer;
}

size_t
SlackChannel::Frame::get_frame_len() const
{
    size_t len = FRAME_HEADER_LENGTH + this->m_data_seg_len;
    return len;
}

const char*
SlackChannel::Frame::get_data_seg_ptr() const
{
    return this->m_buffer + FRAME_HEADER_LENGTH;
}

char*
SlackChannel::Frame::get_data_seg_ptr()
{
    return this->m_buffer +  FRAME_HEADER_LENGTH;
}

size_t
SlackChannel::Frame::get_data_seg_len() const
{
    return this->m_data_seg_len;
}

void
SlackChannel::Frame::set_data_seg_len( int new_length )
{
    this->m_data_seg_len = new_length;

    if ( this->m_data_seg_len >  FRAME_BODY_MAX_LENGTH )
    {
        this->m_data_seg_len = FRAME_BODY_MAX_LENGTH;
    }
}

bool
SlackChannel::Frame::dec_header()
{
    using namespace std; // For strncat and atoi.

    char header[ FRAME_HEADER_LENGTH  + 1 ] = "";

    strncat( header, this->m_buffer, FRAME_HEADER_LENGTH );
    this->m_data_seg_len = atoi( header );

    if ( this->m_data_seg_len >  FRAME_BODY_MAX_LENGTH )
    {
        this->m_data_seg_len = 0;
        return false;
    }

    return true;
}

void
SlackChannel::Frame::enc_header()
{
    using namespace std; // For sprintf and memcpy.

    char header[ FRAME_HEADER_LENGTH + 1 ] = "";

    sprintf( header, "%4d", this->m_data_seg_len );
    memcpy( this->m_buffer, header, FRAME_HEADER_LENGTH );
}
