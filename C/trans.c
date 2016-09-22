
#include "trans.h"

entity entite;
idAlreadySee idms;
paquet *paquets;

int sockreceiveudp;

char *id_app = "TRANS###";

char *filesearched;
char *id_transfert;
int nb_envoi;

void envoiUdp(char * tampon){
  char *message = calloc(512,sizeof(char));
  char len[3];
  sprintf(len,"%lu",strlen(tampon));
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  struct addrinfo *first_info;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype=SOCK_DGRAM;
  char * port = calloc(4,sizeof(char));
  sprintf(port,"%d",entite.nextudpPort);
  int r=getaddrinfo(entite.nextip,port,&hints,&first_info);
  if(r==0){
    if(first_info!=NULL){
      struct sockaddr *saddr=first_info->ai_addr;
      strcat(message,"APPL");
      strcat(message," ");
      char *id = calloc(8,sizeof(char));
      id = generateIdm();
      strcat(message,id);
      strcat(message," ");
      strcat(message,id_app);
      strcat(message," ");
      strcat(message,"REQ");
      strcat(message," ");
      strcat(message,to2Bytes(len));
      strcat(message," ");
      strcat(message,tampon);
      idms.idtab[idms.number] = calloc(8,sizeof(char));
      strncpy(idms.idtab[idms.number++] ,id,8);
      filesearched = calloc(strlen(tampon),sizeof(char));
      strcpy(filesearched,tampon);
      writeMessage_nE(message);
      sendto(sock,message,512,0,saddr,sizeof(struct sockaddr_in));
    }
  }
  close(sock);
}


