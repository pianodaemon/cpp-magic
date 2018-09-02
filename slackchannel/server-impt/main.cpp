#include "server.hpp"
#include <iostream>


int main( int argc , char *argv[] )
{
    if ( argc < 2 )
    {
        std::cerr << "Usage: server <port> [<port> ...]\n";
        return 1;
    }

    boost::asio::io_service io_service;

    try
    {
        using boost::asio::ip::tcp;

        tcp::endpoint endpoint( tcp::v4(), std::atoi( argv[1] ) );

        SlackChannel::Server server( io_service, endpoint );

        io_service.run();
    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
