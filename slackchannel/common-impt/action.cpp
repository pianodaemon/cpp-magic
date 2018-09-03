#include "action.hpp"
#include <cstring>


void
SlackChannel::actionFree( Action* act_ptr )
{
    delete[] act_ptr->payload;
    delete act_ptr;
}


void
SlackChannel::putActionInto( Frame& frm, Action* act_ptr , size_t plsize )
{
    char* ds_ptr = nullptr;

    frm.set_data_seg_len( ACTION_FLOW_INFO_SEGMENT_LENGTH + plsize );

    ds_ptr = frm.get_data_seg_ptr();

    memcpy( ds_ptr , &act_ptr->id , 1 );
    memcpy( ds_ptr + 1 , &act_ptr->transaction , 1 );
    memcpy( ds_ptr + ACTION_FLOW_INFO_SEGMENT_LENGTH , act_ptr->payload, plsize );

    frm.enc_header();
}
