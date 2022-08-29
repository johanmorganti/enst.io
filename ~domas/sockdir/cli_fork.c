/***********************************************************
  cli_fork.c
              -Ce client attend des entrees sur stdin et les
               envoie au serveur auquel il est connecte.
              - UTILISER la commande netstat pour voir
               les connexions entrantes et sortantes
               sur la machine.
  *********************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>


void main (int argc, char *argv[])

{
int		Sock_Des , pid;
struct sockaddr_in Le_Serveur ;
struct hostent 	*Ptr_H ;
static char MESSAGE [] = "Message envoye par client            \n" ;
char Tab [1024] ;

if (argc  != 3)
	{
	printf (" Utilisation : %s hote port !!!!!!\n",argv[0]);
	exit (1);
	}

pid =getpid();
sprintf (MESSAGE+27, "%d", pid);

/* creation du socket du client */

if ((Sock_Des=socket (AF_INET,SOCK_STREAM,0)) < 0)
	{
	perror ("Erreur d'ouverture du socket") ;
	exit (1) ;
	}

/* Construction de l'adresse du serveur qui comprend plusieurs champs :
       - adresse IP de la machine ou se trouve le serveur 
         (recuperee par gethostbyname)
       - numero de port du serveur 
         (passe sur la ligne de commande)
 */
Le_Serveur.sin_family=AF_INET ;
if ((Ptr_H=gethostbyname (argv[1])) == (struct hostent *) 0)
	{
	fprintf (stderr,"%s : Hote inconnu\n",argv[1] ) ;
	exit (2);
	}
memcpy ((char *) &Le_Serveur.sin_addr,(char *) Ptr_H->h_addr,Ptr_H->h_length) ;
Le_Serveur.sin_port=htons (atoi (argv[2])) ;

/* connexion vers le serveur */
if (connect (Sock_Des,(struct sockaddr *) &Le_Serveur,sizeof (Le_Serveur)) < 0)
	{
	perror (" Erreur de connexion sur le socket serveur "); 
	exit () ;
	}

/* attendre message de debut de comm. depuis serveur */
if (read (Sock_Des,Tab,sizeof (Tab)) < 0)
	perror ("Erreur de lecture sur le socket de comm.") ;
printf ("Ici client pid %d, je lis :\n %s\n",pid,Tab) ;

if (write (Sock_Des,MESSAGE,strlen(MESSAGE) ) < 0)
	perror ("Erreur d'ecriture vers le serveur");
	
/* Entrer les messages a transmettre via le clavier (stdin) 
   la fin des entrees est indiquee par la chaine fin
*/
while ( fgets(Tab,sizeof (Tab), stdin) )
	{
	if (!strncmp( Tab,"fin",3)) break;      
	if (write (Sock_Des,Tab,strlen (Tab)) < 0)
		perror ("Erreur d'ecriture vers le serveur");
	}  

shutdown(Sock_Des,1);

/* lire la reponse du serveur */
if (read (Sock_Des,Tab,sizeof (Tab)) < 0)
	perror ("Erreur de lecture sur le socket de comm.") ;
printf ("Ici client pid %d, je lis :\n %s\n",pid,Tab) ;

close (Sock_Des) ;
exit (0) ;
}

