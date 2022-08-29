/**************************************************************************

         client.c: programme client qui envoie des donnees vers servx.c
	 Bertrand Dupouy && Eric Lecolinet
	 Juin 1996

***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netinet/in.h>

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
  sprintf(ident, "Client: %s@%s", user, machine);
  return ident;
}

/* ======================================================================== */
/* ======================================================================== */

int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in server;
  struct hostent *host;
  char *identite, receive_str[1024], send_str[1024];

  if (argc  != 3) {
    printf("Usage: %s hote port\n", argv[0]);
    exit(1);
  }

  /* creation du socket  */
  if ((sock = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
    perror("Erreur d'ouverture du socket") ;
    exit(1) ;
  }

  /* creation de l'adresse du serveur */
  server.sin_family = AF_INET;

  if (! (host = gethostbyname(argv[1]))) {
    fprintf(stderr, "%s : Hote inconnu\n", argv[1]);
    exit(2);
  }

  memcpy((char *) &server.sin_addr, (char *) host->h_addr, host->h_length);
  server.sin_port = htons(atoi(argv[2]));
  
  /* connexion vers le serveur */
  if (connect(sock,( struct sockaddr *) &server, sizeof(server)) < 0) {
    perror("Erreur de connexion sur le socket serveur "); 
    exit(3);
  }

  /* attendre message de debut de communication depuis serveur */
  if (read(sock, receive_str, sizeof(receive_str)) < 0)
    perror("Erreur de lecture sur le socket de comm.") ;
  else printf(">>>%s\n", receive_str);

  identite = get_identite();
  sprintf(send_str, ">>> Debut de %s", identite);
  /* NB: strlen() + 1 pour envoyer le '\0' qui termine la String */
  if (write(sock, send_str, strlen(send_str)+1) < 0) goto ERREUR;

  /* lire les donnees au clavier */
  printf("Tapez votre message. Terminez par ^D ou . en debut de ligne\n");

  while (gets(send_str)) {
    if (send_str[0] == '.') break;
    if (write(sock, send_str, strlen(send_str)+1) < 0) goto ERREUR;
  }

  sprintf(send_str, "<<< Fin de %s", identite);
  if (write(sock, send_str, strlen(send_str)+1) < 0) goto ERREUR;

  shutdown(sock, 1);
  
  /* lire la reponse du serveur */
  if (read(sock, receive_str, sizeof(receive_str)) < 0)
    perror ("Erreur de lecture sur le socket de comm.") ;
  else printf(">>>%s\n", receive_str) ;

  shutdown(sock, 2);
  close(sock);
  exit(0);

 ERREUR:
  perror("Erreur d'ecriture vers le serveur");
  shutdown(sock, 2);
  close(sock);
  exit(4);
}

