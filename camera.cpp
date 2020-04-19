//-- Camera
//-- Autor: Maciej Puchalski
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>


int main(int argc, char *argv[])
{
if(argc < 2)
{
    fprintf(stderr, "Use: \"%s port\"", argv[0]);
	exit(1);
}
int gatePort = atoi(argv[1]);

// TO DO podział na wątki w zależności od tego czy bramka jest na IPv6 czy IPv4 
// lub jedno wspólne nasłuchiwanie naprzemiennie dla IPv4 i IPv6

 struct sockaddr_in gate =
    {
        .sin_family = AF_INET,
        .sin_port = htons( gatePort )
    };
 struct sockaddr_in gate6 =
    {
        .sin_family = AF_INET6,
        .sin_port = htons( gatePort )
    };


   const int socket_ = socket( AF_INET, SOCK_DGRAM, 0 );

    
    if( socket_ < 0 )
    {
        perror( "socket() ERROR" );
        exit( 1 );
    }
    
    

    
    
    
    char buffer[ 4096 ];

    socklen_t len = sizeof( gate );

    if( bind( socket_,( struct sockaddr * ) & gate, len ) < 0 )
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
    printf( "|Camera reply|: %s \n", buffer );
        char gateAdress[128] = {};
        printf( "|Client ip: %s port: %d|\n", inet_ntop( AF_INET, & from.sin_addr,gateAdress, sizeof( gateAdress ) ), ntohs( from.sin_port ) );

    if( inet_pton( AF_INET, gateAdress, & gate.sin_addr ) <= 0 )
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
// gcc camera.cpp -g -Wall -o camera && ./camera