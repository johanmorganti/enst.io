
/* ********************************************************************* *
 *
 *  list1.c : utilisation des Listes
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <Xm/Xm.h>
#include <Xm/List.h>

static char *ouvrables[] = {
  "lundi", "mardi", "mercredi", "jeudi", "vendredi" };


/* ********************************************************************* */

main(int argc, char *argv[])
{
  Widget toplevel, list;
  XtAppContext app;
  Arg args[10];
  int i;

  /* ************************************** Creation du Toplevel ... *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv,
			     NULL, NULL, 0);

  /* ***************************************** Creation de la Liste  *** */

  {
    int k, n = XtNumber(ouvrables);
    XmStringTable tabstring = (XmStringTable) XtMalloc(n * sizeof(XmString));

    for (k = 0; k < n; k++)
      tabstring[k] = XmStringCreateLocalized(ouvrables[k]);

    i = 0;
    XtSetArg(args[i], XmNitems, tabstring); i++;
    XtSetArg(args[i], XmNitemCount, n); i++;
    XtSetArg(args[i], XmNvisibleItemCount, 4); i++;

    list = XmCreateScrolledList(toplevel, "liste", args, i);
    XtManageChild(list);

    for (k = 0; k < n; k++) XmStringFree(tabstring[k]);
    XtFree((char*)tabstring);
  }

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


