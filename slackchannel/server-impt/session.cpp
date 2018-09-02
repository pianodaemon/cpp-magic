#include "session.hpp"

SlackChannel::Session::Session( boost::asio::io_service& io_service ) : m_socket( io_service )
{

}

void SlackChannel::Session::gear( void )
{

}

boost::asio::ip::tcp::socket&
SlackChannel::Session::get_socket()
{
    return this->m_socket;
}
