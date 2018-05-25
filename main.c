#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>   
#include <netdb.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <error.h>
#include <net/route.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT 19
#define MAXDATASIZE 100

    int      main(int argc, char *argv[])
    {
              int sockfd;
              struct sockaddr_in server;
              struct sockaddr_in client;
              struct sockaddr_in addrto;
              socklen_t addrlen;
              int num;
              int so_broadcast=1;
              char buf[MAXDATASIZE];
			  struct ifreq if_eth0;
			  
			  
              bzero(&addrto, sizeof(struct sockaddr_in));
			  
			  
              addrto.sin_family=AF_INET;
              addrto.sin_addr.s_addr=inet_addr("255.255.255.255");
              addrto.sin_port=htons(10666);
			  
              if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
              {
                  perror("Creatingsocket failed.");
                  exit(1);
              }
              setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));
			  
			  
			  
			  strcpy(if_eth0.ifr_name,"eth0");
			  if(setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char*)&if_eth0, sizeof(if_eth0))<0) {
			     printf("bind socket to eth0 error\n");
			  }
			  
			  bzero(&server,sizeof(server));
              server.sin_family=AF_INET;
              server.sin_port=htons(PORT);
              server.sin_addr.s_addr= htonl (INADDR_ANY); 
			  
			  
			  
              if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
              {
                  perror("Bind()error.");
                  exit(1);
              }
              addrlen=sizeof(addrto);
              while(1)
              {
                  num =recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr*)&client,&addrlen);
                  if (num < 0)
                  {
                      perror("recvfrom() error\n");
                      exit(1);
                  }
                  buf[num] = '\0';
                  printf("You got a message (%s%) from client.\nIt's ip is%s, port is %d.\n",buf,inet_ntoa(client.sin_addr),htons(client.sin_port));
                  sendto( sockfd ,"Welcometo my server.\n",22,0,(struct sockaddr *)&addrto,addrlen);
                  if(!strcmp(buf,"bye"))
                    break;
              }
              close(sockfd);
    }
