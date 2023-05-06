
#include <X11/Intrinsic.h> /* include standard */
#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h> /* widget d'un toolkit */
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h> 

String fallback[] = {
	"*labelise.label: Good Bye World",
	"*bouton.label: Click and Die",
	NULL,
};
void sortir(w, client_data,call_data)
	Widget w; caddr_t call_data; caddr_t client_data;
{
	exit(0);
}

main(argc,argv)
	int argc;
	char *argv[];
{
	Widget toplevel, box, label,button;
	XtAppContext app;
	Arg args[1];
	int i;

	toplevel = XtAppInitialize(&app,"Test", (XrmOptionDescList) NULL, 0,
			&argc, argv, fallback, (ArgList)NULL, 0);
	box = XtCreateManagedWidget("boite", formWidgetClass,
			toplevel, NULL,0);
	label = XtCreateManagedWidget("labelise", labelWidgetClass,
			box, (Arg *)NULL, 0);
	i=0;
	XtSetArg(args[i],XtNfromHoriz,label); i++;
	button = XtCreateManagedWidget("bouton", commandWidgetClass,
			box, args,i);
	XtAddCallback(button,XtNcallback,sortir,NULL);
	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}
