#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>

#define SERWER_PORT 4666


#define CLIENT_IP "127.0.0.1"





int main()
{

// TO DO podział na wątki w zależności od tego czy serwer jest na IPv6 czy IPv4 
// lub jedno wpsólne nasłuchiwanie naprzemiennie dla IPv4 i IPv6

 struct sockaddr_in server =
    {
        .sin_family = AF_INET,
        .sin_port = htons( SERWER_PORT )
    };
 struct sockaddr_in server6 =
    {
        .sin_family = AF_INET6,
        .sin_port = htons( SERWER_PORT )
    };


   const int socket_ = socket( AF_INET, SOCK_DGRAM, 0 );

    
    if( socket_ < 0 )
    {
        perror( "socket() ERROR" );
        exit( 1 );
    }
    
    

    
    
    
    char buffer[ 4096 ];

    socklen_t len = sizeof( server );

    if( bind( socket_,( struct sockaddr * ) & server, len ) < 0 )
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
    struct sockaddr_in from = {};
   
    
    memset( buffer, 0, sizeof( buffer ) );
    printf( "Waiting for connection...\n" );
    
    if( recvfrom( socket_, buffer, sizeof( buffer ), 0,( struct sockaddr * ) & from, & len ) < 0 )
    {
        perror( "recvfrom() ERROR" );
        exit( 1 );
    }
    printf( "|Server's reply|: %s \n", buffer );
        char serverAdress[128] = {};
        printf( "|Client ip: %s port: %d|\n", inet_ntop( AF_INET, & from.sin_addr,serverAdress, sizeof( serverAdress ) ), ntohs( from.sin_port ) );

    if( inet_pton( AF_INET, serverAdress, & server.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
        
        strncpy( buffer, "nawiazano polaczenie", sizeof( buffer ) );
        
    if( sendto( socket_, buffer, strlen( buffer ), 0,( struct sockaddr * ) & from, len ) < 0 )
        {
            perror( "sendto() ERROR" );
            exit( 5 );
        }
        
    
    
     shutdown( socket_, SHUT_RDWR );
}
// gcc client.cpp -g -Wall -o client && ./client