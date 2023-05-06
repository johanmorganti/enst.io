
/* ********************************************************************* *
 *
 *  menucascade.c: Barres de Menus et Menus Cascades
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>


			       /* ressources par defaut de l'application */

String fallbacks[] = {
  "*message.background: red",
  "*message.labelString: Couleur Test",
/*"*message.fontList:  -*-courier-bold-r-*-*-*-120-75-75-*-*-iso8859-1", */
/*"*message.fontList:  kanji24",*/
  "*message.fontList:  r24",
  "*fonts*sensitive: False",	/* CascadeButton "fonts" insensitif */
  NULL
};

/* ===================================================================== */
/* Fonction qui cree un PushButton standard */

Widget CreateButton(Widget parent, String name, 
		    XtCallbackProc callback, XtPointer data)
{
  Widget b = XmCreatePushButton(parent, name, NULL, 0);
  
  if (! b)  fprintf(stderr, "! PushButton '%s' couldn't be created\n", name);
  else 
    if (callback)  XtAddCallback(b, XmNactivateCallback, callback, data);

  return b;
}

/* ===================================================================== */
/* Fonction qui cree un CascadeButton et un PulldownMenu associe */
/* (retourne le PulldowmMenu widget) */

Widget CreateCascadeMenu(Widget parent, String name)
{
  Widget menu, cascade;
  char menu_name[50];
                                 /* XmNtearOffModel => menus detachables */
  Arg			                 /* XmNsubMenuId : menu associe' */ 
    menu_args[] = {{XmNtearOffModel, XmTEAR_OFF_ENABLED}},
    cascade_args[] = {{XmNsubMenuId, (XtArgVal)0}};

  sprintf(menu_name, "%s_menu", name);
  menu = XmCreatePulldownMenu(parent, menu_name, 
			      menu_args, XtNumber(menu_args));

  cascade_args[0].value = (XtArgVal) menu;
  cascade = XmCreateCascadeButton(parent, name, 
				  cascade_args, XtNumber(cascade_args));

  if (menu && cascade) XtManageChild(cascade);
  else fprintf(stderr, "! Menu '%s' couldn't be created\n", name);

  return menu;
}

/* ===================================================================== */
/* Fonctions de Callbacks : */

void SetMatriciel(Widget w, XtPointer rowcol, XtPointer call_data)
{
  Arg args[5];
  XtSetArg(args[0], XmNpacking, XmPACK_COLUMN);
  XtSetArg(args[1], XmNnumColumns, 2);
  XtSetValues((Widget)rowcol, args, 2);
}

/* ===================================================================== */

void SetLineaire(Widget w, XtPointer rowcol, XtPointer call_data)
{
  Arg args[5];
  XtSetArg(args[0], XmNpacking, XmPACK_TIGHT);
  XtSetValues((Widget)rowcol, args, 1);
}

/* ===================================================================== */

void SetHorizontal(Widget w, XtPointer rowcol, XtPointer call_data)
{
  XtVaSetValues((Widget)rowcol,	XmNorientation, XmHORIZONTAL, NULL);
}

/* ===================================================================== */

void SetVertical(Widget w, XtPointer rowcol, XtPointer call_data)
{
  XtVaSetValues((Widget)rowcol,	XmNorientation, XmVERTICAL, NULL);
}

/* ===================================================================== */

/* Change la couleur du Background du widget donne' en argument
 * a XtAddCallback en prenant pour nom de couleur le nom du bouton 
 * qui appelle cette fonction.
 *
 * (1e arg. wb = bouton appelant; 2e arg. wl = widget donne' en argument)
 */

void SetBackgroundColor(Widget wb, XtPointer pwl, XtPointer call_data)
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

void SetForegroundColor(Widget wb, XtPointer pwl, XtPointer call_data)
{
  Widget wl = (Widget) pwl;
  XColor xcolor, unused;
  Colormap cmap = DefaultColormapOfScreen(XtScreen(wl));

  if (XAllocNamedColor(XtDisplay(wl), cmap, XtName(wb), &xcolor, &unused))
    XtVaSetValues(wl, XmNforeground, xcolor.pixel, NULL);
}

/* ===================================================================== */

void BlancSurNoir(Widget w, XtPointer label, XtPointer call_data)
{
  Arg args[5];
  int n = 0;

  XtSetArg(args[n], XmNforeground, WhitePixelOfScreen(XtScreen(w)));
  n++;
  XtSetArg(args[n], XmNbackground, BlackPixelOfScreen(XtScreen(w)));
  n++;
  XtSetValues((Widget)label, args, n);
}

/* ===================================================================== */

void NoirSurBlanc(Widget w, XtPointer label, XtPointer call_data)
{
  Arg args[5];
  int n = 0;

  XtSetArg(args[n], XmNforeground, BlackPixelOfScreen(XtScreen(w)));
  n++;
  XtSetArg(args[n], XmNbackground, WhitePixelOfScreen(XtScreen(w)));
  n++;
  XtSetValues((Widget)label, args, n);
}

/* ===================================================================== */

void Sortir(Widget w, XtPointer client_data, XtPointer call_data)
{
  printf("attention, je sors ....... \n");
  exit(0);
}


/* ********************************************************************* */
/* ********************************************************************* */


