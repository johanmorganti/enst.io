/* xm2.c : creer un label et un bouton */

#include <stdio.h>
#include <string.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
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
	Widget toplevel, rowcol, label,button;
	XtAppContext app;
	Arg args[1];
	int i;
				/* initialisation */

	toplevel = XtAppInitialize(&app,"Test", 
				   (XrmOptionDescList) NULL, 0,
				   &argc, argv, 
				   fallback,
				   (ArgList) NULL, 0);

				/* creation du conteneur principal  */
	i = 0;
	XtSetArg(args[i], XmNorientation, XmHORIZONTAL); i++;
/*	XtSetArg(args[i], XmNorientation, XmVERTICAL); i++;  */
	rowcol = XmCreateRowColumn(toplevel, "rowcol", args, i);
	XtManageChild(rowcol);

				/* creation du Label */

	label = XmCreateLabel(rowcol, "labelise", NULL, 0);
	XtManageChild(label);

				/* cretion du boutton  */

	button = XmCreatePushButton(rowcol, "bouton", NULL, 0);
	XtAddCallback(button, XmNactivateCallback, sortir, NULL);
	XtManageChild(button);

	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}


