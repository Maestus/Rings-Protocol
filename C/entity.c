/*************************

Import de la structure
Entity qui est définie
selon le Poly du projet

*************************/


#include "entity.h"

extern entity entite;
extern idAlreadySee idms;
extern char * app;

/*************************************************

Fonction auxiliaire pas encore definie proprement
Mais qui doit normalement servir à inclure une
nouvelle entitée

*************************************************/



void * listeningmulti(void * arg){
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  sock=socket(PF_INET,SOCK_DGRAM,0);
  int ok=1;
  setsockopt(sock,SOL_SOCKET,SO_REUSEPORT,&ok,sizeof(ok));
  struct sockaddr_in address_sock;
  address_sock.sin_family=AF_INET;
  address_sock.sin_port=htons(entite.multicastport);
  address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr=inet_addr(entite.multicastPbReseau);
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
  char tampon[4];
  while(1){
    recv(sock,tampon,4,0);
    if(strcmp(tampon,"DOWN") == 0){
      writeMessage_nR(tampon);
      if(entite.doubleur){
        char *ipduff = getNext(downRing,2);
        char *portdiff = getNext(downRing,3);
        if(strncmp(ipduff,to15Bytes(entite.multicastPbReseau),15) == 0 && atoi(portdiff) == entite.multicastport){
            strcpy(entite.nextip,my_ip);
            entite.nextudpPort = entite.udpPort;
        }else{
          entite.nextipdupl = NULL;
        }
      }else{
      exit(0);
      }
    }
  }
  return 0;
}


void udpsend(char *tampon){
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  struct addrinfo *first_info;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype=SOCK_DGRAM;
  char *port = calloc(4, sizeof(char));
  sprintf(port,"%d",entite.nextudpPort);
  int r=getaddrinfo(entite.nextip,port,&hints,&first_info);
  if(r==0){
    if(first_info!=NULL){
      struct sockaddr *saddr=first_info->ai_addr;
      sendto(sock,tampon,strlen(tampon),0,saddr,sizeof(struct sockaddr_in));
    }
  }
  close(sock);
  if(entite.nextipdupl != NULL){
    int sock2=socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *first_info2;
    struct addrinfo hints2;
    memset(&hints2, 0, sizeof(struct addrinfo));
    hints2.ai_family = AF_INET;
    hints2.ai_socktype = SOCK_DGRAM;
    char *port2 = calloc(4, sizeof(char));
    sprintf(port2,"%d",entite.nextudpPortdupl);
    int r=getaddrinfo(entite.nextipdupl,port2,&hints2,&first_info2);
    if(r==0){
      if(first_info!=NULL){
        struct sockaddr *saddr2=first_info2->ai_addr;
        sendto(sock2,tampon,strlen(tampon),0,saddr2,sizeof(struct sockaddr_in));
      }
    }
    close(sock2);
  }
}

void control(char *tampon) {
  char *ret = malloc(4*sizeof(char));
  if((ret = isGoodFormat(tampon)) != NULL){
     if(nativeCmd(ret,tampon) == 1){
        if(strcmp(ret,"APPL") == 0){
          writeMessage_nC(tampon);
        }else if(strcmp(ret,"WHOS") == 0){
            writeMessage_nR(tampon);
            writeMessage_nC(tampon);
           sendInfo();
        }else if(strcmp(ret,"MEMB") == 0){
           writeMessage_nR(tampon);
           writeMessage_nC(tampon);
        }else if(strcmp(ret,"GBYE") == 0){
          if(cmpIPPort(tampon) != 1){
            writeMessage_nC(tampon);
          }
        }else if(strcmp(ret,"TEST") == 0){
          writeMessage_nR(tampon);
          downRing = calloc(34,sizeof(char));
          strncpy(downRing,tampon,34);
          writeMessage_nC(tampon);
        }else if(strcmp(ret,"EYBG") == 0 && eybgBool){
          writeMessage_nR(tampon);
          idms.number = 0;
          strcpy(entite.nextip,my_ip);
          entite.nextudpPort = entite.udpPort;
          eybgBool = 0;
        }
        udpsend(tampon);
     }
   }
}


void * listeningudp(void* arg){
  int sock = *((int *) arg);
  struct sockaddr_in emet;
  socklen_t a=sizeof(emet);
  while(1){
    char tampon[512];
    int rec=recvfrom(sock,tampon,512,0,(struct sockaddr *)&emet,&a);
    if(rec != -1){
      tampon[rec]= '\0';
      control(tampon);
    }
  }
  return NULL;
}

void *listeningtcp(void * arg){
  pthread_t insert;
  int sock=*((int *)arg);
  struct sockaddr_in caller;
  socklen_t si=sizeof(caller);
  while(1){
    int sock2=accept(sock,(struct sockaddr *)&caller,&si);
    if(sock2!=-1){
      pthread_create(&insert,NULL,affiche,&sock2);
    }
  }
  return NULL;
}


