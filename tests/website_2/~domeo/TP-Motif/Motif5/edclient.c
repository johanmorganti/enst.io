/**************************************************************************

         edclient.c: programme client qui envoie des donnees vers edserv.c
	 Bertrand Dupouy && Eric Lecolinet
	 Juin 1996

***************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netinet/in.h>
#include <Xm/XmAll.h>

static int sock;
static char *identite;
static Widget text;

/* ======================================================================== */
/* ======================================================================== */
/* Fonction recuperant le nom  de la personne lancant le serveur */

void SetIdentite(void)
{
  /* nom de la machine */
  struct utsname name;
  char *machine, *user;

  machine = (uname(&name) < 0) ? "???" : name.nodename;
  if (!machine) machine = "machine?";
  user = getlogin();
  if (!user) user = "user?";

  identite = (char*)malloc(strlen(machine)+strlen(user)+20);
  sprintf(identite, "Client: %s@%s", user, machine);
}


void Erreur()
{
  perror("Erreur d'ecriture vers le serveur");
  shutdown(sock, 2);
  close(sock);
  exit(4);
}

/* ======================================================================== */
/* ======================================================================== */

static void ExitCB(Widget w, XtPointer clientd, XtPointer calld) 
{
  shutdown(sock, 1);
  shutdown(sock, 2);
  close(sock);
  exit(0);
}

static void TapeTexteCB(Widget w, XtPointer clientd, XtPointer calld) 
{
  char send_str[1024];
  XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct*)calld;

  fprintf(stderr, "Entered: <%c>\n", cbs->text->ptr[0]);

#if TEST    
    sprintf(send_str, ">>> Debut de %s", identite);
    /* NB: strlen() + 1 pour envoyer le '\0' qui termine la String */
    if (write(sock, send_str, strlen(send_str)+1) < 0) Erreur();
#endif
    send_str[0] = cbs->text->ptr[0];
    send_str[1] = '\0';
    if (write(sock, send_str, 2) < 0) Erreur();

#if TEST    
    sprintf(send_str, "<<< Fin de %s", identite);
    if (write(sock, send_str, strlen(send_str)+1) < 0) Erreur();
#endif
}


static void RecoitTexte(XtPointer data, int *source, XtInputId *id)
{
  char receive_str[1024];

  if (read(sock, receive_str, sizeof(receive_str)) < 0)
    perror ("Erreur de lecture sur le socket de comm.") ;
  else {
    printf(">Recoit: <%s>\n", receive_str);
    XmTextInsert(text, XmTextGetLastPosition(text), receive_str);
  }
}

static String fallbacks[] = {
  "*background: LightSteelBlue",
  "*text.editMode: MULTI_LINE_EDIT",
  "*text.rows: 15",
  NULL
};

/* ======================================================================== */
/* ======================================================================== */

int main(int argc, char *argv[])
{
  struct sockaddr_in server;
  struct hostent *host;
  char receive_str[1024];
  XtAppContext app_context;
  XtInputId id;
  Widget toplevel, box, sortir;

  /* a mettre en premier car enleve les arguments X de argc, argv */
  toplevel = XtAppInitialize(&app_context, "XEd", NULL, 0, 
			     &argc, argv, fallbacks, NULL, 0);

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

  SetIdentite();

  /* ajouter source sur socket */
  id = XtAppAddInput(app_context, sock, (XtPointer)XtInputReadMask,
		     RecoitTexte, NULL);

  /* Creer Interface */
  box = XmCreateRowColumn(toplevel, "box", NULL, 0);
  XtManageChild(box);

  text = XmCreateScrolledText(box, "text", NULL, 0);
  XtManageChild(text);
  XtAddCallback(text, XmNmodifyVerifyCallback, TapeTexteCB, NULL);

  sortir = XmCreatePushButton(box, "sortir", NULL, 0);
  XtManageChild(sortir);
  XtAddCallback(sortir, XmNactivateCallback, ExitCB, NULL);

  /* realisation + lancement de la boucle d'evenements */
  XtRealizeWidget(toplevel);
  XtAppMainLoop(app_context);
}