int files(){
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if(d){
    while((dir = readdir(d)) != NULL){
      if((strcmp(dir->d_name,".") != 0) && (strcmp(dir->d_name,"..") != 0))
	printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return(0);
}


void sendSEN(char *file, char *id){
  FILE *fp;
  fp = fopen(file, "r");
  char *s = calloc(492,sizeof(char));
  size_t nread;
  int i = 1;
  while((nread = fread(s, 1, 463, fp)) > 0){
    if(nread<463) s[nread] = '\0';
    char * message = calloc(512,sizeof(char));
    strcat(message,"APPL");
    strcat(message," ");
    char *identifiant = calloc(8,sizeof(char));
    identifiant = generateIdm();
    strcat(message,identifiant);
    strcat(message," ");
    strcat(message,id_app);
    strcat(message," ");
    strcat(message,"SEN");
    strcat(message," ");
    strcat(message,id);
    strcat(message," ");
    char * stringNomess = calloc(8,sizeof(char));
    sprintf(stringNomess,"%d",i);
    strncat(message,to8Bytes(stringNomess),8);
    strcat(message," ");
    strcat(message,to3Bytes(nread));
    strcat(message," ");
    strcat(message,s);
    writeMessage_nE(message);
    udpsend(message);
    usleep(5000);
    i++;
  }
  //fclose(fp);
}



void sendROK(char * file, int nummess){
  char *message = calloc(512,sizeof(char));
  char *len = calloc(3,sizeof(char));
  sprintf(len,"%lu",strlen(file));
  char *nombre = calloc(nummess,sizeof(char));
  sprintf(nombre,"%d",nummess);
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  struct addrinfo *first_info;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype=SOCK_DGRAM;
  char * port = calloc(4, sizeof(char));
  sprintf(port,"%d",entite.nextudpPort);
  int r=getaddrinfo(entite.nextip,port,&hints,&first_info);
  if(r==0){
    if(first_info!=NULL){
      struct sockaddr *saddr=first_info->ai_addr;
      strcat(message,"APPL");
      strcat(message," ");
      char *id = calloc(8,sizeof(char));
      id = generateIdm();
      strcat(message,id);
      strcat(message," ");
      strcat(message,id_app);
      strcat(message," ");
      strcat(message,"ROK");
      strcat(message," ");
      strcat(message,id);
      strcat(message," ");
      strcat(message,to2Bytes(len));
      strcat(message," ");
      strcat(message,file);
      strcat(message," ");
      strcat(message,nombre);
      idms.idtab[idms.number] = calloc(8,sizeof(char));
      strncpy(idms.idtab[idms.number++] ,id,8);
      sendto(sock,message,512,0,saddr,sizeof(struct sockaddr_in));
      sendSEN(file,id);
    }
  }
  close(sock);
}


int existingFile(char *tampon){
  char *name = calloc(99,sizeof(char));
  name = getNext(tampon,5);
  if(access(name,F_OK) != -1 ) {
    writeMessage_nR(tampon);
    struct stat st;
    stat(name, &st);
    int size = st.st_size;
    double nbpart = ((double)size)/492;
    int nb = (int)nbpart;
    if(nbpart>nb){
      nb = nb+1;
    }
    sendROK(name,nb);
    name = NULL;
    return 1;
  } else {
    name = NULL;
    return 0;
  }
}


char * getSize(char * tampon){
  char *str_copy2 = calloc(512,sizeof(char));
  strcpy(str_copy2,tampon);
  char *token  = calloc(2,sizeof(char));
  int count = 1;
  while ((token = strsep(&str_copy2, " "))){
    if(count == 7){
      str_copy2 = NULL;
      return token;
    }
    count++;
  }
  return "NONE";
}


char * getContent(char *tampon){
  return tampon+49;
}

char * getType(char *tampon){
  char *str_copy = malloc(512*sizeof(char));
  strcpy(str_copy,tampon);
  char *token;
  int count = 1;
  while ((token = strsep(&str_copy, " "))){
    if(count == 4){
      str_copy = NULL;
      return token;
    }
    count++;
  }
  return "NONE";
}


void getFile(){
  char *filename = calloc(99,sizeof(char));
  sprintf(filename,"%s_%s",id_transfert,filesearched);
  FILE *fp;
  fp = fopen(filename, "w");
  int i = 0;
  int taille_paquets = nb_envoi*1024;
  printf("%d\n",taille_paquets);
  paquets = calloc(taille_paquets,sizeof(char));
  char * newfile = calloc(8+strlen(filesearched),sizeof(char));
  sprintf(newfile,"%s%s",filesearched,id_transfert);
  int sock=sockreceiveudp;
  struct sockaddr_in emet;
  socklen_t a=sizeof(emet);
  while(i <= nb_envoi){
    char *tampon = calloc(512 ,sizeof(char));
    int rec=recvfrom(sock,tampon,512,0,(struct sockaddr *)&emet,&a);
    if(rec != -1){
      writeMessage_nR(tampon);
      char * content = calloc(463,sizeof(char));
      content = getContent(tampon);
      fputs(content,fp);
      i++;
    }
  }
  fclose(fp);
  writeMessage_nI("Fichier reçu");
}


void * listenudpapp(void * arg){
  int sock=*((int *)arg);
  struct sockaddr_in emet;
  socklen_t a=sizeof(emet);
  while(1){
    char *tampon = calloc(512 , sizeof(char));
    int rec=recvfrom(sock,tampon,512,0,(struct sockaddr *)&emet,&a);
    if(rec != -1){
      char *ret = malloc(4*sizeof(char *));
      tampon[rec]='\0';
      if((ret = isGoodAppFormatTRANS(tampon,id_app)) != NULL){
	       if(isIn(idms.idtab,idms.number,ret) == 0){
           idms.idtab[idms.number] = calloc(8,sizeof(char));
           strncpy(idms.idtab[idms.number++],ret,8);
           if(strcmp(getType(tampon),"REQ") == 0){
             if(existingFile(tampon) == 0){
               writeMessage_nC(tampon);
               udpsend(tampon);
             }
           }else if(strcmp(getType(tampon),"ROK") == 0){
             if(strcmp(getNext(tampon,6),filesearched) == 0){
               id_transfert = calloc(8,sizeof(char));
               strcpy(id_transfert,getNext(tampon,4));
               nb_envoi = atoi(getNext(tampon,7));
               printf("id_trans = %s\nnbenvoi = %d\n",id_transfert,nb_envoi);
               getFile();
             }
           }
	       }writeMessage_nF(tampon);
      }else{
        control(tampon);
      }
      ret = NULL;
    }
    tampon = NULL;
  }
  return NULL;
}



void *app_init_udp(void * arg){
  int sockudpreceiver=socket(PF_INET,SOCK_DGRAM,0);
  sockudpreceiver=socket(PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in address_sock;
  address_sock.sin_family=AF_INET;
  address_sock.sin_port=htons(entite.udpPort);
  address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
  int c=bind(sockudpreceiver,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
  if(c==0){
    sockreceiveudp = sockudpreceiver;
    pthread_create(&udpreceiving,NULL,listenudpapp,&sockudpreceiver);
    pthread_join(udpreceiving,NULL);
  }else
    exit(0);
  return NULL;
}


void *menuApp(void *arg){
  printf("***********************************************************\n");
  printf("                          TRANS                            \n");
  printf("***********************************************************\n");
  printf("  - REQ namefile \n");
  printf("***********************************************************\n");
  files();
  print_menu();
  while(1){
    char *tampon = calloc(492,sizeof(char));
    int i;
    for(i=0; (tampon[i] = getchar())!='\n'; i++){
      ;
    }
    tampon[i] = '\0';
    char *token;
    char* tmp = calloc(strlen(tampon)+1, sizeof(char));
    strcpy(tmp, tampon);
    token = strtok(tmp," ");
    if(strcmp(token,"REQ") == 0){
      token = strtok(NULL," ");
      if(token != NULL){
        envoiUdp(token);
      }
    }else{
      choixmenu(tampon);
    }
  }
  return NULL;
}
