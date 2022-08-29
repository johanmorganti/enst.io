
/* ********************************************************************* *
 *
 *  form_attach_all.c : Utilisation du Form Manager
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
  Arg args[10];
  int i;

  /* **************************************** Creation des Peres ... *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv,
			     NULL, NULL, 0);

  form = XmCreateForm(toplevel, "box", NULL, 0);


  /* **************************************** Creation des Fils ... *** */

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  boutons[0] = XmCreatePushButton(form, "bouton1", args, i);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNleftWidget, boutons[0]); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  boutons[1] = XmCreatePushButton(form, "bouton2", args, i);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, boutons[0]); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
  boutons[2] = XmCreatePushButton(form, "bouton3", args, i);

  i = 0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNtopWidget, boutons[1]); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNleftWidget, boutons[2]); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
  boutons[3] = XmCreatePushButton(form, "bouton4", args, i);

  /* ************************************** Manager les widgets ******** */

  XtManageChildren(boutons, 4);
  XtManageChild(form);	          /* manager le conteneur APRES ses fils */


  /* ************************************** Realisation et Lancement *** */


  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


