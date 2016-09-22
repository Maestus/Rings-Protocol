#include <assert.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <dirent.h>

/**************************

Import des headers pour TCP

**************************/

#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*****************************************

Import des headers pour (Respectivement) :

-Affichage

-Conversion String->Int

-Pour les idm (en milliseconds)

*****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/*************************

Import pour la mise en
forme des messages

*************************/

#include "message.h"

/*************************

Import du header utilisé
pout la gestion des
thread

*************************/

#include <pthread.h>


/**********************

Import pour le in_addr
Stockage d'addresse
IPV4

**********************/

#include <netinet/in.h>

/*************************************

MACROS définis pour connaitre la limite
de numéro de port

**************************************/

#define MAX_UDP_PORT_LISTEN 9999
#define MAX_PORT_MULTICAST 9999

/*************************************

Structure utilisée pour les entitées

*************************************/

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "annexe_functions.h"
#include "getteurs_message.h"
#include "gathered_native_messages.h"
#include "envoi_auto_native.h"
#include "insertions_native.h"
#include "formatage.h"

int test;

char *my_ip;

char *desc;

pthread_t tcp;

pthread_t udpreceiving;

pthread_t multi;

int eybgBool;

int waitingstart;

struct tm *start;

struct tm *end;

time_t now;

time_t now2;

char *downRing;


typedef struct{
  char **idtab;
  int number;
} idAlreadySee;

typedef struct{
  char *identity;
  int udpPort;
  int tcpPort;
  char *nextip;
  char *nextipdupl;
  int nextudpPortdupl;
  int nextudpPort;
  char *multicastPbReseau;
  char *multicastPbReseaudupl;
  int multicastport;
  int multicastportdupl;
  int doubleur;
} entity;


void * listeningudp(void * arg);

void udpsend(char *);

void udpsendmulti();

void *listeningtcp(void * arg);

void *listeningmulti(void *);

void init_entity(int argc, char * argv[]);

void *init_tcp(void *arg);

void *init_udp(void *arg);

void * choixmenu(void *arg);

void * menu(void *);

void * testingRing(void *);

void control(char *);

#endif
