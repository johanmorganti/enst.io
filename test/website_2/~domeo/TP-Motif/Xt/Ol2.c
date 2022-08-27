#include <X11/Intrinsic.h> /* include standard */
#include <X11/StringDefs.h>
#include <Xol/OpenLook.h>

#include <Xol/StaticText.h> /* widget d'un toolkit */
#include <Xol/Form.h>
#include <Xol/OblongButt.h> 

String fallback[] = {
	"*labelise.string: Good Bye World",
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
	Widget toplevel, form, label,button;
	XtAppContext app;
	Arg args[3];
	int i;

	OlToolkitInitialize(0);

	toplevel = XtAppInitialize(&app,"Test", (XrmOptionDescList) NULL, 0,
			&argc, argv, fallback, (ArgList)NULL, 0);
	form = XtCreateManagedWidget("boite", formWidgetClass,
			toplevel, NULL,0);
	label = XtCreateManagedWidget("labelise", staticTextWidgetClass,
			form, (Arg *)NULL, 0);
	i=0;
	XtSetArg(args[i], XtNyRefWidget, (caddr_t) label);i++;
        XtSetArg(args[i], XtNxAddWidth, (caddr_t) True);i++;
        XtSetArg(args[i], XtNyAddHeight, (caddr_t) True);i++;

	button = XtCreateManagedWidget("bouton", oblongButtonWidgetClass,
			form, args,i);
	XtAddCallback(button,XtNselect,sortir,NULL);
	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}
