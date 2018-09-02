#include "server.hpp"


SlackChannel::Server::Server( boost::asio::io_service& io_service,
    const boost::asio::ip::tcp::endpoint& endpoint ) :
    m_io_service( io_service ), m_acceptor( io_service, endpoint )
{

}
