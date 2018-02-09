#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>


#define TCP_STUFF_SUCCEED           0
#define TCP_STUFF_SEND_ERROR       -5001
#define TCP_STUFF_POLL_ERROR       -5002
#define TCP_STUFF_SRV_CONN_CLOSED  -5003
#define TCP_STUFF_RECV_ERROR       -5004
#define TCP_STUFF_CAN_NOT_CONNECT  -5005

#define __BI_BUF_LEN 20

#define DO 0xfd
#define WONT 0xfc
#define WILL 0xfb
#define DONT 0xfe
#define IAC 0xff

#define CMD_WINDOW_SIZE 31

typedef unsigned char byte_chunck;

static int
negotiate( int sock, byte_chunck* buf )
{
    int rc = TCP_STUFF_SUCCEED;
 
    if ( buf[1] == DO && buf[2] == CMD_WINDOW_SIZE )
    {
        byte_chunck cmd_1[] = { 255, 251, 31 };
        if ( send(sock, cmd_1, sizeof(cmd_1) / sizeof(cmd_1[0]) , 0) < 0 ) rc = TCP_STUFF_SEND_ERROR;
         
        byte_chunck cmd_2[] = { 255, 250, 31, 0, 80, 0, 24, 255, 240 };
        if ( send(sock, cmd_2, sizeof(cmd_2) / sizeof(cmd_2[0]) , 0) < 0 ) rc = TCP_STUFF_SEND_ERROR;
    }
    else
    {
        int i = 0;
        do
        {
            i++;
            if ( buf[i] == DO )
            {
                buf[i] = WONT;
                continue;
            }

            if ( buf[i] == WILL ) buf[i] = DO;         

        } while( i < 3 );
 
        if ( send(sock, buf, 3 , 0) < 0 ) rc = TCP_STUFF_SEND_ERROR;
    }

    return rc;
}

static int
init_conn( int* fd , const char* host_ip, const int& host_port, struct sockaddr_in* sa_ptr )
{
    int rc = -1;

    for(;;)
    {
        if ( ( *fd = socket( AF_INET , SOCK_STREAM , 0 ) ) == -1 ) break;

        sa_ptr->sin_addr.s_addr = inet_addr( host_ip );
        sa_ptr->sin_family = AF_INET;
        sa_ptr->sin_port = htons( host_port );

        if ( connect( *fd , ( struct sockaddr * )sa_ptr , sizeof( struct sockaddr_in ) ) < 0 ) break;

        rc = TCP_STUFF_SUCCEED;
        break;
    }

    return rc;
}

static int 
scan_io_channels( const int& sock_fd , const int& kb_fd )
{
    int rc = TCP_STUFF_SUCCEED;
    struct timeval ts;

    ts.tv_sec = 1; 
    ts.tv_usec = 0;
    
    byte_chunck* buf = new byte_chunck[ __BI_BUF_LEN + 1 ];
 
    for(;;)
    {
        fd_set read_set;
        FD_ZERO( &read_set );
        FD_SET( sock_fd , &read_set );
        FD_SET( kb_fd , &read_set );
        
        int nready = select( sock_fd + 1 , &read_set, nullptr, nullptr, &ts );
        
        if ( nready < 0 ) 
        {
            rc = TCP_STUFF_POLL_ERROR;
            break;
        }
        
        if ( nready == 0 ) continue;
        
        if ( FD_ISSET( sock_fd , &read_set ) )
        {
            // start by reading a single byte
            int ngot = recv( sock_fd , buf , 1 , 0 );

            if ( ngot == 0 )
            {
                rc = TCP_STUFF_SRV_CONN_CLOSED;
                break;
            }

            if ( ngot < 0 )
            {
                rc = TCP_STUFF_RECV_ERROR;
                break;
            }

            if ( buf[0] == IAC )
            {
                // read 2 more bytes
                ngot = recv( sock_fd, buf + 1 , 2 , 0 );

                if ( ngot == 0 )
                {
                    rc = TCP_STUFF_SRV_CONN_CLOSED;
                    break;
                }
                
                if ( ngot < 0 )
                {
                    rc = TCP_STUFF_RECV_ERROR;
                    break;
                }

                if ( ( rc = negotiate( sock_fd, buf ) ) < 0 ) break;
            }
            else
            {
                buf[1] = 0;
                std::cout << buf;
                fflush( nullptr );
            }

            continue;
        }
        
        if ( FD_ISSET( kb_fd, &read_set ) )
        {
             if ( read( kb_fd, buf, 1 ) == 1 )
             {
                  if ( send( sock_fd, buf, 1, 0 ) < 0 )
                  {
                      rc = TCP_STUFF_SEND_ERROR;
                      break;
                  }

                  // Raw terminal needs to force a LF
                  if ( buf[0] == '\n' ) putchar('\r');
             }
        }
    }

    delete buf;
 
    return rc; 
}


extern int
slack_telnet( const char* host_ip , const unsigned int host_port  )
{
    int rc = 0;

    enum nvt_states
    {
        NVT_POWER_ON,
        NVT_SET_TERM_RAW,
        NVT_RESTORE_TERM,
        NVT_EXPECTING_DATA,
        NVT_SHUTDOWN
    };

    struct sockaddr_in server;
    enum nvt_states ms = NVT_POWER_ON;
    static struct termios tin, tlocal;
    int sfd;

    unsigned int getoff = false;

    for(;;)
    {
        switch( ms )
        {
            case NVT_POWER_ON:
            {
                if ( init_conn( &sfd , host_ip, host_port, &server ) < 0 )
                {
                    ms = NVT_SHUTDOWN;
                    rc = TCP_STUFF_CAN_NOT_CONNECT; 
                }
                else
                {
                    ms = NVT_SET_TERM_RAW;
                }

                break;
            }
            case NVT_SET_TERM_RAW:
            {
                //back up terminal initial mode
                tcgetattr( STDIN_FILENO, &tin );
                memcpy( &tlocal, &tin, sizeof( tin ) );

                //set terminal raw mode up
                cfmakeraw( &tlocal );
                tcsetattr( STDIN_FILENO, TCSANOW, &tlocal );
                ms = NVT_EXPECTING_DATA;

                break;
            }
            case NVT_EXPECTING_DATA:
            {
                rc = scan_io_channels( sfd , STDIN_FILENO );
                close( sfd );
                ms = NVT_RESTORE_TERM;

                break;
            }
            case NVT_RESTORE_TERM:
            {
                tcsetattr( STDIN_FILENO, TCSANOW, &tin );
                ms = NVT_SHUTDOWN;

                break;
            }
            case NVT_SHUTDOWN:
            {
                getoff = 1;
                break;
            }
        }
        
        if ( getoff ) break;
    }

    return rc;
}

int main(int argc , char *argv[])
{
    std::cout << slack_telnet( "127.0.0.1" , 23  );
    return 0;    
}
