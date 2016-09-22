#include "envoi_auto_native.h"

extern entity entite;
extern idAlreadySee idms;

int cmpIPPort(char *tampon){
  char *token;
  char* tmp = calloc(strlen(tampon)+1, sizeof(char));
  strcpy(tmp, tampon);
  token = strtok(tmp," ");
  token = strtok(NULL," ");
  token = strtok(NULL," ");
  if(strcmp(token,to15Bytes(entite.nextip)) == 0){
    token = strtok(NULL," ");
    if(atoi(token) == entite.nextudpPort){
      writeMessage_nR(tampon);
      char *message = calloc(13, sizeof(char));
      strncat(message,"EYBG",4);
      strncat(message," ",1);
      char *ip = calloc(8,sizeof(char));
      strncpy(ip,generateIdm(),8);
      strncat(message,ip,8);
      idms.idtab[idms.number] = calloc(8,sizeof(char));
      strncpy(idms.idtab[idms.number++] ,ip,8);
      writeMessage_nE(message);
      udpsend(message);
      token = strtok(NULL," ");
      strcpy(entite.nextip,token);
      token = strtok(NULL," ");
      entite.nextudpPort = atoi(token);
      return 1;
    }
  }
  return 0;
}

void sendInfo(){
  char *message = calloc(43,sizeof(char));
  char appl[4] = "MEMB";
  strncat(message,appl,4);
  strncat(message," ",1);
  char *ip = calloc(8,sizeof(char));
  strncpy(ip,generateIdm(),8);
  strncat(message,ip,8);
  strncat(message," ",1);
  strncat(message,entite.identity,8);
  strncat(message," ",1);
  strncat(message,to15Bytes(my_ip),15);
  strncat(message," ",1);
  char *port = calloc(4,sizeof(char));
  sprintf(port,"%d",entite.udpPort);
  strcat(message,port);
  idms.idtab[idms.number] = calloc(8,sizeof(char));
  strncpy(idms.idtab[idms.number++],ip,8);
  writeMessage_nE(message);
  udpsend(message);
}


void *affiche(void * arg){
  int sockcomm=*((int *)arg);
  if(entite.doubleur){
    write(sockcomm,"NOTC\n",5);
    close(sockcomm);
    return NULL;
  }
  char *mess = calloc(47,sizeof(char));
  char *ip = to15Bytes(entite.nextip);
  char *ipmulti = to15Bytes(entite.multicastPbReseau);
  sprintf(mess,"WELC %s %d %s %d\n",ip,entite.nextudpPort,ipmulti,entite.multicastport);
  writeMessageE(mess);
  write(sockcomm,mess,47*sizeof(char));
  char *buff = calloc(47,sizeof(char));
  read(sockcomm,buff,47*sizeof(char));
  writeMessageR(buff);
  char *str_copy = calloc(47,sizeof(char));
  strcpy(str_copy,buff);
  char *token;
  token = strsep(&str_copy, " ");
  if(strncmp(token,"DUPL",4) == 0){
    insertDupl(buff);
    char *ackd = calloc(10,sizeof(char));
    sprintf(ackd,"ACKD %d\n",entite.udpPort);
    writeMessageE(ackd);
    write(sockcomm,ackd,10);
    free(ackd);
    entite.doubleur = 1;
  }else{
    insert(buff);
  }
  writeMessage_nE("ACKC");
  write(sockcomm,"ACKC\n",5);
  close(sockcomm);
  free(mess);
  free(buff);
  free(ip);
  free(ipmulti);
  ip = NULL;
  return NULL;
}
