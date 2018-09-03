#include "reader.hpp"

SlackChannel::Reader::Reader( boost::asio::ip::tcp::socket& soc, DataReceiver rec ) : m_socket( soc ), m_receiver( rec )
{

}

SlackChannel::Reader::~Reader()
{

}

void
SlackChannel::Reader::gear()
{
    boost::asio::async_read( this->m_socket,
        boost::asio::buffer( m_pivot.get_frame_ptr(),
                             SlackChannel::FRAME_HEADER_LENGTH ),
        boost::bind( &SlackChannel::Reader::fetch_head,
            this, boost::asio::placeholders::error ) );
}

void
SlackChannel::Reader::fetch_head( const boost::system::error_code& error )
{
    if ( !error && this->m_pivot.dec_header() )
    {
        boost::asio::async_read( this->m_socket,
            boost::asio::buffer( this->m_pivot.get_data_seg_ptr(),
                                 this->m_pivot.get_data_seg_len() ),
            boost::bind( &SlackChannel::Reader::fetch_body, this,
                    boost::asio::placeholders::error ) );
    }

}

void
SlackChannel::Reader::fetch_body( const boost::system::error_code& error )
{
    if ( !error )
    {
        this->m_receiver( this->m_pivot.get_data_seg_ptr(),
                          this->m_pivot.get_data_seg_len() );

        this->gear();
    }
}
