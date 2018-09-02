#ifndef __SLACKCHANNEL_READER_HPP__
#define __SLACKCHANNEL_READER_HPP__

#include "common-impt.hpp"
#include "frame.hpp"
#include <functional>


namespace SlackChannel
{
    using DataReceiver = std::function<void(const char* , size_t)>;

    class Reader : public boost::enable_shared_from_this<Reader>
    {
        public:

            Reader( boost::asio::ip::tcp::socket& socket, DataReceiver& receiver );
            ~Reader();

            void operator() (void);

        private:

            void gear();
            void fetch_head( const boost::system::error_code& error );
            void fetch_body( const boost::system::error_code& error );

            Frame m_pivot;
            boost::asio::ip::tcp::socket& m_socket;
            DataReceiver m_receiver;
    };

    using ReaderPtr = boost::shared_ptr<Reader>;
}


#endif /* __SLACKCHANNEL_READER_HPP__ */
