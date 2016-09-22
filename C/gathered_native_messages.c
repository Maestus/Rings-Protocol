#include "gathered_native_messages.h"

extern idAlreadySee idms;

int nativeCmd(char *type,char *tampon){
  if(type == NULL)
    return 0;
  char *token;
  char* tmp = calloc(strlen(tampon)+1, sizeof(char));
  strcpy(tmp, tampon);
  if(strcmp(type,"WHOS") == 0){
    token = strtok(tmp," ");
    token = strtok(NULL," ");
    if(token != NULL && sizeof(token) == 8)
      if(isIn(idms.idtab,idms.number,token) == 0){
        idms.idtab[idms.number] = calloc(8,sizeof(char));
        strncpy(idms.idtab[idms.number++],token,8);
	      return 1;
     }
     writeMessage_nF(tampon);
  }else if(strcmp(type,"MEMB") == 0){
    token = strtok(tmp," ");
    token = strtok(NULL," ");
    if(token != NULL && sizeof(token) == 8)
      if(isIn(idms.idtab,idms.number,token) == 0){
        idms.idtab[idms.number] = calloc(8,sizeof(char));
        strncpy(idms.idtab[idms.number++],token,8);
	      return 1;
      }
      writeMessage_nF(tampon);
  }else if(strcmp(type,"GBYE") == 0){
    token = strtok(tmp," ");
    token = strtok(NULL," ");
    if(token != NULL && sizeof(token) == 8)
      if(isIn(idms.idtab,idms.number,token) == 0){
        idms.idtab[idms.number] = calloc(8,sizeof(char));
        strncpy(idms.idtab[idms.number++],token,8);
	      return 1;
      }
      writeMessage_nF(tampon);
  }else if(strcmp(type,"EYBG") == 0){
    token = strtok(tmp," ");
    token = strtok(NULL," ");
    if(token != NULL && sizeof(token) == 8)
      if(isIn(idms.idtab,idms.number,token) == 0){
        idms.idtab[idms.number] = calloc(8,sizeof(char));
        strncpy(idms.idtab[idms.number++],token,8);
	      return 1;
      }
      writeMessage_nF(tampon);
  }else if(strcmp(type,"TEST") == 0){
    token = strtok(tmp," ");
    token = strtok(NULL," ");
    if(token != NULL && sizeof(token) == 8){
      if(isIn(idms.idtab,idms.number,token) == 0){
        idms.idtab[idms.number] = calloc(8,sizeof(char));
        strncpy(idms.idtab[idms.number++],token,8);
	      return 1;
      }else{
        test = 0;
        writeMessage_nF(tampon);
      }
    }
  }else if(strcmp(type,"APPL") == 0){
    token = strtok(tmp," ");
    token = strtok(NULL," ");
    if(token != NULL && sizeof(token) == 8){
      if(isIn(idms.idtab,idms.number,token) == 0){
        idms.idtab[idms.number] = calloc(8,sizeof(char));
        strncpy(idms.idtab[idms.number++],token,8);
	      return 1;
      }else{
        test = 0;
        writeMessage_nF(tampon);
      }
    }
  }
  return 0;
}
