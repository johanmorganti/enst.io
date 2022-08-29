/* ********************************************************************* *
 *
 *  orientation.c : Orientation du Manager RowColumn
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

/* ********************************************************************* */
/* ******************************************* Fonctions de Callback *** */
/* Remarques:
 * - ces fonctions de callback modifient certaines ressources du widget "box"
 *   (qui est un RowColumn Manager) afin de changer sa disposition spatiale.
 * - ce widget est transmis a la fonction de callback via son 2eme argument
 *   (cet argument est specifie par XtAddCallback())
 * - cette technique evite d'avoir a utiliser des variables globales
 */

/* changer l'orientation du RowColumn "box" */

void SetVertical(Widget w, XtPointer box, XtPointer call_data)
{
  XtVaSetValues((Widget)box, XmNorientation, (XtArgVal)XmVERTICAL, NULL);
}

void SetHorizontal(Widget w, XtPointer box, XtPointer call_data)
{
  XtVaSetValues((Widget)box, XmNorientation, (XtArgVal)XmHORIZONTAL, NULL);
}

/* ===================================================================== */
/* mettre le RowColumn "box" en mode PACK_TIGHT (= lineaire) */

void SetLineaire(Widget w, XtPointer box, XtPointer call_data)
{
  XtVaSetValues((Widget)box, XmNpacking, (XtArgVal)XmPACK_TIGHT, NULL);
}

/* mettre le RowColumn "box" en mode PACK_COLUMN (= matriciel)
 * et imposer 2 colonnes
 */
void SetMatriciel(Widget w, XtPointer box, XtPointer call_data)
{
  XtVaSetValues((Widget)box, 
		XmNpacking,	(XtArgVal) XmPACK_COLUMN,
		XmNnumColumns,	(XtArgVal) 2,
		NULL);
}

/* ===================================================================== */

void Sortir(Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}

/* ********************************************************************* */
/* ******************************************* Fonctions Utilitaires *** */
/* Creer un PushButton et lui ajouter une fonction de callback */

Widget CreateButton(Widget parent, String name, 
		    XtCallbackProc callback, XtPointer data)
{
  Widget w = XmCreatePushButton(parent, name, NULL, 0);
  /* NB: "data" sera passe en 2eme argument de la fonction de callback */
  XtAddCallback(w, XmNactivateCallback, callback, data);
  return w;
}

/* ********************************************************************* */
/* *********************************************************** Main **** */

int main(int argc, char *argv[])
{
  XtAppContext app;
  Widget toplevel, box, w[10];

  /* *************************** Creation du Toplevel et du Manager *** */

  toplevel = XtVaAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv, NULL,
			       /* cette ressource permet de modifier */
			       /* la taille de la fenetre principale */
			       XmNallowShellResize, (XtArgVal) True, 
			       NULL);

  box = XmCreateRowColumn(toplevel, "box", NULL, 0);
  XtManageChild(box);

  /* ***************************************** Creation des Enfants *** */

  w[0] = XmCreateLabel(box, "message",	NULL, 0);
  /* Notes: 
   * - CreateButton = fonction utilitaire (definie ci-dessus) qui cree
   *   un PushButton et lui ajoute une fonction de callback.
   * - Le dernier argument ("box") sera automatiquement passe a la 
   *   fonction de callback correspondante.
   */
  w[1] = CreateButton(box, "horizontal",SetHorizontal,	box);
  w[2] = CreateButton(box, "vertical",	SetVertical,	box);
  w[3] = CreateButton(box, "lineaire",	SetLineaire,	box);
  w[4] = CreateButton(box, "matriciel",	SetMatriciel,	box);
  w[5] = CreateButton(box, "quit",	Sortir, NULL);

  /* manager tous les enfants d'un coup */
  XtManageChildren(w, 6);	/* (tableau de widgets, nbre de widgets) */

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


