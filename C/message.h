#include <stdio.h>

#define SEPARATOR "-----------------------------------------------------------"
#define SEPARATOR_INFORMATION "----------------------INFORMATION--------------------------"
#define SEPARATOR_CIRCULANT "------------------------CIRCULANT--------------------------"
#define SEPARATOR_RECU "---------------------------RECU----------------------------"
#define SEPARATOR_ENVOI "---------------------------ENVOI---------------------------"
#define SEPARATOR_FIN "---------------------------FIN---------------------------"
#define MENU_SEPARATOR "***********************************************************"


/******************************

Méthode d'écriture des messages

******************************/

void print_menu();

void writeMessage(char *);

void writeMessage_n(char *);

void writeMessageR(char *);

void writeMessage_nR(char *);

void writeMessageF(char *);

void writeMessage_nF(char *);

void writeMessageE(char *);

void writeMessage_nE(char *);

void writeMessageC(char *);

void writeMessage_nC(char *);

void writeMessageI(char *);

void writeMessage_nI(char *);
