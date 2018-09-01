#ifndef __SLACKCHANNEL_READER_HPP__
#define __SLACKCHANNEL_READER_HPP__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "frame.hpp"

namespace SlackChannel
{
    class Reader : public boost::enable_shared_from_this<Reader>
    {
        public:

            Reader( boost::asio::ip::tcp::socket& socket );
            ~Reader();

        private:

            void operator() (void);
            void gear();
            void fetch_head( const boost::system::error_code& error );
            void fetch_body( const boost::system::error_code& error );

            Frame m_pivot;
            boost::asio::ip::tcp::socket& m_socket;
    };

}


#endif /* __SLACKCHANNEL_READER_HPP__ */
