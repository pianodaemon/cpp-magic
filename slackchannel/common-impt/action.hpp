#ifndef __SLACKCHANNEL_ACTION_HPP__
#define __SLACKCHANNEL_ACTION_HPP__

#include "frame.hpp"
#include <inttypes.h>


namespace SlackChannel
{
    struct Action
    {
        uint8_t id;
        uint8_t transaction;
        char* payload;
    };

    void putActionInto( Frame*, Action*, size_t );
}

#endif /*  __SLACKCHANNEL_ACTION_HPP__ */
