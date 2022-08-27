/***********************************************************
  msgclient.c

              UTILISER la commande ipcs pour consulter les 
              outils System V fonctionnant sur la machine

  *********************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>

#define MSGKEY	50		/* cle d'entree dans la table d'IPC */

struct msg_struct {		/* structure d' un message */
  long mtype;
  char mtext[256];
};

void main (void)
{  
  struct msg_struct	msg;	 /* message courant */
  int 			msg_id;	 /* identificateur du message courant */
  int			pid;	 /* identificateur du processus */
  int			*ptr_int;/* servira a mettre un entier dans
                                    le texte du message */  
  msg_id = msgget(MSGKEY, 0777);
  
  pid = getpid();
  printf ("msgclient a pour pid : %d\n", pid);

/****************************************************************
 Ce qui suit est une horreur pour mettre :
     le pid dans le champ texte, mais sous forme d'un entier 
****************************************************************/ 

  ptr_int   = (int *)msg.mtext;
  *ptr_int  = pid;      /* voila, c est ca */

/* On envoie un message de type 1 vers le serveur */

  msg.mtype = 1;
  msgsnd (msg_id, &msg, sizeof(int), 0);
  
/* On attend un message dont le type est notre pid */
  
  sleep (5);
  msgrcv (msg_id, &msg, 256, (long)pid, 0); 

/* Apres reception, on affiche ce qui a ete recu */

  printf("(Pid %d),reception depuis serveur : %d \n", (int)getpid(), *ptr_int);
  printf("(Pid %d), type de message lu : %ld \n",  (int)getpid(), msg.mtype);
}







