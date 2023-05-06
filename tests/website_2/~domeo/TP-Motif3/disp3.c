/**************************************************************************

         disp3.c: affichage motif pour programmes gc.c
	 Affiche une montre pendant les calculs
	 Bertrand Dupouy && Eric Lecolinet
	 Juin 1996

***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <Xm/XmAll.h>
#include <X11/cursorfont.h>
#include "disp.h"

/* ======================================================================== */
/* ======================================================================== */

/* variable globale qui definit les defauts de l'appli graphique */
String fallbacks[] = {
    "*background: LightSteelBlue",
    "*foreground: Black",
    "*groscal.width: 350",
    "*groscal.dialogTitle: Gros Calcul 3",
    "*groscal.okLabelString: GO",
    "*groscal.cancelLabelString: Exit",
    /* nombre de messages visibles dans la Liste */
    "*groscal*messages.visibleItemCount: 12",
    "*groscal*messages.background: white",
    NULL
  };
  
static Widget dialog, list;

/* ======================================================================== */
/* ======================================================================== */
/* Fonction qui change le curseur et affiche une montre a la place */

void BusyCursor(Widget w, Boolean busy)
{
  Display *display = XtDisplay(w);
  static cursor_defined = False;
  static Cursor busy_cursor;

  if (! cursor_defined) {
    cursor_defined = True;
    busy_cursor = XCreateFontCursor(display, XC_watch);
  }

  if (busy) XDefineCursor(display, XtWindow(w), busy_cursor);
  else XUndefineCursor(display, XtWindow(w));
  XFlush(display);
}

/* ======================================================================== */
/* ======================================================================== */

void AfficheMessage(char *message)
{
  if (message) {
    XmString xms = XmStringCreateLocalized(message);
    XmListAddItem(list, xms, 0);	/* 0 => ajoute a la fin */
    XmListSetBottomPos(list, 0);	/* rend dernier item visible */
    XmListSelectPos(list, 0, False);	/* selectionne dernier item */
    XmStringFree(xms);
  }

  /* pour faire reapparaitre le Dialogue s'il etait cache */
  XtManageChild(dialog);	/* !! */

  /* pour remettre a jour l'affichage */
  XmUpdateDisplay(dialog);	/* !! */
}

/* ======================================================================== */
/* ======================================================================== */

static void ExitCB(Widget w, XtPointer clientd, XtPointer calld)
{
  exit(0);
}

/* ======================================================================== */

static void GrosCalculCB(Widget w, XtPointer clientd, XtPointer calld)
{
  BusyCursor(dialog, True);	/* !! affiche une montre */
  gros_calcul(150, 150, 150);
  BusyCursor(dialog, False);	/* !! remet le curseur normal */
}

/* ======================================================================== */
/* ======================================================================== */

Widget CreerInterface(Widget toplevel)
{
  Arg args[] = {
    {XmNresizePolicy, XmRESIZE_NONE},
    {XmNautoUnmanage, False},
  };

  /* Creer la boite de Dialogue
   * Remarque: 
   * on interdit les changements de tailles de la boite de Dialogue
   * provoques PAR LE PROGRAMME en mettant XmNresizePolicy a RESIZE_NONE
   * NBS :
   * - ca n'empeche pas l'utilisateur de retailler la fenetre
   * - que se passe-t'il si XmNresizePolicy n'est pas specifiee ?
   */
  dialog = XmCreateTemplateDialog(toplevel, "groscal", args, XtNumber(args));

  /* Le bouton OK lance le gros calcul */
  XtAddCallback(dialog, XmNokCallback, GrosCalculCB, NULL);

  /* Le bouton Cancel provoque la sortie de l'application via ExitCB */
  XtAddCallback(dialog, XmNcancelCallback, ExitCB, NULL);

  /* ajouter une Liste avec ascenseur dans le Dialogue */
  list = XmCreateScrolledList(dialog, "messages", NULL, 0);

  /* manager les widgets (=> les faire apparaitre a l'ecran) */
  XtManageChild(list);
  XtManageChild(dialog);

  /* realiser physiquement les Widgets (= creer et afficher les Windows) */
  XtRealizeWidget(dialog);

  return list;
}

/* ======================================================================== */
/* ======================================================================== */