void init_entity(int argc, char * argv[]){

  struct hostent *HostEntPtr;
  struct in_addr in;
  char Hostname[100];

  gethostname( Hostname, sizeof(Hostname) );
  HostEntPtr = gethostbyname( Hostname );

  if ( HostEntPtr != NULL ){
    memcpy( &in.s_addr, *HostEntPtr->h_addr_list, sizeof(in.s_addr) );
  }

  if(argc > 1){
    desc = calloc(1024,sizeof(char));
    entite.doubleur = 0;
    struct sockaddr_in sa;
    if(argc == 6){
      entite.identity = malloc(8*sizeof(char));
      entite.identity = argv[1];
      if(atoi(argv[2]) > MAX_UDP_PORT_LISTEN || atoi(argv[5]) > MAX_PORT_MULTICAST){
        writeMessage_nI("Les ports udp doivent etre inferieur à 9999");
        exit(-1);
      }
      entite.udpPort = atoi(argv[2]);
      entite.nextudpPort = atoi(argv[2]);
      entite.tcpPort = atoi(argv[3]);
      entite.nextip = calloc(15,sizeof(char));
      strcpy(entite.nextip,inet_ntoa(in));
      entite.multicastPbReseau = calloc(15,sizeof(char));
      if(inet_pton(AF_INET, argv[4], &(sa.sin_addr)) == 1){
        char * tmp = calloc(3,sizeof(char));
        strcpy(tmp,argv[4]);
        char * ipport = strtok(tmp,".");
        if(atoi(ipport) > 223 && atoi(ipport) < 240) {
          strcpy(entite.multicastPbReseau,argv[4]);
        } else {
          writeMessage_nI("Error: Wrong IP for multicast");
          exit(-1);
        }
      } else {
        writeMessage_nI("Error: Not a IP for multicast");
        exit(-1);
      }
      entite.multicastport = atoi(argv[5]);
      entite.nextipdupl = NULL;
    }else if(argc == 5){
      entite.identity = malloc(8*sizeof(char));
      entite.identity = "Anonymus";
      if(atoi(argv[1]) > MAX_UDP_PORT_LISTEN || atoi(argv[4]) > MAX_PORT_MULTICAST){
        writeMessage_nI("Les ports udp doivent etre inferieur à 9999");
        exit(-1);
      }
      entite.udpPort = atoi(argv[1]);
      entite.nextudpPort = atoi(argv[1]);
      entite.tcpPort = atoi(argv[2]);
      entite.nextip = calloc(15,sizeof(char));
      entite.multicastPbReseau = calloc(15,sizeof(char));
      strcpy(entite.nextip,inet_ntoa(in));
      if(inet_pton(AF_INET, argv[3], &(sa.sin_addr)) == 1){
        char * tmp = calloc(3,sizeof(char));
        strcpy(tmp,argv[3]);
        char * ipport = strtok(tmp,".");
        if(atoi(ipport) > 223 && atoi(ipport) < 240){
          strcpy(entite.multicastPbReseau,argv[3]);
        } else {
          writeMessage_nI("Error: Wrong IP for multicast");
          exit(-1);
        }
      } else {
        writeMessage_nI("Error: Not an IP for multicast");
        exit(-1);
      }
      entite.multicastport = atoi(argv[4]);
      entite.nextipdupl = NULL;
    }else{
      writeMessage_nI("Typer : ./entity [identifiant] UDP_Port TCP_Port IP_MULTI_DIFFUSION UDP_MULTI_DIFFUSION");
      exit(-1);
    }
    my_ip = malloc(15*sizeof(char));
    my_ip = inet_ntoa(in);
    sprintf(desc,"Votre entité a été initialisée avec les attributs suivants : \nIdentity : %s\nIP : %s          IP SUIVANTE : %s\nIP MULTI : %s    Port MULTI : %d\nPort UDP : %d               Port UDP SUIVANT : %d\nPort TCP : %d\n",entite.identity,to15Bytes(my_ip),to15Bytes(entite.nextip),to15Bytes(entite.multicastPbReseau),entite.multicastport,entite.udpPort,entite.nextudpPort,entite.tcpPort);
    writeMessageI(desc);
  }else{
    writeMessage_nI("Typer : ./entity [identifiant] UDP_Port TCP_Port IP_MULTI_DIFFUSION UDP_MULTI_DIFFUSION");
    exit(-1);
  }
}


