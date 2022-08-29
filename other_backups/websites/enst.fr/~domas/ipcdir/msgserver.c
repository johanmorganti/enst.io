/***********************************************************
  msgserver.c

              UTILISER la commande ipcs pour consulter les 
              outils System V fonctionnant sur la machine


  Fonctionnement : 1- Ce serveur attend des messages de type 1.
                      Ces messages contiennent dans leur partie texte
                      le pid de l'expediteur.
                   2- Le serveur repond en renvoyant un message dont 
                      le type est le pid de l'emetteur et le texte
                      son propre pid.

  *********************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define MSGKEY	50	/* cle d'entree dans la table d'IPC */

struct msg_struct {	/* structure d'un message */
  long mtype;
  char mtext[256];
} msg;

int msg_id;		/* identificateur de message */

void main (void)
{
  int	i_signal;	/* compteur de signaux */
  int	pid_client;	/* identificateur du processus client */
  int	*ptr_int;	/* servira a extraire la partie "texte" du message */
  void raz_msg (int);
 

  /* rendre la zone de messages sur reception de n'importe quel signal : */

  for (i_signal = 0; i_signal < NSIG; i_signal++)
    signal(i_signal, raz_msg);

  /* Pour donner des noms de facon elegante aux outils 
   system V, utiliser la fonction ftok (cf. le man) */
  
  msg_id = msgget(MSGKEY, 0777|IPC_CREAT);

  printf("Ici msgserver: zone messages prete, en attente de messages\n");
  
  /* Boucle d'attente des messages envoyes par les clients :
     on attend des messages de type 1.  */

  for(;;) 
    {
    msgrcv (msg_id, &msg, 256, 1L, 0);

  /* le texte du message recu est un entier (le pid du client) */
    ptr_int     = (int *)msg.mtext;
    pid_client  = *ptr_int;
    printf("Serveur: reception de messages envoyes par pid\t%d\n", pid_client);
    msg.mtype   = pid_client;
  
 /*le serveur envoie son propre pid comme "texte" */
    *ptr_int    = getpid();
    msgsnd (msg_id, &msg, sizeof(int), 0);
    }
  
}

/* Gestion (brutale) des signaux : quel que soit le signal recu,
   on detruit la file de messages */

void raz_msg( int signal )
{
  printf(" Le serveur execute raz_msg avec signal : %d\n", signal);
  msgctl(msg_id, IPC_RMID, 0);
  exit();
}










