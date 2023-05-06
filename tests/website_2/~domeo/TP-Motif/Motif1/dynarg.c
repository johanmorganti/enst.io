/* ********************************************************************* *
 *
 *  dynarg.c : Modification dynamique des ressources
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
/* NB: 1er arg  = widget appelant le callback 
 *     2eme arg = donnee client specifiee par XtAddCallback()
 *     3eme arg = pointeur sur une structure dependant du widget appelant
 */

/* à compléter :
void Echanger(.....)
{
 ......

}
*/

/* ===================================================================== */

void Sortir(Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}

/* ********************************************************************* */

int main(int argc, char *argv[])
{
  XtAppContext app;
  Widget toplevel, box, message, xchange, quit;

  /* ************************************************ Initialisation *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv, NULL,
			     NULL, 0);

  /* ****************************************** Creation des Widgets *** */

  box = XmCreateRowColumn(toplevel, "box", NULL, 0);
  XtManageChild(box);

  message = XmCreateLabel(box, "message", NULL, 0);
  /* Modification DYNAMIQUE (apres creation) des ressources du Label */
  XtVaSetValues(message, 
		XmNforeground, BlackPixelOfScreen(XtScreen(message)),
		XmNbackground, WhitePixelOfScreen(XtScreen(message)),
		NULL);
  XtManageChild(message);

  xchange = XmCreatePushButton(box, "xchange", NULL, 0);
  /* XtAddCallback(...); à compléter ... */
  XtManageChild(xchange);

  quit = XmCreatePushButton(box, "quit", NULL, 0);
  XtAddCallback(quit, XmNactivateCallback, Sortir, NULL);
  XtManageChild(quit);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}
