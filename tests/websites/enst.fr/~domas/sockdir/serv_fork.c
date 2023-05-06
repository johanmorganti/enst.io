/***********************************************************
  serv_fork.c
             - UTILISER la commande netstat pour voir
               les connexions entrantes et sortantes
               sur la machine.
             - ATTENTION : le signal  SIGCHLD n'est 
               pas gere, il y aura donc plein
               de petits zombies a observer !
  *********************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*                         SERVEUR                               */

int Sock_Serv ,Sock_Com, Ret_Read,Taille ;
char Buf  [1024] ;
static char MESSAGE [] = "Reponse de pid                            \n";

void main (void)

{
  struct sockaddr_in Le_Serveur ;
  
  void Gerant_Comm (void);

  if ((Sock_Serv =socket (AF_INET,SOCK_STREAM,0)) < 0)
	{
	  perror ("Ouverture du socket en mode connecte\n") ;
	  exit (1) ;
	}
  /**********************************************************************
  construction de l'adresse du serveur, attention le numero
  de port est "en dur". 
  **********************************************************************/
  Le_Serveur.sin_family      = AF_INET ;
Le_Serveur.sin_addr.s_addr = INADDR_ANY ;
Le_Serveur.sin_port        = 7545 ;

sprintf (MESSAGE+15, " %d fils de %d",(int)getpid(), (int)getppid());
printf ( "Ici le serveur  %s\n",MESSAGE);

Taille=sizeof (Le_Serveur) ;

if (bind (Sock_Serv ,(struct sockaddr *)&Le_Serveur, Taille ) < 0)
  {
    perror ("Serveur : erreur sur bind ") ;
    exit (1) ;
  }

/* Verifications sur le serveur : */
if (getsockname (Sock_Serv ,(struct sockaddr *) &Le_Serveur,&Taille) < 0)
  {
    perror ("Serveur : erreur sur getsocketname ") ;
    exit (1) ;
  }
printf (" Numero du port : %d\n",ntohs (Le_Serveur.sin_port)) ;
printf (" **********************************\n"); 

/* Le serveur se met en attente sur le socket d ecoute */
listen (Sock_Serv ,5) ;

/* Ci-dessous le pere ferme le socket de communications
   et retourne attendre d'autres demandes de connexion
   */

while (1)
  /* cf. man -s 3n accept , attention EWOULBLOCK n'est pas gere ! */
  {
    if((Sock_Com=accept (Sock_Serv,(struct sockaddr *) 0,(int *) 0)) == -1)
      {
	perror (" Serveur : erreur sur Accept") ;
	continue ;
      }
    
    /* Creation d'un processus pour gerer la communication */
    if (fork () == 0){
      Gerant_Comm();
      exit (4);
    }
    close (Sock_Com) ;
  }
}

/**************** Fonction de gestion des communications ************/ 
void Gerant_Comm()
{
  close (Sock_Serv ) ; 
  if (write (Sock_Com,"Debut de comm.\n",15) < 0 )
    perror ("Erreur ecriture Message") ;
  do
    {
      memset(Buf, 0, sizeof (Buf)) ;
      if ((Ret_Read=read (Sock_Com,Buf,sizeof (Buf))) < 0)
	{ 
	  perror ("Erreur lecture Message") ;
	  printf (" *************************************\n");
	  continue;
	} 
      if (Ret_Read == 0 )
	{ 
	  printf (" Ici, pid %d : Fin de connexion \n", (int)getpid()) ;
	  printf (" *************************************\n");
	} 
      else 
	{
	  printf("Ici %d, fils du serveur %d, lit :\n",(int)getpid(),(int)getppid());
	  printf("%s\n", Buf );
	}
    }
  while (Ret_Read > 0) ;
  
  sprintf (MESSAGE+15, " %d fils de %d",(int)getpid(), (int)getppid());
  if (write (Sock_Com, MESSAGE, strlen(MESSAGE) + 1 ) < 0)
    {
      printf ("Ici pid %d : ", (int)getpid()) ;
      perror ("Erreur ecriture d'un message") ;
      exit (2) ;
    }
}




