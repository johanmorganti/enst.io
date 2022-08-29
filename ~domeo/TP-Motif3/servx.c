/**************************************************************************

         servx.c: serveur (gestion du reseau par boucle X)
	 Bertrand Dupouy && Eric Lecolinet
	 Juin 1996

***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/utsname.h>
#include <netinet/in.h>
#include <Xm/XmAll.h>
#include "disp.h"

/* ======================================================================== */
/* ======================================================================== */
/* Fonction recuperant le nom  de la personne lancant le serveur */

char *get_identite(void)
{
  /* nom de la machine */
  struct utsname name;
  char *machine = (uname(&name) < 0) ? "???" : name.nodename;

  /* nom du user */
  char *user = getlogin();

  char *ident = (char*)malloc(strlen(machine)+strlen(user)+20);
  sprintf(ident, "Serveur: %s@%s", user, machine);
  return ident;
}

/* ======================================================================== */
/* ======================================================================== */

static int ouvrir_socket(int port)
{  
  struct sockaddr_in server;
  int sock_ecou = socket(AF_INET,SOCK_STREAM,0);

  memset(&server, '\0', sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  if (bind(sock_ecou,(struct sockaddr*) &server,sizeof(server)) < 0) {  
    perror ("Erreur lors du bind\n");
    return -1;
  }

  listen(sock_ecou, 5);
  return sock_ecou;
}

/* ======================================================================== */
/* ======================================================================== */

static void lecture_socket(int sock, char *identite)
{
  char buffer[1024];
  int nb_lus;

  if (write(sock, identite, strlen(identite)+1) < 0)
    perror("Erreur ecriture Message\n");

  while (True) {

    /* sortie si erreur */
    if ((nb_lus = read(sock, buffer, sizeof(buffer))) < 0) { 
      perror("Erreur lecture Message\n");
      return;
    } 

    /* sortie normale (fin de connexion) */
    else if (nb_lus == 0) {
      char message[] = "That's all folks";
      fprintf(stderr, "- Ici serveur: Fin de connexion \n") ;
      if (write(sock, message, strlen(message)+1) < 0)
	perror("Serveur: Erreur ecriture d'un message");
      return;
    } 

    /* sinon affichage des caracteres recus */
    else {
      fprintf(stderr, ">>>Recu: %s\n", buffer);
      AfficheMessage(buffer);
    }
  }
}

/* ======================================================================== */
/* fonction de gestion du reseau  */
/* ======================================================================== */

static void gere_reseau(XtPointer data, int *source, XtInputId *id)
{
  char *identite = (char*)data;
  int sock_comm;
  int sock_ecou = *source;	/* ATT: *source et non source !!! */

  fprintf(stderr, "gere_reseau : sock_ecou = %d\n", sock_ecou);
  fprintf(stderr, "avant accept\n");

  sock_comm = accept(sock_ecou, (struct sockaddr *)0, (int *)0);
  if (sock_comm < 0) {
    perror("erreur sur accept\n");
  }

  lecture_socket(sock_comm, identite);

  shutdown(sock_comm, 2);
  close(sock_comm);

  fprintf(stderr, "- Ici serveur: fin de dialogue\n");
}

/* ======================================================================== */
/* ======================================================================== */

int main(int argc, char *argv[])
{
  int port;			/* port du serveur */
  int sock_ecou;
  char *identite, banniere[80];
  XtAppContext app_context;
  extern String fallbacks[];
  XtInputId id;

  /* a mettre en premier car enleve les arguments X de argc, argv */
  Widget toplevel = XtAppInitialize(&app_context, "XAnnonce", NULL, 0, 
				    &argc, argv, fallbacks, NULL, 0);

  /* noter que les arguments ont eventuellement ete modifies  */
  if (argc != 2) {
    printf("Usage: %s port\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[1]);
  sock_ecou = ouvrir_socket(port);

  if (sock_ecou < 0) {
    printf("Ouverture socket impossible sur port %d\n", port);
    exit(2);
  }

  identite = get_identite();
  CreerInterface(toplevel);

  /* ajouter source sur socket */
  id = XtAppAddInput(app_context, sock_ecou, (XtPointer)XtInputReadMask,
		     gere_reseau, identite);

  /* premier message affiche */
  sprintf(banniere, "On attend sur le port %d ...", port);
  AfficheMessage(banniere);

  /* lancement de la boucle d'evenements X */
  XtAppMainLoop(app_context);
}

/* ======================================================================== */
