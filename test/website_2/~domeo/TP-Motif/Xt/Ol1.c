#include <X11/Intrinsic.h> /* include standard */
#include <X11/StringDefs.h>

#include <Xol/StaticText.h> /* widget d'un toolkit */

static String fallback[]={
        "*foreground:           white",
        "*background:           black",
        NULL,
};

main(argc,argv)
	int argc;
	char *argv[];
{
	Widget olinit,toplevel, box, label;
	XtAppContext app;
	Arg args[3];
	int i;
	Pixel fg,bg;

	OlToolkitInitialize(NULL);

	toplevel = XtAppInitialize(&app,"Test",
			(XrmOptionDescList) NULL, 0,
			&argc, argv,
/*			fallback,	*/
			(String *) NULL,
			 (ArgList)NULL, 0);
	bg = BlackPixelOfScreen(XtScreen(toplevel));
	fg = WhitePixelOfScreen(XtScreen(toplevel));
	i=0;
	XtSetArg(args[i],XtNstring,"texte de la widget"); i++;
/*	XtSetArg(args[i],XtNforeground,fg); i++;	*/
/*	XtSetArg(args[i],XtNbackground,bg); i++;	*/

	label = XtCreateManagedWidget("labelise", staticTextWidgetClass,
			toplevel, args, i);
	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}
