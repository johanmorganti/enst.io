
/* ********************************************************************* *
 *
 *  formulaire1.c :  (Form Widget avec Attachements et Positions)
 *  Utilisation du Form Manager pour representer un "formulaire".
 *
 *  REMARQUE : methode valide quand on sait quel est le plus grand widget
 *             (ie. le Text widget (en hauteur) dans le cas present)
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
  Widget toplevel, form, w[20];
  XtAppContext app;
  char name[40];
  int n;

/* *********************************************** ressources ********** */

  /* !! essayer fractionBase = 2, 4, .. */
  Arg f_args[] = {{XmNfractionBase, (XtArgVal) 3}};
  Arg l_args[] = {
    {XmNleftAttachment,  XmATTACH_FORM},
    {XmNrightAttachment, XmATTACH_POSITION},
    {XmNrightPosition,   (XtArgVal) 1},
  };
  Arg t_args[] = {
    {XmNleftAttachment,  XmATTACH_POSITION},
    {XmNleftPosition,    (XtArgVal) 1},
    {XmNrightAttachment, (XtArgVal) XmATTACH_FORM}
  };


  /* ****************************************** Toplevel et Manager *** */

  toplevel = XtAppInitialize(&app, "XFormulaire", NULL, 0, &argc, argv,
			     NULL, NULL, 0);

  form = XmCreateForm(toplevel, "form", f_args, XtNumber(f_args));

  /* ********************************************** Fils du Manager *** */

  n = 0;  
  while (n < XtNumber(w)) {

    sprintf(name, "label%d", n/2);
    w[n] = XmCreateLabel(form, name, l_args, XtNumber(l_args));
    n++;

    sprintf(name, "text%d", n/2);
    w[n] = XmCreateTextField(form, name, t_args, XtNumber(t_args));

    /* attacher en haut au TextField de la ligne precedente */
    if (n > 2)
      XtVaSetValues(w[n],
		    XmNtopAttachment,  (XtArgVal) XmATTACH_WIDGET,
		    XmNtopWidget,      (XtArgVal) w[n-2], 
		    NULL);

    /* attacher le Label en haut et en bas du TextField correspondant */
    /* (afin que les deux widgets soient centres verticalement) */

    XtVaSetValues(w[n-1],
		  XmNtopAttachment,    (XtArgVal) XmATTACH_OPPOSITE_WIDGET,
		  XmNtopWidget,        (XtArgVal) w[n], 
		  XmNbottomAttachment, (XtArgVal) XmATTACH_OPPOSITE_WIDGET,
		  XmNbottomWidget,     (XtArgVal) w[n], 
		  NULL);
    n++;
  }

  /* ************************************** Manager les widgets ******** */

  XtManageChildren(w, n);
  XtManageChild(form);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


