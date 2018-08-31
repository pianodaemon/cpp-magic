SlackChannel::Reader::Reader( boost::asio::io_service& io_service ) : m_socket( io_service )
{

}

void
SlackChannel::Reader::operator() ()
{

}

SlackChannel::Reader::~Reader()
{

}

void
SlackChannel::Reader::gear()
{
    boost::asio::async_read( this->m_socket,
        boost::asio::buffer( m_read_chunk.get_frame_ptr(), SlackChannel::FRAME_HEADER_LENGTH ),
        boost::bind( &SlackChannel::Reader::fetch_head,
            shared_from_this(), boost::asio::placeholders::error ) );
}

void
SlackChannel::Reader::fetch_head( const boost::system::error_code& error )
{
    if ( !error && this->m_read_chunk.dec_header() )
    {
        boost::asio::async_read( this->m_socket,
            boost::asio::buffer( this->m_read_chunk.get_data_seg_ptr(),
                                 this->m_read_chunk.get_data_seg_len() ),
            boost::bind( &SlackChannel::Reader::fetch_body, shared_from_this(),
                    boost::asio::placeholders::error ) );
    }

}

void
SlackChannel::Reader::fetch_body( const boost::system::error_code& error )
{
    if (!error)
    {
	belowlayer( this->m_read_chunk.get_data_seg_ptr(),
                    this->m_read_chunk.get_data_seg_len() );

        this->gear();
    }
}
