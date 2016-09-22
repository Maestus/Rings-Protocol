#include "entity.h"
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include <stdio.h>
#endif


typedef struct{
  int n; // Le numéro du paquet
  int size; // La taille du paquet
  char *content; //le paquet;
}paquet;

void *menuApp(void *);
void *app_init_udp(void *);
void * listenudpapp(void *);
void envoiUdp(char *);
