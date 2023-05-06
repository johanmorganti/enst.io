
/* ********************************************************************* *
 *
 *  list2.c : utilisation des Listes
 *  [Eric Lecolinet - 1994].
 *
 *  !!! A UTILISER AVEC FICHIER DE RESSOURCES: XListDemo
 *
 * ********************************************************************* */


#include <Xm/Xm.h>
#include <Xm/List.h>

static void print_selection(Widget w, XtPointer client, XtPointer call)
{
  XmListCallbackStruct *lcs = (XmListCallbackStruct *) call;
  String reason, choice;

  switch(lcs->reason) {
    case XmCR_BROWSE_SELECT:     reason = "BROWSE_SELECT"; break;
    case XmCR_SINGLE_SELECT:     reason = "SINGLE_SELECT"; break;
    case XmCR_MULTIPLE_SELECT:   reason = "MULTIPLE_SELECT"; break;
    case XmCR_EXTENDED_SELECT:   reason = "EXTENDED_SELECT"; break;
    default: reason = "DEFAULT_ACTION"; break;

  }
  XmStringGetLtoR(lcs->item, XmFONTLIST_DEFAULT_TAG, &choice);

  printf("- Reason   = %s \n",   reason);
  printf("- Item     = %s \n",   choice);
  printf("- Position = %d \n\n", lcs->item_position);
  XtFree(choice);
}

/* ********************************************************************* */

main(int argc, char *argv[])
{
  Widget toplevel, list;
  XtAppContext app;

  /* ************************************** Creation du Toplevel ... *** */

  toplevel = XtAppInitialize(&app, "XListDemo", NULL, 0, &argc, argv,
			     NULL, NULL, 0);

  /* ***************************************** Creation de la Liste  *** */

  list = XmCreateScrolledList(toplevel, "liste", NULL, 0);
  XtManageChild(list);
  XtAddCallback(list, XmNdefaultActionCallback, print_selection, NULL);
  XtAddCallback(list, XmNbrowseSelectionCallback, print_selection, NULL);
  XtAddCallback(list, XmNsingleSelectionCallback, print_selection, NULL);
  XtAddCallback(list, XmNmultipleSelectionCallback, print_selection, NULL);
  XtAddCallback(list, XmNextendedSelectionCallback, print_selection, NULL);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


