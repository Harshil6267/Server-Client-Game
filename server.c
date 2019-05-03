#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

static int client_count = 0;

void myfunction(int client, int client_number)
{
  int cpoints[2], Spoints[2];
  cpoints[0]=0;
  cpoints[1]=0;
  Spoints[0]=0;
  Spoints[1]=0;
  char message[255];


  //===================== SENDING WELCOME MESSAGE =======================//
  //========================= WRITE OPERATION ===========================//
  write(client, "Welcome to the Game Arena, be my guest and start the game",60);


  while(1)
  {
    //==================== READING CLIENT SCORE ====================// 
    //==================== READ OPERATION ==========================//
    if(read(client, &cpoints, sizeof(cpoints))<0)
    {
      fprintf(stderr, "read() error\n");
      exit(3);
    }
    if(cpoints[0] == 0)
    {
      // printf("Client %d has left the game.\n",client_number);
      printf("\n\n=====================================================================================\n");
      printf("=========================== Client %d has left the game. =============================\n",client_number);
      printf("=====================================================================================\n\n");
      close(client);
      exit(5);      
    }
    printf("\n\t\t\t#### Client %d has roll the dice ###\n",client_number);
    printf("\t\t\t\t\t\t\t\t::: Client score :::\n");
    printf("\t\t\t\t\t\t\t\tClient dice value : %d\n",cpoints[0]);
    printf("\t\t\t\t\t\t\t\tTotal client score : %d\n\n\n",cpoints[1]);


    //==================== CHECKING CLIENT SCORE ==================//
    //==================== WRITE OPERATION ========================//
    if(cpoints[1] >= 100)
    {
      write(client,"Game over: You won the game.",50);
      printf("Client won!!!\n");
      printf("\n\n=====================================================================================\n");
      printf("================ Client %d has shut down the connection with server. =================\n",client_number);
      printf("=====================================================================================\n\n");      
      close(client);
      exit(5);
    }


    //=================== SERVER ROLLONG DICE AND SENDING ITS SCORE TO CLIENT ====================//
    //===================================== WRITE OPERATION ======================================//
    Spoints[0] = (rand() % 6)+1;
    Spoints[1]+=Spoints[0];
    printf("::: Server score :::\n");
    printf("Server dice value: %d\n", Spoints[0]);
    printf("Total server score : %d\n", Spoints[1]);
    write(client, &Spoints, sizeof(Spoints));


    //=================== CHECKING SERVER SCORE ==================//
    //====================== WRITE OPERATION =====================//
    if(Spoints[1] >= 100)
    {
      write(client,"Game over: You lost the game.",50);      
      printf("Server won!!! yahoooo.. I never lose :)\n");
      printf("\n\n=====================================================================================\n");
      printf("================ Client %d has shut down the connection with server. =================\n",client_number);
      printf("=====================================================================================\n\n");            
      close(client);
      exit(5);
    }


    //================= SENDING MESSAGE FOR NEXT ROUND ====================//
    //========================= WRITE OPERATION ===========================//
    write(client,"Game on: you can now play your dice",50);
    printf("\n\n=====================================================================================\n");
    printf("=====================================================================================\n\n");
  }
    exit(1); 
}

int main(int argc, char *argv[]){  

  srand(time(NULL));
  pid_t p;
  int n;
  int sd, client, portNumber;
  socklen_t len;
  char *currentTime;
  time_t currentUnixTime;

  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, NULL, NULL);
    client_count++;    
    printf("\n\n=====================================================================================\n");
    printf("== Client #%d is available, it's time to create new child & let him play with client==\n",client_count);
    printf("=====================================================================================\n\n");    
    if(!fork())
    {
      myfunction(client, client_count);
    }    
    // wait(&n);
  }
    close(client);  
}