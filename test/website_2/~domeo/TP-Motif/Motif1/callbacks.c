
/* ********************************************************************* *
 *
 *  callbacks.c: essai des callbacks d'un PushButton
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>

char *ftag = XmFONTLIST_DEFAULT_TAG;

/* ********************************************************************* */
/* NB: w           = widget appelant le callback 
 *     client_data = donnee client specifiee par XtAddCallback()
 *     call_data   = donnee dependant du widget appelant
 */

void ButtonCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
  char s[100];
  XmString xms;
				/* récupérer le label passé en argument */
  Widget label = (Widget) client_data;
			               /* récupérer les données d'appel */
  XmPushButtonCallbackStruct 
    *call = (XmPushButtonCallbackStruct*)call_data;

				/* afficher le nom du bouton appelant */
				/* et la "raison" du callback */
  switch(call->reason) {
  case XmCR_ARM:
    sprintf(s, "bouton %s armé ", XtName(w));
    break;
  case XmCR_DISARM:
    sprintf(s, "bouton %s desarmé ", XtName(w));
    break;
  case XmCR_ACTIVATE:
    sprintf(s, "bouton %s activé ", XtName(w));
    break;
  default:
    sprintf(s, "> cas anormal ");
  }
  printf("%s, raison = %d \n", s, call->reason);

                         /* changer le texte du label passe' en argument */
  xms = XmStringCreate(s, ftag);
  XtVaSetValues(label, XmNlabelString, xms, NULL);
				    /* ne pas oublier le XmStringFree ! */
  XmStringFree(xms);
}


/* ********************************************************************* */


int main(int argc, char *argv[])
{
  Widget toplevel, rowcol, label, button;
  XtAppContext app;
  XmString mstr;
  Arg args[10];
  int n;


  /* ************************************************ Initialisation *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv,
			     NULL, NULL, 0);

  /* ****************************************** Creation des Widgets *** */

				                  /* creation du manager */
  rowcol = XmCreateRowColumn(toplevel, "box", NULL, 0);
  XtManageChild(rowcol);
				          /* liste d'arguments du Label */
  n = 0;
  mstr = XmStringCreate("Chaîne définie dans le code", ftag);
  XtSetArg(args[n], XmNlabelString, mstr);
  n++;
  XtSetArg(args[n], XmNbackground, WhitePixelOfScreen(XtScreen(toplevel)));
  n++;
  XtSetArg(args[n], XmNforeground, BlackPixelOfScreen(XtScreen(toplevel)));
  n++;

  label = XmCreateLabel(rowcol, "message", args, n);
  XtManageChild(label);
  XmStringFree(mstr);	                  /* NB: recuperer place memoire */
	   

  button = XmCreatePushButton(rowcol, "bouton", NULL, 0);
  XtManageChild(button);

				/* callbacks */

  XtAddCallback(button, XmNactivateCallback, ButtonCallback, label);
  XtAddCallback(button, XmNarmCallback, ButtonCallback, label);
  XtAddCallback(button, XmNdisarmCallback, ButtonCallback, label);


  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


