
/* ********************************************************************* *
 *
 *  mainwindow.c: MainWindow et ScrolledText
 *  [Eric Lecolinet - 1997].
 *
 * ********************************************************************* */


#include <Xm/XmAll.h>


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
  Widget toplevel, mainbox, menubar, text, commande, label, menu, cascade, button;
  char *names[] = {"white", "black", "yellow", "red", "green", NULL};


  /* *** Initialisation et creation des Managers principaux ************ */

  toplevel = XtAppInitialize(&app, "XMenuDemo", NULL, 0, &argc, argv,
			     fallbacks, NULL, 0);
  mainbox = XmCreateMainWindow(toplevel, "mainbox", NULL, 0);
  XtManageChild(mainbox);

  menubar = XmCreateMenuBar(mainbox, "menubar", NULL, 0);
  XtManageChild(menubar);

  /* Notes:
   * * XmNeditMode = XmMULTI_LINE_EDIT signifie que le Text peut avoir
   *    plusieurs lignes (par defaut il ne peut en avoir qu'une !!!)
   *
   * * XmNeditable = True signifie qu'on peut entrer du texte
   *   (sinon on peut seulement afficher du texte mais pas le modifier)
   */
  text = XmCreateScrolledText(mainbox, "texte", NULL, 0);
  XtVaSetValues(text, 
		XmNeditMode, XmMULTI_LINE_EDIT, 
		XmNeditable, True,
		XmNrows, 10,
		XmNvalue, "bla bla bla\nbla bla bla\nbla bla bla\n",
		NULL);
  XtManageChild(text);

  commande = XmCreateTextField(mainbox, "commande", NULL, 0);
  XtManageChild(commande);

  label = XmCreateLabel(mainbox, "message", NULL, 0);
  XtManageChild(label);

/*
  !!! Attention:  XtParent(text) afin d'acceder a la ScrolledWindow
  !!!             contenant le widget Text

  XmMainWindowSetAreas(mainbox, 
                       menubar, commande, NULL, NULL, XtParent(text));
  ou bien : 
*/

  XtVaSetValues(mainbox,
		XmNmenuBar, menubar,
		XmNcommandWindow, commande,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNworkWindow, XtParent(text),   /* !!! */
		XmNmessageWindow, label,
		XmNshowSeparator, True,
		NULL);


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
    XtAddCallback(button, XmNactivateCallback, SetBackgroundCB, text);
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
    XtAddCallback(button, XmNactivateCallback, SetForegroundCB, text);
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


