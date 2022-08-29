/* ********************************************************************* *
 *
 *  promptdialog.c : Ouvrir un PromtDialog et recuperer le texte entre'
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
/* ******************************************* Fonctions Specifiques *** */

/* fonction de callback qui recupere le texte entre' dans le PromptDialog
 * Remarque: "call_data" est un pointeur sur une structure dependant du
 * PromptDialog qui appelle cette fonction.
 */

void GetAndSetTextCB(Widget dialog, XtPointer message, XtPointer cbs)
{
  /* ce champ pointe sur le texte entre (! Attention: XmString !) */
  XmString value = ((XmSelectionBoxCallbackStruct*)cbs)->value;
  String str;

  /* affiche la chaine entree sur le Label "message" */
  XtVaSetValues((Widget)message, XmNlabelString, value, NULL);

  /* affiche la chaine entree sur le xterm :
   * ! Attention: il faut convertir la XmString en String normale
   */
  XmStringGetLtoR(value, XmFONTLIST_DEFAULT_TAG, &str);
  printf("> Texte entree dans %s = %s \n", XtName(dialog), str);
  XtFree(str);
}

/* ********************************************************************* */
/* *********************************************************** Main **** */

int main(int argc, char *argv[])
{
  String fallbacks[] = {
    "*background: grey",
    "*foreground: blue",
    "*XmTextField.background: white",
    "*message.background: red",
    "*message.foreground: white",
    "*message.labelString: Hello \\nBrave\\nNew World ...!",
    "*fontList: -*-helvetica-bold-r-normal-*-12-*-*-*-*-*-*-*",
    "*exit_dialog.dialogTitle: Exit",
    "*exit_dialog.messageString: Voulez-vous quitter l'application ?",
    "*prompt_dialog.dialogTitle: Entrez votre nom",
    "*prompt_dialog.selectionLabelString: Entrez votre nom",
    NULL,			/* toujours terminer par NULL !!! */
  };
  XtAppContext app;
  Widget toplevel, mainbox, message, prompt_dialog, exit_dialog, open, quit;

  toplevel = XtVaAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv,
			       fallbacks, NULL);

  /* manager principal "mainbox" */
  mainbox = XmCreateRowColumn(toplevel, "mainbox", NULL, 0);
  XtManageChild(mainbox);

  /* zone de message en haut de "mainbox" */
  message = XmCreateLabel(mainbox, "message",  NULL, 0);
  XtManageChild(message);

  /* creer le PromptDialog et y associer la fonction de callback
   * qui recupere le texte quand on clique sur OK
   */
  prompt_dialog = XmCreatePromptDialog(mainbox, "prompt_dialog", NULL, 0);
  XtAddCallback(prompt_dialog, XmNokCallback, GetAndSetTextCB, message);

  /* creer le bouton qui ouvre le PromptDialog */
  open = XmCreatePushButton(mainbox, "open", NULL, 0);
  XtAddCallback(open, XmNactivateCallback, OpenDialogCB, prompt_dialog);
  XtManageChild(open);

  /* creer le Dialog de confirmation de sortie et y associer 
   * la fonction de callback qui sort de l'application qunad on clique OK
   */
  exit_dialog = XmCreateQuestionDialog(mainbox, "exit_dialog", NULL, 0);
  XtAddCallback(exit_dialog, XmNokCallback, ExitCB, NULL);

  /* creer le bouton qui ouvre le Dialog de confirmation */
  quit = XmCreatePushButton(mainbox, "quit", NULL, 0);
  XtAddCallback(quit, XmNactivateCallback, OpenDialogCB, exit_dialog);
  XtManageChild(quit);

  /* Realisation et Lancement */
  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}
