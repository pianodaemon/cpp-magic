#ifndef __SLACKCHANNEL_WRITER_HPP__
#define __SLACKCHANNEL_WRITER_HPP__

#include "common-impt.hpp"
#include "frame.hpp"
#include <deque>


namespace SlackChannel
{
    class Writer
    {
        public:

            Writer( boost::asio::ip::tcp::socket& );
            ~Writer();

        private:

            void deliver( Frame&& );
            void flush( const boost::system::error_code& );

            std::deque<Frame> m_qwrite;
            boost::mutex m_mutex;
            boost::asio::ip::tcp::socket& m_socket;
    };

}


#endif /* __SLACKCHANNEL_WRITER_HPP__ */