void updateDesc(){
  sprintf(desc,"Votre entité a été initialisée avec les attributs suivants : \nIdentity : %s\nIP : %s          IP SUIVANTE : %s\nIP MULTI : %s    Port MULTI : %d\nPort UDP : %d               Port UDP SUIVANT : %d\nPort TCP : %d\n",entite.identity,to15Bytes(my_ip),to15Bytes(entite.nextip),to15Bytes(entite.multicastPbReseau),entite.multicastport,entite.udpPort,entite.nextudpPort,entite.tcpPort);
}


void *init_tcp(void *arg){
  int sock;
  struct sockaddr_in sockaddress;

  bzero(&sockaddress,sizeof(sockaddress));
  sockaddress.sin_family=AF_INET;
  sockaddress.sin_port=htons(entite.tcpPort);
  sockaddress.sin_addr.s_addr=htonl(INADDR_ANY);

  if ((sock = socket(AF_INET,SOCK_STREAM,0))== -1){
    writeMessageI("création raté");
    exit(0);
  }

  int r=bind(sock,(struct sockaddr *)&sockaddress,sizeof(struct sockaddr_in));

  if(r==0){
    r=listen(sock,0);
    if(r==0){
      listeningtcp(&sock);
    }
  }else{
    writeMessageI("PROBLEME DE BIND\n");
    exit(0);
  }

  return NULL;
}


