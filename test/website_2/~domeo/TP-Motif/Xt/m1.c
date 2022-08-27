/* xm1.c : creer un label dans une fenetre */

#include <stdio.h>
#include <string.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>

static String fallback[]={
        "*foreground:           white",
        "*background:           black",
        NULL,
};

main(argc,argv)
     int argc;
     char *argv[];
{
        Widget toplevel, rowcol, label;
	XtAppContext app;
	Arg args[5];
	int i;
	Pixel fg,bg;
	XmString xm_string;

	/* *** initialisation **************************************** */
	/* ************************************ initialisation ******* */

	toplevel = XtAppInitialize(&app,"Test",
				   (XrmOptionDescList) NULL, 0,
				   &argc, argv,
				   /* fallback,	*/
				   (String*) NULL,
				   (ArgList) NULL, 0);

	bg = BlackPixelOfScreen(XtScreen(toplevel));
	fg = WhitePixelOfScreen(XtScreen(toplevel));

	/* ************************************************************ */
	/* *********************** creation du conteneur principal **** */
/*
	rowcol = XtCreateManagedWidget("rowcol", xmRowColumnWidgetClass,
				       toplevel, NULL, 0);
 */
	rowcol = XmCreateRowColumn(toplevel, "rowcol", NULL, 0);
	XtManageChild(rowcol);

	xm_string = XmStringCreate("Message .....", XmSTRING_DEFAULT_CHARSET);

	/* ************************************************************ */
	/* *** Creation du Label ************************************** */

	i = 0;
	XtSetArg(args[i], XmNlabelString, xm_string); i++;
	XtSetArg(args[i], XmNforeground, fg); i++;
	XtSetArg(args[i], XmNbackground, bg); i++;

/*
        label = XtCreateManagedWidget("labelise", xmLabelWidgetClass,
	                              rowcol, args, i);
 */

	label = XmCreateLabel(rowcol, "labelise", args, i);
	XtManageChild(label);
	XmStringFree(xm_string);             /* !!! a ne pas oublier ! */


	XtRealizeWidget(toplevel);

	XtAppMainLoop(app);
}



