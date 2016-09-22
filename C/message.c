/*******************

Inclusion du header

*******************/

#include "message.h"

/*******************

Import pour la
méthode d'affichage
printf

*******************/

#include <string.h>
#include <stdio.h>

/*******************************************************************

Fonction writeMessage maléable selon vos gouts en terme d'affichage.

*******************************************************************/



void print_menu(){
  printf("%s\n",MENU_SEPARATOR);
  printf("                           MENU                             \n");
  printf("%s\n",MENU_SEPARATOR);
  printf("Pour vous inserer dans un anneau : connect ip port \n");
  printf("1/ WHOS REQUEST\n");
  printf("2/ GBYE REQUEST\n");
  printf("3/ Informations\n");
  printf("%s\n",MENU_SEPARATOR);
}


void writeMessageE(char * c){
  printf("%s\n",SEPARATOR_ENVOI);
  printf("%s",c);
  printf("%s\n",SEPARATOR);
}

void writeMessage_nE(char * c){
  printf("%s\n",SEPARATOR_ENVOI);
  printf("%s\n",c);
  printf("%s\n",SEPARATOR);
}

void writeMessageF(char * c){
  printf("%s\n",SEPARATOR_FIN);
  printf("%s",c);
  printf("%s\n",SEPARATOR);
}

void writeMessage_nF(char * c){
  printf("%s\n",SEPARATOR_FIN);
  printf("%s\n",c);
  printf("%s\n",SEPARATOR);
}

void writeMessageC(char * c){
  printf("%s\n",SEPARATOR_CIRCULANT);
  printf("%s",c);
  printf("%s\n",SEPARATOR);
}

void writeMessage_nC(char * c){
  printf("%s\n",SEPARATOR_CIRCULANT);
  printf("%s\n",c);
  printf("%s\n",SEPARATOR);
}

void writeMessageI(char * c){
  printf( "%s\n",SEPARATOR_INFORMATION);
  printf("%s",c);
  printf("%s\n",SEPARATOR);
}

void writeMessage_nI(char * c){
  printf("%s\n",SEPARATOR_INFORMATION);
  printf("%s\n",c);
  printf("%s\n",SEPARATOR);
}

void writeMessageR(char * c){
  printf("%s\n",SEPARATOR_RECU);
  printf("%s",c);
  printf("%s\n",SEPARATOR);
}

void writeMessage_nR(char * c){
  printf("%s\n",SEPARATOR_RECU);
  printf("%s\n",c);
  printf("%s\n",SEPARATOR);
}
