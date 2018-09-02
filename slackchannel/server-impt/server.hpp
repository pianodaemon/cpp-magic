#ifndef __SLACKCHANNEL_SERVER_HPP__
#define __SLACKCHANNEL_SERVER_HPP__

#include <boost/asio.hpp>

namespace SlackChannel
{
    class Server
    {

        public:

            Server( boost::asio::io_service&, const boost::asio::ip::tcp::endpoint& );

        private:

            boost::asio::io_service& m_io_service;
            boost::asio::ip::tcp::acceptor m_acceptor;
    };
}

#endif /* __SLACKCHANNEL_SERVER_HPP__ */
