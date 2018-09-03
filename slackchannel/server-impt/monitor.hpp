#ifndef __SLACKCHANNEL_MONITOR_HPP__
#define __SLACKCHANNEL_MONITOR_HPP__

#include "server-impt.hpp"
#include "common-impt/action.hpp"

namespace SlackChannel
{
    class Monitor
    {
        public:

            Monitor();
            ~Monitor();

        private:

            void incomming( const char*, size_t );

    };

}

#endif /*  __SLACKCHANNEL_MONITOR_HPP__ */
