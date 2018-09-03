#include "session.hpp"

SlackChannel::Session::Session( boost::asio::io_service& io_service ) : m_socket( io_service )
{
    this->m_reader_ptr = new Reader( this->m_socket,
        boost::bind( &SlackChannel::Session::incomming, shared_from_this(),
            boost::placeholders::_1, boost::placeholders::_2 ) );
}

SlackChannel::Session::~Session()
{

    delete this->m_reader_ptr;
}

void
SlackChannel::Session::gear( void )
{
    this->m_reader_ptr->gear();

}

boost::asio::ip::tcp::socket&
SlackChannel::Session::get_socket()
{
    return this->m_socket;
}

void
SlackChannel::Session::incomming( const char* buffer, size_t len )
{

}
