#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>


int main(int argc, char *argv[])
{
  srand(time(NULL));
  signal(SIGINT, SIG_IGN);
  char message[100];
  int server, portNumber;
  socklen_t len;
  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
  }

 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }


  int Cpoints[2], spoints[2];
  Cpoints[0]=0;
  Cpoints[1]=0;
  spoints[0]=0;
  spoints[1]=0;
  char ch;
  //================= REDING WELCOME MESSAGE ==================//
  //===================== READ OPERATION ======================//
  if (read(server, message, 100)<0){
    fprintf(stderr, "read() error\n");
    exit(3); 
  }
  printf("\n\n=======================================================================\n");
  printf("====== %s ======\n", message);
  printf("=======================================================================\n");

  while(1)
  {
    //=================== CLIENT ROLLONG DICE AND SENDING ITS SCORE TO SERVER ====================//
    //===================================== WRITE OPERATION ======================================//
    printf("\n------------------ Press the Enter key to roll Dice -------------------\n");
    printf("---------------------- To Quit a game press 'q' -----------------------\n");
    
    while(1)
    {
      ch=fgetc(stdin);
      
      if(ch==0x0A)
      {
        Cpoints[0] = (rand() % 6)+1;
        Cpoints[1]+=Cpoints[0];
        break;
      }
      else if(ch=='q')
      {
        Cpoints[0]=0;
        Cpoints[1]=0;
        write(server, &Cpoints, sizeof(Cpoints));        
        printf("\n\n=======================================================================\n");
        printf("=============== I am quiting this game, Good bye server. ==============\n");
        printf("=======================================================================\n\n\n");
        exit(7);        
      }
      else
      {
        Cpoints[0]=0;
        printf("To roll dice press only Enter key");
      }
    }    

    printf("\n::: Client score :::\n");
    printf("Client dice value : %d\n", Cpoints[0]);
    printf("Total client score : %d\n", Cpoints[1]);
    printf("\n\n\n");
    write(server, &Cpoints, sizeof(Cpoints));


    //==================== CHECKING CLIENT SCORE ==================//
    //======================== READ OPERATION =====================//
    if(Cpoints[1] >= 100)
    {
      if (read(server, &message, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      printf("\n**********************************************************************\n");      
      printf("******** Message From server :: %s *********\n",message);
      printf("**************** Okay bye server, see you soon!!! ********************");
      printf("\n**********************************************************************\n\n\n");
      close(server);
      exit(5);
    }
    

    //==================== READING SERVER SCORE ====================// 
    //======================== READ OPERATION ======================//    
    if (read(server, &spoints, sizeof(spoints))<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }
    printf("\t\t\t\t\t\t::: Server score :::\n");    
    printf("\t\t\t\t\t\tServer dice value :%d \n",spoints[0]);
    printf("\t\t\t\t\t\tTotal server score : %d\n",spoints[1]);


    //==================== CHECKING SERVER SCORE ==================//
    //======================== READ OPERATION =====================//
    if(spoints[1] >= 100)
    {
      if (read(server, &message, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      printf("\n**********************************************************************\n");      
      printf("******** Message From server :: %s *********\n",message);
      printf("**************** Okay bye server, see you soon!!! ********************");
      printf("\n**********************************************************************\n\n\n");
      close(server);
      exit(5);
    }


    //================= READING MESSAGE FOR NEXT ROUND ===================//
    //========================= READ OPERATION ===========================//
    if (read(server, &message, 50)<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }
    printf("\n=======================================================================\n");
    printf("***** Message From server :: %s ******",message);
    printf("\n=======================================================================\n");
  }

  exit(0);
}