void *init_udp(void * arg){
  int sockudpreceiver=socket(PF_INET,SOCK_DGRAM,0);
  sockudpreceiver=socket(PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in address_sock;
  address_sock.sin_family=AF_INET;
  address_sock.sin_port=htons(entite.udpPort);
  address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
  int c=bind(sockudpreceiver,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
  if(c==0){
    pthread_create(&udpreceiving,NULL,listeningudp,&sockudpreceiver);
    pthread_join(udpreceiving,NULL);
  }
  else
    exit(0);
  return NULL;
}


void * choixmenu(void *arg){
  char *choix = ((char *)arg);
  char *token;
  if(strcmp(choix,"") == 0) {
    return NULL;
  }
  if(strcmp((token = strtok(choix," ")),"connect") == 0){
    char *ip = calloc(15,sizeof(char));
    int port;
    token = strtok(NULL," ");
    if(token != NULL){
      ip = token;
      token = strtok(NULL," ");
      if(token != NULL){
	       port = atoi(token);
      }
      else{
	       writeMessageI("port non indiquer\n");
      }
    }else{
      writeMessageI("il faut donner l'ip suivi du port\n");
    }
    struct sockaddr_in sa4;
    if(inet_pton(AF_INET, ip, &(sa4.sin_addr)) == 1){
      sa4.sin_port = htons(port);
      sa4.sin_family = AF_INET;
      inet_aton(ip,&sa4.sin_addr);
      int socktcp=socket(PF_INET,SOCK_STREAM,0);
      int r=connect(socktcp,(struct sockaddr *)&sa4,sizeof(struct sockaddr_in));
      if(r!=-1){
        char *buff = calloc(47,sizeof(char));
        char *ackc = calloc(5,sizeof(char));
        char *mess = calloc(26,sizeof(char));
      	recv(socktcp,buff,47*sizeof(char),0);
      	writeMessageR(buff);
        if(strncmp(buff,"NOTC\n",5) == 0){
          close(socktcp);
          return NULL;
        }
      	sprintf(mess,"NEWC %s %d\n",to15Bytes(entite.nextip), entite.udpPort);
        writeMessageE(mess);
      	write(socktcp,mess,26);
      	recv(socktcp,ackc,5,0);
      	writeMessageR(ackc);
        insert(buff);
        free(buff);
        free(mess);
        free(ackc);
        close(socktcp);
      }else{
      	writeMessageI("Personne n'écoute\n");
      	return NULL;
      }
    }else{
      writeMessageI("Adresse ip non conforme\n");
      return NULL;
    }
  } else if(strcmp(token,"dupl") == 0){
    char *ip = calloc(15,sizeof(char));
    int port;
    token = strtok(NULL," ");
    if(token != NULL){
      ip = token;
      token = strtok(NULL," ");
      if(token != NULL){
	       port = atoi(token);
      }
      else{
	       writeMessageI("port non indiquer\n");
      }
    }else{
      writeMessageI("il faut donner l'ip suivi du port\n");
    }
    struct sockaddr_in sa4;
    if(inet_pton(AF_INET, ip, &(sa4.sin_addr)) == 1){
      sa4.sin_port = htons(port);
      sa4.sin_family = AF_INET;
      inet_aton(ip,&sa4.sin_addr);
      int socktcp=socket(PF_INET,SOCK_STREAM,0);
      int r=connect(socktcp,(struct sockaddr *)&sa4,sizeof(struct sockaddr_in));
      if(r!=-1){
        char *buff = calloc(47,sizeof(char));
        char *ackd = calloc(10,sizeof(char));
        char *mess = calloc(47,sizeof(char));
      	recv(socktcp,buff,47*sizeof(char),0);
      	writeMessageR(buff);
        if(strncmp(buff,"NOTC\n",5) == 0){
          close(socktcp);
          return NULL;
        }
      	sprintf(mess,"DUPL %s %d %s %d\n",to15Bytes(my_ip), entite.udpPort, to15Bytes(entite.multicastPbReseau), entite.multicastport);
        writeMessageE(mess);
      	write(socktcp,mess,47);
      	recv(socktcp,ackd,10,0);
      	writeMessageR(ackd);
        char*token;
        token = strtok(ackd, " ");
        token = strtok(NULL, " ");
        entite.nextudpPort = atoi(token);
        free(buff);
        free(mess);
        free(ackd);
        close(socktcp);
      }else{
      	writeMessageI("Personne n'écoute\n");
      	return NULL;
      }
    }else{
      writeMessageI("Adresse ip non conforme\n");
      return NULL;
    }
  }else if(strcmp(choix,"WHOS") == 0){
    char *message = calloc(13, sizeof(char));
    const char * cmd = "WHOS";
    strncat(message,cmd,4);
    strncat(message," ",1);
    char *id = calloc(8,sizeof(char));
    id = generateIdm();
    strncat(message,id,8);
    idms.idtab[idms.number] = calloc(8,sizeof(char));
    strncpy(idms.idtab[idms.number++],id,8);
    writeMessage_nE(message);
    udpsend(message);
  }else if(strcmp(choix,"GBYE") == 0){
    char *message = calloc(55,sizeof(char));
    eybgBool = 1;
    const char * cmd = "GBYE";
    strncat(message,cmd,4);
    strcat(message," ");
    char *id = calloc(8,sizeof(char));
    id = generateIdm();
    strcat(message,id);
    strcat(message," ");
    strcat(message,to15Bytes(my_ip));
    strcat(message," ");
    char *port = calloc(4,sizeof(char));
    sprintf(port,"%d",entite.udpPort);
    strncat(message,port,4);
    strncat(message," ",1);
    strncat(message,to15Bytes(entite.nextip),15);
    strncat(message," ",1);
    char *nextport = calloc(4,sizeof(char));
    sprintf(nextport,"%d",entite.nextudpPort);
    strncat(message,nextport,4);
    idms.idtab[idms.number] = calloc(8,sizeof(char));
    strncpy(idms.idtab[idms.number++],id,8);
    writeMessage_nE(message);
    udpsend(message);
  }else if(strcmp(choix,"TEST") == 0){
    char *message = calloc(34,sizeof(char));
    const char * cmd = "TEST";
    strncat(message,cmd,4);
    strncat(message," ",1);
    char *id = calloc(8,sizeof(char));
    id = generateIdm();
    strncat(message,id,8);
    strncat(message," ",1);
    strncat(message,to15Bytes(entite.multicastPbReseau),15);
    strcat(message," ");
    char *port = calloc(4,sizeof(char));
    sprintf(port,"%d",entite.multicastport);
    strncat(message,port,4);
    idms.idtab[idms.number] = calloc(8,sizeof(char));
    strncpy(idms.idtab[idms.number++],id,8);
    writeMessage_nE(message);
    udpsend(message);
    test = 1;
    now = time(0);
    start = localtime (&now);
    waitingstart = start->tm_sec;
  }else if(strcmp(choix,"DOWN") == 0){
    writeMessage_nE("DOWN");
    udpsendmulti();
  }else if(atoi(choix) == 3){
    updateDesc();
    writeMessageI(desc);
  }
  return NULL;
}


void * menu(void * arg){
  while(1){
    char * choix = calloc(100,sizeof(char));
    int i;
    for(i=0; (choix[i] = getchar())!='\n'; i++){
      ;
    }
    choix[i]='\0';
    choixmenu(choix);
  }
  return NULL;
}


void udpsendmulti(){
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  struct addrinfo *first_info;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype=SOCK_DGRAM;
  char *port=calloc(4,sizeof(char));
  sprintf(port,"%d",entite.multicastport);
  int r=getaddrinfo(entite.multicastPbReseau,port,NULL,&first_info);
  if(r==0){
    if(first_info!=NULL){
      struct sockaddr *saddr=first_info->ai_addr;
      sendto(sock,"DOWN",4,0,saddr,sizeof(struct sockaddr_in));
    }
  }
  close(sock);
}



void * testingRing(void *arg){
  while (1) {
    while(!test){
    }
    int elapsed;
    while(test){
        now2 = time(0);
        end = localtime (&now2);
        if((elapsed = abs(end->tm_sec - waitingstart)) > 3){
          writeMessageI("BROKEN\n");
          test = 0;
          udpsendmulti();
        }
    }
  }
  return NULL;
}
