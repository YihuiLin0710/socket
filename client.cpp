#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

struct sockaddr_in serv_addr;
struct hostent *server;


struct numbers {
    std::vector <struct sub> digit;
    int input;
    int argc1;
    char **argv1;
};
struct sub{
    char Digit;
    std::string output;
    int argc1;
    char **argv1;
};

char inttochar(int T)
{
    switch(T)
    {
        default:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';    
    }
}

void firstdigit (int number, std::vector<struct sub>& target,int argc,char** argv)
{
    if(number>=10)
        firstdigit(number/10,target,argc,argv);
    struct sub Temp;
    int T=number%10;
    Temp.Digit=inttochar(T);
    Temp.argc1=argc;
    Temp.argv1=argv;
    target.push_back(Temp);
}

void *Gchild(void *Target)//code reference by client.c from blackboard.
{
    int sockfd,port;
    
    struct sub* target= (struct sub *)Target;
    char buffer[256];
    server = gethostbyname(target->argv1[1]);
    port=atoi(target->argv1[2]);

    if(server == NULL)
    std::cout<<"ERROR with server"<<std::endl;

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if (sockfd < 0)
        std::cout<<"ERROR opening socket C";


    bzero(buffer,sizeof(buffer));
    buffer[0]=target->Digit;
    
 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port=htons(port);

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
        std::cout<<"ERROR connecting C"<<std::endl;

    if(send(sockfd, buffer, strlen(buffer), 0)<0)
        std::cout<<"ERROR writing"<<std::endl;

    bzero(buffer,sizeof(buffer));
    if(read(sockfd,buffer,255)<0)
        std::cout<<"ERROR reading C";

    target->output=buffer;
    close(sockfd);
    return NULL;
}

void *child(void * number)
{
    struct numbers *T = (struct numbers *)number;
    int count=(log10(T->input)+1);
    pthread_t Ctid[count];
    
    firstdigit(T->input,T->digit,T->argc1,T->argv1);

    for(int i=0;i<count;i++)
    {
        pthread_create(&Ctid[i],NULL,Gchild,(void *)&T->digit[i]);
    }
    for(int i=0;i<count;i++)
    {
        pthread_join(Ctid[i], NULL);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int msg[256];
    int i=0;
    while(std::cin>>msg[i])
        i++;
    struct numbers package[i];
    pthread_t tid[i];
    for(int j=0;j<i;j++)
    {   
        package[j].argc1=argc;
        package[j].argv1=argv;
        package[j].input=msg[j];
        pthread_create(&tid[j],NULL,child,(void *)&package[j]);
    }
    for(int j=0;j<i;j++)
    {
        pthread_join(tid[j], NULL);
        for (int l= 0; l < package[j].digit.size(); l++)
        {
            std::cout<<package[j].digit[l].output<<std::endl;
        }
        std::cout<<std::endl;
    }
    return 0;   
}
