#include "insertions_native.h"

extern entity entite;


void insert(char *info){
  char *ip = getNext(info,1);
  char *port = getNext(info,2);
  char *ipmulti = getNext(info,3);
  char *portmulti = getNext(info,4);
  strcpy(entite.nextip,ip);
  entite.nextudpPort = atoi(port);

  if(ipmulti != NULL && portmulti != NULL){
    entite.multicastPbReseau = NULL;
    entite.multicastPbReseau = calloc(15,sizeof(char));
    strcpy(entite.multicastPbReseau,ipmulti);
    entite.multicastport = atoi(portmulti);
  }

  pthread_create(&multi,NULL,listeningmulti,NULL);

  free(ip);
  free(ipmulti);
  ipmulti = NULL;
  portmulti = NULL;
  port = NULL;
}


void insertDupl(char * info){
  char *ip = getNext(info,1);
  char *port = getNext(info,2);
  char *ipmulti = getNext(info,3);
  char *portmulti = getNext(info,4);

  entite.nextipdupl = calloc(15,sizeof(char));
  strncpy(entite.nextipdupl,ip,15);
  entite.nextudpPortdupl = atoi(port);


  entite.multicastPbReseaudupl = calloc(15,sizeof(char));
  strncpy(entite.multicastPbReseaudupl,ipmulti,15);
  entite.multicastportdupl = atoi(portmulti);


  free(ip);
  free(ipmulti);
  ipmulti = NULL;
  portmulti = NULL;
  port = NULL;
}
