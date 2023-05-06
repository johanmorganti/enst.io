
/* ********************************************************************* *
 *
 *  formulaire2.c :
 *  Utilisation de Form Manager imbriques pour representer un "formulaire".
 *
 *  REMARQUE : methode valide quelle soit la taille des widgets fils.
 *
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>


/* ********************************************************************* */

main(int argc, char *argv[])
{
  Widget toplevel, bigform, form[10], w[2];
  XtAppContext app;
  char name[40];
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
    w[n++] = XmCreateLabel(form[l], name, l_args, XtNumber(l_args));

    sprintf(name, "text%d", l);
    w[n++] = XmCreateTextField(form[l], name, t_args, XtNumber(t_args));

    XtManageChildren(w, n);
  }

  XtManageChildren(form, l);
  XtManageChild(bigform);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


