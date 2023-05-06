/* ********************************************************************* *
 *
 *  pushbutton.c : creer un label et un bouton dans une fenetre
 *  [Eric Lecolinet - 1994-96].
 *
 * ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>

/* ******************************************* Fonctions de Callback *** */
/* NB: w           = widget appelant le callback 
 *     client_data = donnee client specifiee par XtAddCallback()
 *     call_data   = pointeur sur une structure dependant du widget appelant
 */

void Sortir(Widget w, XtPointer client_data, XtPointer call_data)
{
  /* ... a completer ... */
}

/* ********************************************************************* */

int main(int argc, char *argv[])
{
  XtAppContext app;
  Widget toplevel, box, message, button;

  /* ************************************************ Initialisation *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv, NULL,
			     NULL, 0);

  /* ****************************************** Creation des Widgets *** */

				                  /* creation du manager */
  box = XmCreateRowColumn(toplevel, "box", NULL, 0);
  XtManageChild(box);
				                   /* creation du Label */
  message = XmCreateLabel(box, "message", NULL, 0);
  XtManageChild(message);
				                  /* creation du bouton */
  /* A completer : 
  button = XmCreate...(...,"button",...);
  ... etc ;
  */
				       /* lier les fonctions de callback */
  /* A completer : 
  XtAddCallback(button, ...);
  */

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);                  /* Realisation des widgets */
  XtAppMainLoop(app);	     /* boucle infinie de gestion des evenements */
}
