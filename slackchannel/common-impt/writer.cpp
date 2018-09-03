#include "writer.hpp"

SlackChannel::Writer::Writer( boost::asio::ip::tcp::socket& soc ) : m_socket( soc )
{

}

SlackChannel::Writer::~Writer()
{

}


void
SlackChannel::Writer::deliver( Frame&& msg )
{
    this->m_mutex.lock();

    bool write_in_progress = !this->m_qwrite.empty();
    this->m_qwrite.push_back( msg );

    this->m_mutex.unlock();

    if ( !write_in_progress )
    {
        boost::asio::async_write( this->m_socket,
            boost::asio::buffer( this->m_qwrite.front().get_frame_ptr(),
                this->m_qwrite.front().get_frame_len()),
                boost::bind(&SlackChannel::Writer::flush, this,
                    boost::asio::placeholders::error));
    }
}


void
SlackChannel::Writer::flush( const boost::system::error_code& error )
{

    if ( !error )
    {
        this->m_mutex.lock();

        this->m_qwrite.pop_front();
        bool isnot_empty = !m_qwrite.empty();

        this->m_mutex.unlock();

        if ( isnot_empty )
        {
            boost::asio::async_write( this->m_socket ,
                boost::asio::buffer( this->m_qwrite.front().get_frame_ptr() ,
                    this->m_qwrite.front().get_frame_len() ),
                    boost::bind( &SlackChannel::Writer::flush, this,
                    boost::asio::placeholders::error ) );
        }

    }
}
