#include "getteurs_message.h"

char * getIdm(char *tampon){
  char *token;
  if(tampon != NULL){
    token = strtok(tampon, " ");
    if(strcmp(token,"WHOS") == 0)
      return strtok(NULL," ");
  }
  return NULL;
}

char * getNext(char *buff,int pos){
  char *str_copy = malloc(512*sizeof(char));
  strcpy(str_copy,buff);
  char *token;
  int count = 1;
  while ((token = strsep(&str_copy, " "))){
    if((pos + 1) == 2 && count == 2){
      str_copy = NULL;
      return convertIp(token);
    }
    if((pos + 1) == 3 && count == 3){
      str_copy = NULL;
      return token;
    }
    if((pos + 1) == 4 && count == 4){
      str_copy = NULL;
      return convertIp(token);
    }
    if((pos + 1) == count){
      str_copy = NULL;
      return token;
    }
    count++;
  }free(str_copy);
  return NULL;
}
