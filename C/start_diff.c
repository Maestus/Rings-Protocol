/*****************************************
cc -pthread -Wall start_diff.c -o start_diff diff.c entity.c message.c formatage.c annexe_functions.c getteurs_message.c gathered_native_messages.c envoi_auto_native.c insertions_native.c
*****************************************/

#include  "diff.h"

/****************************************************

Variable global représentant l'entité de ce programme

****************************************************/

entity entite;
idAlreadySee idms;

int main(int argc, char *argv[]){
  /*************************
  Définition d'une l'entitée
  *************************/

  init_entity(argc,argv);
  idms.idtab = calloc(4096,sizeof(char));
  idms.number = 0;
  eybgBool = 0;

  /****************************
  Initialisation du serveur TCP
  ****************************/

  pthread_create(&tcp,NULL,init_tcp,NULL);

  /*****************
  Initialisation udp
  *****************/

  pthread_t udp;
  pthread_create(&udp,NULL,app_init_udp,NULL);

  /************
  Print menu
  ************/

  print_menu();
  pthread_t pthmenu,testring;
  pthread_create(&pthmenu,NULL,menuApp,NULL);
  pthread_create(&testring,NULL,testingRing,NULL);

  while(1){}

  return 1;

}
