#include <X11/Intrinsic.h> /* include standard */
#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h> /* widget d'un toolkit */
#include <X11/Xaw/Box.h>

static String fallback[]={
        "*foreground:           white",
        "*background:           black",
        NULL,
};

main(argc,argv)
	int argc;
	char *argv[];
{
	Widget toplevel, box, label;
	XtAppContext app;
	Arg args[3];
	int i;
	Pixel fg,bg;


	toplevel = XtAppInitialize(&app,"Test",
			(XrmOptionDescList) NULL, 0,
			&argc, argv,
/*			fallback,	*/
			(String *) NULL,
			 (ArgList)NULL, 0);
	bg = BlackPixelOfScreen(XtScreen(toplevel));
	fg = WhitePixelOfScreen(XtScreen(toplevel));
	box = XtCreateManagedWidget("boite", boxWidgetClass,
			toplevel, NULL,0);
	i=0;
/*	XtSetArg(args[i],XtNlabel,"texte"); i++;	*/
/*	XtSetArg(args[i],XtNforeground,fg); i++;	*/
/*	XtSetArg(args[i],XtNbackground,bg); i++;	*/

	label = XtCreateManagedWidget("labelise", labelWidgetClass,
			box, (Arg *) NULL,0); 

/*			box, args, i); 	*/
	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}
