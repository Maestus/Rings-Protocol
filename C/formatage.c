#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "formatage.h"



char *isGoodAppFormatTRANS(char *string, char *idapp){
  char *str = strdup(string);
  char *str_copy = malloc(492);
  strcpy(str_copy,str);
  char *token;
  int count = 0;
  //int size = 0;
  char *res = malloc(8*sizeof(char));
  while ((token = strsep(&str_copy, " "))){
    if(count == 0 && (strcmp(token,"APPL") != 0))
      return NULL;
    if(count == 1 && sizeof(token) != 8)
      return NULL;
    else if(count == 1 && sizeof(token) == 8)
      res=token;
    /*if(count == 2 && (sizeof(token) != 8 || (strcmp(token,idapp) != 0))){
      return NULL;
    }
    if(count == 3 && sizeof(token) != ((size_t)8)){
      return NULL;
    }
    else if(count == 3 && sizeof(token) == ((size_t)8)){
      size = atoi(token);
      if(strcmp(idapp,"TRANS###") == 0)
	     break;
    }
    if(count == 4 && (int)strlen(token) != size){
      return NULL;
    }*/
    count++;
  }
  /*if(strcmp(idapp,"TRANS###" == 0){
    if((int)strlen(str_copy) == size){
      return res;
    }
    else
      return NULL;
  }*/
  free(str);
  free(str_copy);
  /*if(count != 5)
    return NULL;*/
  return res;
}



char* isGoodAppFormatDIFF(char *string, char *idapp){
  if(idapp == NULL){
    isGoodFormat(string);
  }
  char *str = strdup(string);
  char *str_copy = malloc(492);
  strcpy(str_copy,str);
  char *token;
  int count = 0;
  int size = 0;
  char *res = malloc(8*sizeof(char));
  while ((token = strsep(&str_copy, " "))){
    if(count == 0 && (strcmp(token,"APPL") != 0))
      return NULL;
    if(count == 1 && sizeof(token) != 8)
      return NULL;
    else if(count == 1 && sizeof(token) == 8)
      res=token;
    if(count == 2 && (sizeof(token) != 8 || (strcmp(token,idapp) != 0))){
      return NULL;
    }
    if(count == 3 && sizeof(token) != ((size_t)8)){
      return NULL;
    }
    else if(count == 3 && sizeof(token) == ((size_t)8)){
      size = atoi(token);
      if(strcmp(idapp,"DIFF####") == 0)
	break;
    }
    if(count == 4 && (int)strlen(token) != size){
      return NULL;
    }
    count++;
  }
  if(strcmp(idapp,"DIFF####") == 0){
    if((int)strlen(str_copy) == size){
      return res;
    }
    else
      return NULL;
  }
  free(str);
  free(str_copy);
  if(count != 5)
    return NULL;
  return res;
}

char* isGoodFormat(char *string){
  char *str = strdup(string);
  char *str_copy = malloc(10);
  strcpy(str_copy,str);
  char *token;
  token = strsep(&str_copy, " ");
  if(strcmp(token,"WHOS") == 0){
    return "WHOS";
  }
  if(strcmp(token,"MEMB") == 0){
    return "MEMB";
  }
  if(strcmp(token,"GBYE") == 0){
    return "GBYE";
  }
  if(strcmp(token,"EYBG") == 0){
    return "EYBG";
  }
  if(strcmp(token,"TEST") == 0){
    return "TEST";
  }
  if(strcmp(token,"APPL") == 0){
    return "APPL";
  }
  free(str);

  return NULL;
}

int isIn(char *tab[4096], int size, char *id){
  if(tab == NULL){
    return 0;
  }
  for(int i = 0; i < size; i++){
    //printf("tab[%d] = %s\n",i,tab[i]);
    if(tab[i] != NULL){
      if(strcmp(tab[i],id) == 0){
	       return 1;
      }
    }
  }
  return 0;
}
