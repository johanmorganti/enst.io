
/* ********************************************************************* *
 *
 *  menu.c: Barres de Menus et Menus Deroulants
 *  [Eric Lecolinet - 1997].
 *
 * ********************************************************************* */

#include <X11/Xmu/Editres.h>

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>

			       /* ressources par defaut de l'application */

String fallbacks[] = {
  "*message.background: red",
  "*message.labelString: Couleur Test",
/*"*message.fontList:  -*-courier-bold-r-*-*-*-120-75-75-*-*-iso8859-1", */
/*"*message.fontList:  kanji24",*/
  "*message.fontList:  r24",
  NULL
};


/* ===================================================================== */
/* Fonctions de Callbacks : */


/* Change la couleur du Background du widget donne' en argument
 * a XtAddCallback en prenant pour nom de couleur le nom du bouton 
 * qui appelle cette fonction.
 *
 * (1e arg. wb = bouton appelant; 2e arg. wl = widget donne' en argument)
 */

void SetBackgroundCB(Widget wb, XtPointer pwl, XtPointer call_data)
{
  Widget wl = (Widget) pwl;
			     /* le nom de la couleur est celui du bouton */
  char *color_name = XtName(wb);
  XColor xcolor, unused;
  Colormap cmap = DefaultColormapOfScreen(XtScreen(wl));

  if (XAllocNamedColor(XtDisplay(wl), cmap, color_name, &xcolor, &unused))
    XtVaSetValues(wl, XmNbackground, xcolor.pixel, NULL);
}

/* ===================================================================== */

void SetForegroundCB(Widget wb, XtPointer pwl, XtPointer call_data)
{
  Widget wl = (Widget) pwl;
  XColor xcolor, unused;
  Colormap cmap = DefaultColormapOfScreen(XtScreen(wl));

  if (XAllocNamedColor(XtDisplay(wl), cmap, XtName(wb), &xcolor, &unused))
    XtVaSetValues(wl, XmNforeground, xcolor.pixel, NULL);
}

/* ===================================================================== */
/* Fonction de callback standard pour sortir d'une application */

void ExitCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}

/* ********************************************************************* */


main(int argc, char *argv[])
{
  XtAppContext app;
  int k, n;
  Widget toplevel, mainbox, menubar, label, menu, cascade, button;
  char *names[] = {"white", "black", "yellow", "red", "green", NULL};


  /* *** Initialisation et creation des Managers principaux ************ */

  toplevel = XtAppInitialize(&app, "XMenuDemo", NULL, 0, &argc, argv,
			     fallbacks, NULL, 0);
  XtAddEventHandler(toplevel, (EventMask) 0, True, _XEditResCheckMessages,
		    NULL);

  mainbox = XmCreateRowColumn(toplevel, "mainbox", NULL, 0);
  XtManageChild(mainbox);

  menubar = XmCreateMenuBar(mainbox, "menubar", NULL, 0);
  XtManageChild(menubar);

  label = XmCreateLabel(mainbox, "message", NULL, 0);
  XtManageChild(label);


  /* *** creation des menus deroulants et des boutons associes ********* */

  /* - creation de menu0 - - - - - - - - - - - - - - - - - - - - - - - - */

  /* creation du menu deroulant et du CascadeButton associe */
  menu = XmCreatePulldownMenu(menubar, "menu0", NULL, 0);
  cascade = XmCreateCascadeButton(menubar, "background", NULL, 0);
  XtManageChild(cascade);

  /* lier le menu au bouton (afin que celui-ci ouvre le menu) */
  XtVaSetValues(cascade, XmNsubMenuId, (XtArgVal)menu, NULL);

  /* creation des boutons contenus dans le menu */
  for (k = 0; names[k] != NULL; k++) {
    button = XmCreatePushButton(menu, names[k], NULL, 0);
    XtManageChild(button);
    XtAddCallback(button, XmNactivateCallback, SetBackgroundCB, label);
  }

  /* - creation de menu1 - - - - - - - - - - - - - - - - - - - - - - - - */

  /* creation du menu deroulant et du bouton associe */
  menu = XmCreatePulldownMenu(menubar, "menu1", NULL, 0);
  cascade = XmCreateCascadeButton(menubar, "foreground", NULL, 0);
  XtManageChild(cascade);
  XtVaSetValues(cascade, XmNsubMenuId, (XtArgVal)menu, NULL);

  /* creation des boutons du menu */
  for (k = 0; names[k] != NULL; k++) {
    button = XmCreatePushButton(menu, names[k], NULL, 0);
    XtManageChild(button);
    XtAddCallback(button, XmNactivateCallback, SetForegroundCB, label);
  }

  /* - creation de menu2 - - - - - - - - - - - - - - - - - - - - - - - - */

  menu = XmCreatePulldownMenu(menubar, "menu2", NULL, 0);
  cascade = XmCreateCascadeButton(menubar, "exit", NULL, 0);
  XtManageChild(cascade);
  XtVaSetValues(cascade, XmNsubMenuId, (XtArgVal)menu, NULL);

  button = XmCreatePushButton(menu, "quit", NULL, 0),
  XtAddCallback(button, XmNactivateCallback, ExitCB, NULL);
  XtManageChild(button);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


