#include "monitor.hpp"


void
SlackChannel::Monitor::incomming( const char* buff_ptr, size_t len )
{

    Action act;

    char* ds_ptr = const_cast< char* >( buff_ptr );

    size_t plsize = len - ACTION_FLOW_INFO_SEGMENT_LENGTH;

    memcpy( &act.id, ds_ptr, 1 );
    memcpy( &act.transaction, ds_ptr + 1, 1 );

    act.payload = ds_ptr + ACTION_FLOW_INFO_SEGMENT_LENGTH;


}
