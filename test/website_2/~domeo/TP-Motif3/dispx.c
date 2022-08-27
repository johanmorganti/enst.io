/**************************************************************************

         dispx.c: affichage motif pour servx.c
	 Bertrand Dupouy && Eric Lecolinet
	 Juin 1996

***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <Xm/XmAll.h>
#include "disp.h"

/* ======================================================================== */

/* variable globale qui definit les defauts de l'appli graphique */
String fallbacks[] = {
    "*background: DodgerBlue3",
    "*foreground: White",
    "*annonces.width: 300",
    "*annonces.dialogTitle: Annonces",
    "*annonces.cancelLabelString: Exit",
    /* nombre de messages visibles dans la Liste */
    "*annonces*messages.visibleItemCount: 8",
    NULL
  };
  
static Widget dialog, list;

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
  XtManageChild(dialog);
  /* pour remettre a jour la liste au fur et a mesure
   * (sinon celle-ci ne sera affichee qu'a la fin de l'envoi du client)
   */
  XmUpdateDisplay(dialog);
}

/* ======================================================================== */
/* ======================================================================== */

static void ExitCB(Widget w, XtPointer clientd, XtPointer calld)
{
  exit(0);
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
   * - test: essayer sans specifier XmNresizePolicy = False
   */
  dialog = XmCreateTemplateDialog(toplevel, "annonces", args, XtNumber(args));

  /* Le bouton Cancel provoquera la sortie de l'application via ExitCB
   * Remarque: les TemplateDialog ne font apparaitre que les boutons qui
   * sont lies a un callback => les boutons OK et Help n'apparaitront pas.
   */
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
















