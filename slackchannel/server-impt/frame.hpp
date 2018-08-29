#ifndef __SLACKCHANNEL_FRAME_HPP__
#define __SLACKCHANNEL_FRAME_HPP__

#include <cstdlib>

namespace SlackChannel
{

/* The Natural dimensions of a Frame */
const int FRAME_HEADER_LENGTH = 4;
const int FRAME_BODY_MAX_LENGTH = 512;
const int ACTION_FLOW_INFO_SEGMENT_LENGTH = 2;
const int ACTION_ACK_DATA_SIZE = 2;
const int FRAME_FULL_MAX_LENGTH = FRAME_HEADER_LENGTH + FRAME_BODY_MAX_LENGTH;
const int ACTION_DATA_SEGMENT_MAX_LENGTH  = FRAME_BODY_MAX_LENGTH - ACTION_FLOW_INFO_SEGMENT_LENGTH;

class Frame
{
    public:

        Frame();

        const char* get_frame_ptr() const;
        char* get_frame_ptr();
        size_t get_frame_len() const;
        const char* get_data_seg_ptr() const;
        char* get_data_seg_ptr();
        size_t get_data_seg_len() const;
        void set_data_seg_len( int new_length );
        bool dec_header();
        void enc_header();

    private:

        char m_buffer[ FRAME_FULL_MAX_LENGTH ];
        int  m_data_seg_len;
};

}

#endif /*  __SLACKCHANNEL_FRAME_HPP__ */
