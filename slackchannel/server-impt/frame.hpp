#ifndef __SLACKPROTO_FRAME_HPP__
#define __SLACKPROTO_FRAME_HPP__

#include <cstdlib>

namespace SlackProto
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

        const char* getFramePtr() const;
        char* getFramePtr();
        size_t getFrameLength() const;
        const char* getDataSegmentPtr() const;
        char* getDataSegmentPtr();
        size_t getDataSegmentLength() const;
        void setDataSegmentLength( int new_length );
        bool decodeHeader();
        void encodeHeader();

    private:

        char m_buffer[ FRAME_FULL_MAX_LENGTH ];
        int  m_dataSegmentLength;
};

}

#endif /*  __SLACKPROTO_FRAME_HPP__ */
