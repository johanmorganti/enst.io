/* ********************************************************************* *
 *
 *  label.c : creer un label dans une fenetre
 *  [Eric Lecolinet - 1994-96].
 *
 * ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>


int main(int argc, char *argv[])
{
  XtAppContext app;
  Widget toplevel, box, message;

  /* ************************************************ Initialisation *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv, NULL,
			     NULL, 0);

  /* ****************************************** Creation des Widgets *** */

				                  /* creation du manager */
  box = XmCreateRowColumn(toplevel, "box", NULL, 0);
  XtManageChild(box);
				              /* creation du widget fils */
  message = XmCreateLabel(box, "message", NULL, 0);
  XtManageChild(message);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);                  /* Realisation des widgets */
  XtAppMainLoop(app);	     /* boucle infinie de gestion des evenements */
}
