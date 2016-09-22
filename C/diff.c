
#include "diff.h"

entity entite;
idAlreadySee idms;
int eybgBool;

char *id_app = "DIFF####";

void envoiUdp(char * tampon){
  char *message = calloc(512,sizeof(char));
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  struct addrinfo *first_info;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype=SOCK_DGRAM;
  char * port = malloc(10);
  sprintf(port,"%d",entite.nextudpPort);
  int r=getaddrinfo(entite.nextip,port,&hints,&first_info);
  if(r==0){
    if(first_info!=NULL){
      struct sockaddr *saddr=first_info->ai_addr;
      strcat(message,"APPL");
      strcat(message," ");
      char *id = calloc(8,sizeof(char));
      strncpy(id,generateIdm(),8);
      strcat(message,id);
      strcat(message," ");
      strcat(message,id_app);
      strcat(message," ");
      strcat(message,to3Bytes(strlen(tampon)));
      strcat(message," ");
      strcat(message,tampon);
      idms.idtab[idms.number] = calloc(8,sizeof(char));
      strncpy(idms.idtab[idms.number++] ,id,8);
      writeMessage_nE(message);
      sendto(sock,message,512,0,saddr,sizeof(struct sockaddr_in));
    }
  }
  close(sock);
}


void * listenudpapp(void * arg){
  int sock=*((int *)arg);
  struct sockaddr_in emet;
  socklen_t a=sizeof(emet);
  while(1){
    char *tampon = calloc(512, sizeof(char));
    int rec=recvfrom(sock,tampon,512,0,(struct sockaddr *)&emet,&a);
    if(rec != -1){
      char *ret = calloc(8,sizeof(char *));
      tampon[rec]='\0';
      if((ret = isGoodAppFormatDIFF(tampon,id_app)) != NULL){
	       if(isIn(idms.idtab,idms.number,ret) == 0){
           idms.idtab[idms.number] = calloc(8,sizeof(char));
           strncpy(idms.idtab[idms.number++],ret,8);
	         udpsend(tampon);
	         writeMessage_nR(tampon);
           writeMessage_nC(tampon);
	       }else
          writeMessage_nF(tampon);
      }else{
        control(tampon);
      }
      ret = NULL;
    }
    tampon = NULL;
  }
  return NULL;
}


void *app_init_udp(void *arg){
  int sockudpreceiver=socket(PF_INET,SOCK_DGRAM,0);
  sockudpreceiver=socket(PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in address_sock;
  address_sock.sin_family=AF_INET;
  address_sock.sin_port=htons(entite.udpPort);
  address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
  int c=bind(sockudpreceiver,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
  if(c==0){
    pthread_create(&udpreceiving,NULL,listenudpapp,&sockudpreceiver);
    pthread_join(udpreceiving,NULL);
  }else
    exit(0);
  return NULL;
}

void *menuApp(void *arg){
  printf("***********************************************************\n");
  printf("                           DIFF                            \n");
  printf("***********************************************************\n");
  printf("Typage d'envoi de Message : [Votre message]                \n");
  printf("***********************************************************\n");
  print_menu();
  while(1){
    char *tampon = calloc(485,sizeof(char));
    int i;
    for(i=0; (tampon[i] = getchar())!='\n'; i++){
      ;
    }
    tampon[i]='\0';
    if(strlen(tampon) > 1 && tampon[0] == '[' && tampon[strlen(tampon)-1] == ']'){
      tampon[strlen(tampon)-1] = '\0';
      envoiUdp(tampon+1);
    }else{
      choixmenu(tampon);
    }
  }
  return NULL;
}
