#include "server.hpp"
#include <boost/bind.hpp>


SlackChannel::Server::Server( boost::asio::io_service& io_service,
    const boost::asio::ip::tcp::endpoint& endpoint ) :
    m_io_service( io_service ), m_acceptor( io_service, endpoint )
{

}

void
SlackChannel::Server::start_accept( void )
{
    SessionPtr new_session_ptr( new Session( this->m_io_service ) );

    this->m_acceptor.async_accept( new_session_ptr->get_socket(),
        boost::bind( &Server::handle_accept, this, new_session_ptr,
            boost::asio::placeholders::error ) );
}

void
SlackChannel::Server::handle_accept( SessionPtr session_ptr, const boost::system::error_code& error )
{
    if ( !error )
    {
        session_ptr->gear();
    }

    this->start_accept();
}
