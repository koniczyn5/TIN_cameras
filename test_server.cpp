#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>

#define SERWER_PORT 4668
#define CLIENT_PORT 4666
#define SERWER_IP "127.0.0.1"
#define CLIENT_IP "127.0.0.1"

int main()
{
    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
        .sin_port = htons( SERWER_PORT )
    };
    struct sockaddr_in client =
    {
        .sin_family = AF_INET,
        .sin_port = htons( CLIENT_PORT )
    };
    
    if( inet_pton( AF_INET, SERWER_IP, & serwer.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
    if( inet_pton( AF_INET, CLIENT_IP, & client.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }


    const int socket_ = socket( AF_INET, SOCK_DGRAM, 0 );
    if(( socket_ ) < 0 )
    {
        perror( "socket() ERROR" );
        exit( 2 );
    }
    
    char buffer[ 4096 ] = "Message for client";
    
    socklen_t len = sizeof( serwer );
    /*
   if( bind( socket_,( struct sockaddr * ) & serwer, len ) < 0 )
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
    
    */
        if( sendto( socket_, buffer, strlen( buffer ), 0,( struct sockaddr * ) & client, len ) < 0 )
        {
            perror( "sendto() ERROR" );
            exit( 5 );
        }
        memset( buffer, 0, sizeof( buffer ) );
        if( recvfrom( socket_, buffer, sizeof( buffer ), 0,( struct sockaddr * ) & client, & len ) < 0 )
        {
            perror( "recvfrom() ERROR" );
            exit( 4 );
        }
        printf( "|Message from client|: %s \n", buffer );
    /*
    while( 1 )
    {
        struct sockaddr_in client = { };
        
        memset( buffer, 0, sizeof( buffer ) );
        
        printf( "Waiting for connection...\n" );
        if( recvfrom( socket_, buffer, sizeof( buffer ), 0,( struct sockaddr * ) & client, & len ) < 0 )
        {
            perror( "recvfrom() ERROR" );
            exit( 4 );
        }
        printf( "|Message from client|: %s \n", buffer );
        
        char buffer_ip[ 128 ] = { };
        printf( "|Client ip: %s port: %d|\n", inet_ntop( AF_INET, & client.sin_addr, buffer_ip, sizeof( buffer_ip ) ), ntohs( client.sin_port ) );
        
        strncpy( buffer, "Message for client", sizeof( buffer ) );
        
        if( sendto( socket_, buffer, strlen( buffer ), 0,( struct sockaddr * ) & client, len ) < 0 )
        {
            perror( "sendto() ERROR" );
            exit( 5 );
        }
    }
    */
    shutdown( socket_, SHUT_RDWR );
}
// gcc server.cpp -g -Wall -o server && ./server