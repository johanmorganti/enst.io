
/* ********************************************************************* *
 *
 *  xmutil.c
 *  qq fonctions "utiles" pour se faciliter la vie avec Motif
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <stdarg.h>
#include <Xm/XmAll.h>


/* **************************** Changer la couleur d'un Graphic Context */

Boolean ChangeColor(Display *display, GC gc, String color_name)
{
  XColor xcolor, unused;
  Colormap cmap = DefaultColormap(display, 0);
  XGCValues gcval;

  if (XAllocNamedColor(display, cmap, color_name, &xcolor, &unused)) {
    gcval.foreground = (u_long) xcolor.pixel;
    XChangeGC(display, gc, GCForeground, &gcval);
    return True;
  }
  return False;
}

/* ***************************** fonction de conversion de chaines *** */

XmString StringCreate(String text)
{
  return XmStringCreateLtoR(text, XmFONTLIST_DEFAULT_TAG);
}

String StringGet(XmString xmstring)
{
  char *text;
  return XmStringGetLtoR(xmstring, XmFONTLIST_DEFAULT_TAG, &text) ? text: NULL;
}

/* ******************************** callback pour ouvrir un Dialog *** */

void OpenDialog(Widget w, XtPointer dialog, XtPointer xxx)
{
  if (dialog) XtManageChild((Widget)dialog);
}

/* ********************************* callback pour fermer un Dialog *** */

/* NB: si dialog est null, prendre w (widget appelant) a la plce */

void CloseDialog(Widget w, XtPointer dialog, XtPointer xxx)
{
  if (dialog) XtUnmanageChild((Widget)dialog);
  else XtUnmanageChild(w);
}

/* ***************************** affectationsimplifiee des arglists *** */

Cardinal SetArgs(ArgList arglist, Cardinal maxlength, ...)
{
  String name;
  XtArgVal value;
  Cardinal n = 0;
  va_list vargs;

  va_start(vargs, maxlength);
  while (name = va_arg(vargs, String)) 
    if (n >= maxlength) {
      fprintf(stderr, "! SetArgs: too many arguments: cardinal >= %d \n", n);
      break;
    }
    else {
      value = va_arg(vargs, XtArgVal);
      XtSetArg(arglist[n], name, value); n++;
    }
  va_end(vargs);
  return n;
}

/* ********************************** creation simplifiee des Menus *** */

/* NB: fonction variadique (ANSI C) */  


Widget CreateMenu(Widget parent, String title, ...)
{
  Widget menu, cascade, button;
  Arg arg;
  String name;
  XtCallbackProc proc;
  XtPointer data;
  va_list vlist;

  menu = XmCreatePulldownMenu(parent, title, NULL, 0),
  /* ne PAS manager le menu ! */

  XtSetArg(arg, XmNsubMenuId, menu);
  cascade = XmCreateCascadeButton(parent, title, &arg, 1);
  XtManageChild(cascade);

  va_start(vlist, title);
  while (name = va_arg(vlist, String)) {
  
    button = XmCreatePushButton(menu, name, NULL, 0);
    XtManageChild(button);

    proc = va_arg(vlist, XtCallbackProc); /* fonction de callback */
    data = va_arg(vlist, XtPointer);      /* data associe */

    if (proc != NULL)
      XtAddCallback(button, XmNactivateCallback, proc, data);
  }
  va_end(vargs);
  return menu;
}

/* ****************************** creation simplifiee des Dialogues *** */

Widget CreateDialog(Widget parent, String name, 
		    Widget (*create_function)(),
		    XtCallbackProc ok_callback, XtPointer data)
{
  Widget h, dialog;
  Arg args[5];
  Cardinal i = SetArgs(args, XtNumber(args), 
		       XmNdialogTitle, StringCreate(name), 
		       XmNautoUnmanage, (XtArgVal)False,
		       NULL);
  dialog = (*create_function) (parent, name, args, i);

  XtAddCallback(dialog, XmNokCallback, ok_callback, data);
  XtAddCallback(dialog, XmNcancelCallback, CloseDialog, dialog);

				/* enlever le bouton Help */
  if (h = XtNameToWidget(dialog, "*Help")) XtUnmanageChild(h);
  return dialog;
}

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */

Widget XVa(Widget XmFunc(Widget, String, ArgList, Cardinal),
	   Widget parent, String widget_name, ...)
{
  String name;
  XtArgVal value;
  Cardinal index;
  va_list vargs;
  Widget w = NULL;

  ArgList arglist = NULL;
  Cardinal card = 0;
				/* compter les arguments */
  va_start(vargs, widget_name);
  while (name = va_arg(vargs, String)) {
    value = va_arg(vargs, XtArgVal);
    card++;
  }
  va_end(vargs);

  if (card > 0) {
				/* creer une liste d'arguments */
    if (! (arglist = (ArgList) XtCalloc(card, sizeof(Arg)) ) ) {
      fprintf(stderr, 
	      "! CreateArgs: XtCalloc failed: cardinal = %d \n", card);
      card = 0;
    }
    else {
				/* initialiser la liste d'arguments */
      va_start(vargs, widget_name);
      index = 0;
      while (name = va_arg(vargs, String)) {
	value = va_arg(vargs, XtArgVal);
	XtSetArg(arglist[index], name, value); index++;
      }
      va_end(vargs);
    }
  }
				/* creer le widget et le manager */

  w =  (XmFunc) (parent, widget_name, arglist, card);
  XtManageChild(w);
				/* detruire la liste d'arguments */
  if (arglist) XtFree((void*)arglist);

  return w;
}
