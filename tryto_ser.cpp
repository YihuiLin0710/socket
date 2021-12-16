#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/wait.h>


struct sockaddr_in serv_addr, cli_addr;

void fireman(int)//function code from blackboard
{
   while (waitpid(-1, NULL, WNOHANG) > 0);
}


std::string working(char info)
{
    switch (info){
        default:
            return "0 = 1 1 1 1 1 1 0";
            break;
        case '1':
            return "1 = 0 1 1 0 0 0 0";
            break;
        case '2':
            return "2 = 1 1 0 1 1 0 1";
            break;
        case '3':
            return "3 = 1 1 1 1 0 0 1";
            break;
        case '4': 
            return "4 = 0 1 1 0 0 1 1";
            break;
        case '5': 
            return "5 = 1 0 1 1 0 1 1";
            break;
        case '6': 
            return "6 = 1 0 1 1 1 1 1";
            break;
        case '7': 
            return "7 = 1 1 1 0 0 0 0";
            break;
        case '8': 
            return "8 = 1 1 1 1 1 1 1";
            break;
        case '9': 
            return "9 = 1 1 1 1 0 1 1"; 
            break;
    }
}



int main(int argc, char *argv[])//code reference by server.c from blackboard.
{
    char msg[256];
    char msg1[256];
    int sock,newsock,port,clilen;

//socking
    sock = socket(AF_INET, SOCK_STREAM,0);
    if (sock < 0) 
        std::cout<<"ERROR opening socket S";

//info
    bzero((char *) &serv_addr, sizeof(serv_addr));
    port=atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr= INADDR_ANY; 
    serv_addr.sin_port=htons(port);

//binding
    if(bind(sock, (struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
        std::cout<<"ERROR binding S";

//listening
    if(listen(sock,5)<0)
        std::cout<<"ERROR listening S";

//accepting
    clilen = sizeof(cli_addr);
    signal(SIGCHLD, fireman); 
    while(true)
    {
        newsock=accept(sock,(struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
        if(fork()==0)
        {
            if(newsock<0)
                std::cout<<"ERROR accepting S";

//reading
            bzero(msg,256);
            if(read(newsock,msg,255)<0)
                std::cout<<"ERROR reading S";
            char buff=*(msg);

//writing
            bzero(msg,256);
            bzero(msg1,256);
            strcpy(msg1,working(buff).c_str());
            if(write(newsock,msg1,strlen(msg1))<0)
                std::cout<<"ERROR writing S";
            close(newsock);
            _exit(0);
        }
    }
    wait(0);
    close(sock);
    return 0;
}