main(int argc, char *argv[])
{
  Widget 
    toplevel, mainbox, menubar, rowcol, label, w[10],
    cascade[10], menu[5], submenu[5];
  XtAppContext app;
  XmString mstr;
  char *ftag = XmFONTLIST_DEFAULT_TAG;
  Arg args[10];
  int n, k;


  /* ************************************************ Initialisation *** */

  XtSetArg(args[0], XmNallowShellResize, True);

  toplevel = XtAppInitialize(&app, "XMenuDemo", NULL, 0, &argc, argv,
			     fallbacks, args, 1);

  /* *************************************** Creation des Managers *** */

  /* NB: les Managers sont eux-meme manages APRES leurs fils */

  mainbox = XmCreateRowColumn(toplevel, "mainbox", NULL, 0);
  menubar = XmCreateMenuBar(mainbox, "menubar", NULL, 0);
  rowcol  = XmCreateRowColumn(mainbox, "box", NULL, 0);


  /* *********************************** creation des CascadeButtons *** */
  /*                                     et des PulldownMenus associes   */

  menu[0] = CreateCascadeMenu(menubar, "geometrie");
  menu[1] = CreateCascadeMenu(menubar, "couleurs");
  menu[2] = CreateCascadeMenu(menubar, "divers");

  /* CascadeButtons manages par la fonction CreateCascadeMenu() */
  /* les PulldownMenus ne sont pas manages */

  XtManageChild(menubar);


  /* *********************************** creation des widgets contenus *** */
  /*                                     dans le RowColumn "box"           */

				            /* creation des widgets fils */
  k = 0;

  w[k++] = label = XmCreateLabel(rowcol, "message", NULL, 0);

  w[k++] = CreateButton(rowcol, "horizontal",     SetHorizontal, rowcol);
  w[k++] = CreateButton(rowcol, "vertical",	  SetVertical,   rowcol);
  w[k++] = CreateButton(rowcol, "lineaire",       SetLineaire,   rowcol);
  w[k++] = CreateButton(rowcol, "matriciel",      SetMatriciel,  rowcol);
  w[k++] = CreateButton(rowcol, "blanc_sur_noir", BlancSurNoir,  label);
  w[k++] = CreateButton(rowcol, "noir_sur_blanc", NoirSurBlanc,  label);
  w[k++] = CreateButton(rowcol, "quit",           Sortir,        NULL);

				                  /* manager les widgets */

  XtManageChildren(w, k);	/* (tableau de widgets, nbre de widgets) */
  XtManageChild(rowcol);	  /* manager le conteneur APRES ses fils */


  /* *********************************** creation des widgets contenus *** */
  /*                                     dans les PulldowMenus            */

				/* Creer les fils du menu "geometry_menu" */
  k = 0;

  w[k++] = XmCreateLabel(menu[0], "Direction:",    NULL,          0);
  w[k++] = CreateButton(menu[0],  "horizontal",    SetHorizontal, rowcol);
  w[k++] = CreateButton(menu[0],  "vertical",      SetVertical,   rowcol);
  w[k++] = XmCreateSeparator(menu[0], "",          NULL,          0);
  w[k++] = XmCreateLabel(menu[0], "Organisation:", NULL,          0);
  w[k++] = CreateButton(menu[0],  "lineaire",      SetLineaire,   rowcol);
  w[k++] = CreateButton(menu[0],  "matriciel",     SetMatriciel,  rowcol);

  XtManageChildren(w, k);

  /* - - - - - - - - - - - - sous-menus cascades du menu "couleurs_menu" */

  /* NB: les sous-menus et les cascadeButtons qui les appellent sont */
  /* tous deux fils du 2eme pulldownmenu */

  submenu[0] = CreateCascadeMenu(menu[1], "background");
  submenu[1] = CreateCascadeMenu(menu[1], "foreground");
  submenu[2] = CreateCascadeMenu(menu[1], "fonts");


  /* - - - - - - - - - - - - - - - - - - - - - - - fils des sous-menus */

  {
    char *names[] = {"", "", "white", "black", "yellow", "red", "green", NULL};

    k = 0;
    w[k++] = XmCreateLabel(submenu[0], "Background", NULL, 0);
    w[k++] = XmCreateSeparator(submenu[0], "", NULL, 0);

				/* creation iterative des boutons */
    for ( ; names[k] != NULL; k++)
      w[k] = CreateButton(submenu[0], names[k], SetBackgroundColor, label);

    XtManageChildren(w, k);

    k = 0;
    w[k++] = XmCreateLabel(submenu[1], "Foreground", NULL, 0);
    w[k++] = XmCreateSeparator(submenu[1], "", NULL, 0);

				/* creation iterative des boutons */
    for ( ; names[k] != NULL; k++)
      w[k] = CreateButton(submenu[1], names[k], SetForegroundColor, label);

    XtManageChildren(w, k);
  }

  /* - - - - - - - - - - - - - - - - - - - - - - fils du menu "divers" */

  k = 0;
  w[k++] = CreateButton(menu[2], "quit", Sortir, NULL);
  XtManageChildren(w, k);


  /* ************************************** Realisation et Lancement *** */

  XtManageChild(mainbox);
  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


