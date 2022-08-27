/* ********************************************************************* *
 *
 *  modaldialog.c : Ouvrir un PromtDialog et recuperer le texte entre'
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

/* ===================================================================== */
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


/* Fonction standard pour creer un PushButton, le manager et lui ajouter 
 * une fonction de callback
 */
Widget CreatePushButton(Widget parent, String name, 
			XtCallbackProc callback, XtPointer data)
{
  Widget w = XmCreatePushButton(parent, name, NULL, 0);
  if (!w) return NULL;
  /* ajouter la fonction de callback si elle est definie */
  if (callback) XtAddCallback(w, XmNactivateCallback, callback, data);
  /* manager et retourner le widget cree */
  XtManageChild(w);
  return w;
}

/* ********************************************************************* */
/* ********************************************* Nouvelles Fonctions *** */

/* Fonction pour recuperer un composant d'un dialogue predefini
 * (NB: voir fonctions XmSelectionBoxGetChild, etc. pour child_type)
 */

Widget GetDialogChild(Widget d, unsigned char child_type)
{
  if (XtClass(d) == xmMessageBoxWidgetClass)
    return XmMessageBoxGetChild(d, child_type);

  else if (XtClass(d) == xmSelectionBoxWidgetClass)
    return XmSelectionBoxGetChild(d, child_type);

  else if (XtClass(d) == xmFileSelectionBoxWidgetClass)
    return XmFileSelectionBoxGetChild(d, child_type);

  else return NULL;
}

/* Fonction standard pour creer un Dialog et ajouter une fonction
 * de callback au bouton OK
 * Remarques:
 * - si autoUnmanage == True,  les boutons OK et Cancel ferment le Dialog
 * - si autoUnmanage == False, on rajoute un callback pour que le bouton
 *                             Cancel ferme le Dialog
 * - 1er argument == fonction de creation Motif (ie. XmCreateXXXDialog)
 */

Widget CreateAnyDialog(Widget (*create_function)(Widget,String,ArgList,Cardinal),
		       Widget parent, String name,
		       Boolean auto_unmanage, 
		       Boolean modal_dialog,
		       Boolean show_help_button,
		       XtCallbackProc callback, XtPointer data)
{
  Widget d, helpw;
  Arg args[5];
  XtSetArg(args[0], XmNautoUnmanage, auto_unmanage);
  XtSetArg(args[1], XmNdialogStyle,
	   modal_dialog? XmDIALOG_FULL_APPLICATION_MODAL: XmDIALOG_MODELESS);

  d = (*create_function)(parent, name, args, 2);
  if (!d) return NULL;

  /* ajouter la fonction de callback si elle est definie */
  if (callback) XtAddCallback(d, XmNokCallback, callback, data);

  /* rajouter un callback pour que le bouton Cancel ferme le Dialog
   * si autoUnmanage == False
   */
  if (!auto_unmanage)
    XtAddCallback(d, XmNcancelCallback, CloseDialogCB, NULL);

  /* enlever le bouton Help */
  if (!show_help_button && (helpw = GetDialogChild(d, XmDIALOG_HELP_BUTTON)))
    XtUnmanageChild(helpw);

  return d;			/* NB: ne PAS manager le dialog */
}

/* ===================================================================== */

Widget CreateQuestionDialog(Widget parent, String name,
			    Boolean auto_unmanage, Boolean modal_dialog,
			    Boolean show_help_button,
			    XtCallbackProc callback, XtPointer data)
{
  return CreateAnyDialog(XmCreateQuestionDialog, parent, name,
			 auto_unmanage, modal_dialog, show_help_button,
			 callback, data);
}


Widget CreatePromptDialog(Widget parent, String name,
			  Boolean auto_unmanage, Boolean modal_dialog,
			  Boolean show_help_button,
			  XtCallbackProc callback, XtPointer data)
{
  return CreateAnyDialog(XmCreatePromptDialog, parent, name,
			 auto_unmanage, modal_dialog, show_help_button,
			 callback, data);
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
  Widget toplevel, mainbox, message, prompt_dialog, exit_dialog;

  toplevel = XtVaAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv,
			       fallbacks,
 			       /* cette ressource permet de modifier */
			       /* la taille de la fenetre principale */
			       XmNallowShellResize, (XtArgVal) True, 
			       NULL);

  /* creer le manager principal */
  /* (NB: equivalent a :  XmCreateRowColumn + XtManageChild) */
  mainbox = XtVaCreateManagedWidget("mainbox", xmRowColumnWidgetClass,
				    toplevel, NULL);

  /* zone de message en haut de "box" */
  /* (NB: equivalent a :  XmCreateLabel + XtManageChild) */
  message = XtVaCreateManagedWidget("message", xmLabelWidgetClass,
				    mainbox, NULL);

  /* creer le PromptDialog et le bouton qui ouvre ce Dialogue
   * depuis la fenetre principale de l'application
   */
  prompt_dialog = CreatePromptDialog(mainbox, "prompt_dialog", 
				     False, False, False,
				     GetAndSetTextCB, message);
  CreatePushButton(mainbox, "open", OpenDialogCB, prompt_dialog);

  /* creer le Dialog de confirmation de sortie et le bouton qui ouvre 
   * ce Dialogue depuis la fenetre principale de l'application
   */
  exit_dialog = CreateQuestionDialog(mainbox, "exit_dialog",
				     True, True, True,
				     ExitCB, NULL);
  CreatePushButton(mainbox, "quit", OpenDialogCB, exit_dialog);

  /* Realisation et Lancement */
  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}
