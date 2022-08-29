
/* ********************************************************************* *
 *
 *  formulaire4.c : 
 *  similiaire a formulaire3.c mais:
 *  - ouvre un Warning Dialog NON MODAL si le champ est vide
 *  - ferme ce dialog des que l'on tape un caractere sur un champ quelconque
 *
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <stdio.h>
#include <Xm/XmAll.h>


static char *ftag = XmFONTLIST_DEFAULT_TAG;          /* fonte par defaut */
static Widget warning = NULL;	                       /* Warning Dialog */


/* ********************************************************************* */
/* Cree un Warning Dialog NON modal */

static Widget CreateWarningDialog(Widget parent)
{
  Widget warning = XmCreateWarningDialog(parent, "warning", NULL, 0);
  XtVaSetValues(warning,
		XmNdialogTitle, XmStringCreateLtoR("Warning", ftag),
		XmNmessageString, XmStringCreateLtoR("Null Entry !", ftag),
		XmNdialogStyle, XmDIALOG_MODELESS,
		XmNnoResize, True,
		NULL);
  XtUnmanageChild(XmMessageBoxGetChild(warning, XmDIALOG_CANCEL_BUTTON));
  XtUnmanageChild(XmMessageBoxGetChild(warning, XmDIALOG_HELP_BUTTON));
  return warning;
}

/* ********************************************************************* */

static void EnterCB(Widget textf, XtPointer name, XtPointer call_data)
{
  String value = XmTextFieldGetString(textf);

  if (! value || ! value[0]) {
    if (! warning) warning = CreateWarningDialog(textf);
    XtManageChild(warning);
  }

  else {
    printf("> %s =  %s \n", (String)name, value);
				/* passer au champ suivant */
    XmProcessTraversal(textf, XmTRAVERSE_NEXT_TAB_GROUP);
  }
  if (value) XtFree(value);
}

/* ********************************************************************* */

static void CharTyped(Widget textf, XtPointer client_data, XtPointer call_data)
{
  if (warning) XtUnmanageChild(warning);
}

/* ********************************************************************* */

main(int argc, char *argv[])
{
  Widget toplevel, bigform, form[10], w[2];
  XtAppContext app;
  char *str, name[40];
  XmString ms;
  int n, l;

/* *********************************************** ressources ********** */

  Arg f_args[] = {
    {XmNfractionBase,     (XtArgVal) 3},
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

				/* recuperer le texte affiche sur label */
    XtVaGetValues(w[n-1], XmNlabelString, &ms);
    XmStringGetLtoR(ms, ftag, &str);
    if (ms) XmStringFree(ms);

    XtAddCallback(w[n], XmNactivateCallback, EnterCB, str);
    XtAddCallback(w[n], XmNmodifyVerifyCallback, CharTyped, NULL);
    n++;

    XtManageChildren(w, n);
  }

  XtManageChildren(form, l);
  XtManageChild(bigform);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


