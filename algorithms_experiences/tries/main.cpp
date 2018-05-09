#include "tries.hpp"


int main( int argc , char *argv[] )
{
//        std::cout << slack_telnet( "127.0.0.1" , 23  );
    TriePtr t_ptr = nullptr;

    trie_insert( &t_ptr, "hola" );

    trie_search( t_ptr, "hola" );
    return 0;

}
