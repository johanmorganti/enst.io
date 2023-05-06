/* ********************************************************************* *
 *
 *  dialog.c : Ouvrir une boite de dialogue
 *  [Eric Lecolinet - 1996/97].
 *
 * ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Xm/XmAll.h>	     /* XmAll.h => inclut tous les headers Motif */


/* ********************************************************************* */
/* ******************************************* Fonctions Utilitaires *** */

/* Fonction de callback standard pour sortir d'une application
 */
void ExitCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}


/* Fonction de callback pour ouvrir le Dialogue passe en argument
 */
void OpenDialogCB(Widget w, XtPointer dialog, XtPointer call_data)
{
  Widget d = (Widget)dialog;
  XtManageChild(d);
  XtPopup(XtParent(d), XtGrabNone); /* pas obligatoire mais preferable...*/
}


/* Fonction de callback pour fermer le Dialogue passe en argument
 * (ou a defaut, le widget appelant)
 */
void CloseDialogCB(Widget w, XtPointer dialog, XtPointer call_data)
{
  Widget d = (Widget)dialog;
  if (! d) d = w;		 /* ouvre le widget appelant par defaut */
  XtUnmanageChild(d);
  XtPopdown(XtParent(d));	 /* pas obligatoire mais preferable ... */
}


/* ********************************************************************* */
/* *********************************************************** Main **** */

int main(int argc, char *argv[])
{
  String fallbacks[] = {
    "*background: grey",
    "*foreground: blue",
    "*message.background: red",
    "*message.foreground: white",
    "*message.labelString: Hello \\nBrave\\nNew World ...!",
    "*fontList: -*-helvetica-bold-r-normal-*-12-*-*-*-*-*-*-*",
    "*mydialog.dialogTitle: Alerte, alerte ...!",
    "*mydialog.messageString: L'avion va décoller dans 5 minutes\\n\
veuillez attacher vos ceintures",
    "*mydialog*symbolPixmap: /usr/local/X11/include/bitmaps/woman",
    "*mydialog.autoUnmanage: True",
    NULL,			/* toujours terminer par NULL !!! */
  };
  XtAppContext app;
  Widget toplevel, mainbox, message, dialog, open, close, quit;

  toplevel = XtVaAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv,
			       fallbacks, NULL);
  /* manager principal "mainbox" */
  mainbox = XmCreateRowColumn(toplevel, "mainbox", NULL, 0);
  XtManageChild(mainbox);

  /* zone de message en haut de "mainbox" */
  message = XmCreateLabel(mainbox, "message",  NULL, 0);
  XtManageChild(message);

  /* creation A PRIORI de la Boite de Dialogues */
  /* (NB: ne pas manager le Dialogue puisqu'il est initialement ferme) */
  dialog = XmCreateWarningDialog(mainbox, "mydialog", NULL, 0);

  /* PushButtons contenus dans "mainbox" */
  /* "open" ouvre la boite de dialogue via la fonction de callback 
   * OpenDialogCB a laquelle on passe le widget 'dialog' en argument
   */
  open = XmCreatePushButton(mainbox, "open", NULL, 0);
  XtAddCallback(open, XmNactivateCallback, OpenDialogCB, dialog);
  XtManageChild(open);

  /* "close" ferme la boite de dialogue */
  close = XmCreatePushButton(mainbox, "close", NULL, 0);
  XtAddCallback(close, XmNactivateCallback, CloseDialogCB, dialog);
  XtManageChild(close);

  /* "quit" termine l'application */
  quit = XmCreatePushButton(mainbox, "quit", NULL, 0);
  XtAddCallback(quit, XmNactivateCallback, ExitCB, NULL);
  XtManageChild(quit);

  /* Realisation et Lancement */
  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}
