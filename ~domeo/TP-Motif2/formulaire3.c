
/* ********************************************************************* *
 *
 *  formulaire3.c : 
 *  similiaire a formulaire2.c mais avec gestion du focus clavier :
 *  - passe au champ suivant quand on fait RETURN si on a tape qq chose
 *
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <stdio.h>
#include <Xm/XmAll.h>


/* ********************************************************************* */

static void EnterCB(Widget textf, XtPointer client_data, XtPointer call_data)
{
  String value = XmTextFieldGetString(textf);

  if (value && value[0]) {
    printf("> %s =  %s \n", XtName(textf), value);
    /* passer au champ suivant */
    XmProcessTraversal(textf, XmTRAVERSE_NEXT_TAB_GROUP);
  }
  if (value) XtFree(value);
}

/* ********************************************************************* */

main(int argc, char *argv[])
{
  Widget toplevel, bigform, form[10], w[2];
  XtAppContext app;
  char name[40];
  int n, l;

/* *********************************************** ressources ********** */

  /* essayer fractionBase = 3, 4 */
  Arg f_args[] = {
    {XmNfractionBase,     (XtArgVal) 2},
    {XmNleftAttachment,   XmATTACH_FORM},
    {XmNrightAttachment,  XmATTACH_FORM},
  };
  Arg l_args[] = {
    {XmNtopAttachment,    XmATTACH_FORM},
    {XmNbottomAttachment, XmATTACH_FORM},
    {XmNleftAttachment,   XmATTACH_FORM},
    {XmNrightAttachment,  XmATTACH_POSITION},
    {XmNrightPosition,    (XtArgVal) 1},
  };
  Arg t_args[] = {
    {XmNtopAttachment,    XmATTACH_FORM},
    {XmNbottomAttachment, XmATTACH_FORM},
    {XmNrightAttachment,  XmATTACH_FORM},
    {XmNleftAttachment,   XmATTACH_POSITION},
    {XmNleftPosition,     (XtArgVal) 1},
  };


  /* ****************************************** Toplevel et Manager *** */

  toplevel = XtAppInitialize(&app, "XFormulaire", NULL, 0, &argc, argv,
			     NULL, NULL, 0);

  bigform = XmCreateForm(toplevel, "bigform", NULL, 0);

  /* ********************************************** Fils du Manager *** */

  for (l = 0; l < 10; l++) {

    sprintf(name, "form%d", l);
    form[l] = XmCreateForm(bigform, name, f_args, XtNumber(f_args));
    if (l > 0)
      XtVaSetValues(form[l], 
		    XmNtopAttachment, XmATTACH_WIDGET,
		    XmNtopWidget,     form[l-1],
		    NULL);
    n = 0;

    sprintf(name, "label%d", l);
    w[n] = XmCreateLabel(form[l], name, l_args, XtNumber(l_args));
    n++;

    sprintf(name, "text%d", l);
    w[n] = XmCreateTextField(form[l], name, t_args, XtNumber(t_args));
    XtAddCallback(w[n], XmNactivateCallback, EnterCB, NULL);
    n++;

    XtManageChildren(w, n);
  }

  XtManageChildren(form, l);
  XtManageChild(bigform);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


