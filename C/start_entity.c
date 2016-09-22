/********************|||Compilation et Execution|||*********************

cc -pthread -Wall -std=gnu99 start_entity.c -o start_entity entity.c message.c formatage.c annexe_functions.c getteurs_message.c gathered_native_messages.c envoi_auto_native.c insertions_native.c
cc -pthread -Wall start_entity.c -o start_entity  entity.c  message.c formatage.c annexe_functions.c getteurs_message.c gathered_native_messages.c envoi_auto_native.c insertions_native.c
./entity (identifiant) portUdp portTcp IP_MULTI_DIFFUSION PORT_MULTI_DIFFUSION

***********************************************************************/

#include  "entity.h"

/****************************************************

Variable global représentant l'entité de ce programme

****************************************************/

entity entite;
idAlreadySee idms;

int main(int argc, char*argv[]){


  /*************************
  Définition d'une l'entitée
  *************************/

  init_entity(argc,argv);
  idms.idtab = calloc(4096,sizeof(char));
  idms.number = 0;
  eybgBool = 0;

  /****************************
  Initialisatiofn du serveur TCP
  ****************************/

  pthread_create(&tcp,NULL,init_tcp,NULL);

  /*****************
  Initialisation udp
  *****************/

  pthread_t udp;
  pthread_create(&udp,NULL,init_udp,NULL);

  /************
  Print menuing
  ************/

  print_menu();
  pthread_t pthmenu,testring;
  pthread_create(&pthmenu,NULL,menu,NULL);
  pthread_create(&testring,NULL,testingRing,NULL);

  while(1){
  }

  return 1;
}
