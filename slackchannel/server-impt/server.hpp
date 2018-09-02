#ifndef __SLACKCHANNEL_SERVER_HPP__
#define __SLACKCHANNEL_SERVER_HPP__

#include "session.hpp"


namespace SlackChannel
{
    class Server
    {

        public:

            Server( boost::asio::io_service&, const boost::asio::ip::tcp::endpoint& );
            void start_accept( void );
        private:

            void handle_accept( SessionPtr, const boost::system::error_code& );
            boost::asio::io_service& m_io_service;
            boost::asio::ip::tcp::acceptor m_acceptor;
    };
}

#endif /* __SLACKCHANNEL_SERVER_HPP__ */
