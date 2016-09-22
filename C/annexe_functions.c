#include "annexe_functions.h"

char* generateIdm(){
  struct timeval tv;
  gettimeofday(&tv, NULL); // get current time
  //long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
  unsigned long time_in_micros = (1000000 * tv.tv_sec + tv.tv_usec)%100000000;
  char *time = calloc(8,sizeof(char));
  sprintf(time,"%ld",time_in_micros%100000000);
  if(strlen(time) < 8){
    char *tmp = calloc(8,sizeof(char));
    for(int i = 0 ; i < 8 - strlen(time) ; i++)
      strncat(tmp,"0",1);
    strncat(tmp,time,strlen(time));
    return tmp;
  }
  return time;
}


char * to3Bytes(size_t size){
  char * tochar = calloc(3,sizeof(char));
  sprintf(tochar,"%lu",size);
  int taille = strlen(tochar);
  char * res = calloc(3,sizeof(char));
  for(int i = 0 ; i < 3 - taille; i++){
    strcat(res,"0");
  }
  strncat(res,tochar,taille);
  return res;
}

char * to8Bytes(char * num){
  char * res = calloc(8,sizeof(char));
  int taille = strlen(num);
  for(int i = 0 ; i < 8 - taille; i++){
    strcat(res,"0");
  }
  strncat(res,num,taille);
  return res;
}


char * to2Bytes(char *length){
  char *res = calloc(2,sizeof(char));
  if(atoi(length) < 10){
    sprintf(res,"0%s",length);
  }
  return res;
}



char * to15Bytes(char *string){
  char *res = calloc(3,sizeof(char));
  char *tmp = calloc(3,sizeof(char));
  char *ret = calloc(15,sizeof(char));
  char *str_copy = calloc(15,sizeof(char));
  strcpy(str_copy,string);
  char *token;
  while ((token = strsep(&str_copy, "."))){
    strncpy(tmp,token,3);
    int count = strlen(token);
    while(count < 3){
      sprintf(res,"0%s",tmp);
      strncpy(tmp,res,3);
      count++;
    }
    strcat(ret,tmp);
    if(strlen(ret) < 15)
      strncat(ret,".",1);
    if(strlen(ret) > 15){
      printf("Erreur dans la conversion ip : %s\n",ret);
      exit(-1);
    }
  }
  free(str_copy);
  return ret;
}

char * convertIp(char *ip){
  char *res = calloc(3, sizeof(char));
  char *tmp = calloc(3,sizeof(char));
  char *ret = calloc(15, sizeof(char));
  char *str_copy = calloc(15,sizeof(char));
  strcpy(str_copy,ip);
  char *token;
  while ((token = strsep(&str_copy, "."))){
    strncpy(tmp,token,3);
    if(atoi(tmp) < 100){
      if(atoi(tmp) >= 10){
	       strncat(res,tmp+1,1);
	       strncat(res,tmp+2,1);
      }else{
	       strncat(res,tmp+2,1);
      }
    }
    if(atoi(tmp) >= 100){
      strcat(ret,tmp);
      strcat(ret,".");
    }else{
      strcat(ret,res);
      strcat(ret,".");
    }
    res[0] = '\0';
  }
  ret[strlen(ret)-1] = '\0';
  return ret;
}
