/* xm3.c : creer un label et un bouton (version 2) */

#include <stdio.h>
#include <string.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>


String fallback[] = {
	"*labelise.labelString: Good Bye World",
	"*bouton.labelString: Click and Die",
	NULL,
};

void sortir(w, client_data, call_data)
	Widget w; caddr_t client_data, call_data;
{
	exit(0);
}

main(argc,argv)
	int argc;
	char *argv[];
{
	Widget toplevel, form, label,button;
	XtAppContext app;
	Arg args[10];
	int i;

	/* **************************************** initialisation **** */

	toplevel = XtAppInitialize(&app,"Test", 
				   (XrmOptionDescList) NULL, 0,
				   &argc, argv, 
				   fallback,
				   (ArgList) NULL, 0);

	/* *********************** creation du conteneur principal **** */

	form = XmCreateForm(toplevel, "form", NULL, 0);
	XtManageChild(form);

	/* ************************************* creation du Label **** */

	i = 0;
	XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
	XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;

	XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;

	label = XmCreateLabel(form, "labelise", args, i);
	XtManageChild(label);

	/* *********************************** creation du boutton **** */

	i = 0;
	XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
	XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
	XtSetArg(args[i], XmNleftWidget, label); i++;

	XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;

/*	XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++; */

	button = XmCreatePushButton(form, "bouton", args, i);
	XtAddCallback(button, XmNactivateCallback, sortir, NULL);
	XtManageChild(button);

	/* ********************************************* lancement **** */

	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}


