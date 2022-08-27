#include <Xm/Xm.h>
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
	Widget toplevel, box, label;
	XtAppContext app;
	Arg args[3];
	int i;
	Pixel fg,bg;
	XmString lbl_txt;


	toplevel = XtAppInitialize(&app,"Test",
			(XrmOptionDescList) NULL, 0,
			&argc, argv,
			fallback,
			(String *) NULL,
			 (ArgList)NULL, 0);
	i=0;
	lbl_txt = XmStringCreateLtoR("texte de la widget",XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i],XmNlabelString,lbl_txt); i++;

	label = XtCreateManagedWidget("labelise", xmLabelWidgetClass,
			toplevel, args, i);
	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}
