
/* ********************************************************************* *
 *
 *  form_pos.c : Utilisation du Form Manager (Positions)
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>


/* ********************************************************************* */

main(int argc, char *argv[])
{
  Widget toplevel, form, boutons[4];
  XtAppContext app;
  char name[40];
  int n, x, y;

  /* ****************************************** Toplevel et Manager *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv,
			     NULL, NULL, 0);
  {
    Arg args[] = {{XmNfractionBase, (XtArgVal) 2}};
    form = XmCreateForm( toplevel, "form", args, XtNumber(args) );
  }

  /* ********************************************** Fils du Manager *** */

  for (n = 0, y = 0;  y < 2;  y++)
    for (x = 0;  x < 2;  x++, n++) {

      sprintf(name, "bouton%d", n + 1);
      boutons[n] = XmCreatePushButton( form, name, NULL,0 );

      XtVaSetValues(boutons[n],

		    XmNtopAttachment,    (XtArgVal) XmATTACH_POSITION,
		    XmNtopPosition,      (XtArgVal) y, 
		    XmNleftAttachment,   (XtArgVal) XmATTACH_POSITION,
		    XmNleftPosition,     (XtArgVal) x,

    /* uniquement pour 2e cas : */
		    XmNbottomAttachment, (XtArgVal) XmATTACH_POSITION,
		    XmNbottomPosition,   (XtArgVal) y+1,
		    XmNrightAttachment,  (XtArgVal) XmATTACH_POSITION,
		    XmNrightPosition,    (XtArgVal) x+1,
		    NULL);
    }

  /* ************************************** Manager les widgets ******** */

  XtManageChildren(boutons, n);
  XtManageChild(form);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


