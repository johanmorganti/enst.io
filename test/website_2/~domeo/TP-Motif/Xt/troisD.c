/*	troisD.c */

#include <stdio.h>
#include <X11/Intrinsic.h>	/* include standard pour une appli. utilisant*/
#include <X11/StringDefs.h>	/* le toolkit Xt */
#include <X11/Shell.h>

#include <X11/Xaw/Form.h>		/* include specifique de l'appli. */
#include <X11/Xaw/Command.h>

#include "ThreeD.h"

extern int _Xdebug;		/* non documente */

static String fallback[]={
	"*foreground:		black",
	"*background:		white",
	NULL,
};

void sortir ()
{
	exit(0);
}

#define COL 32
#define LIG 32
float tf[COL][LIG];

Widget toplevel;
Widget w3d;
Widget board;
main(argc,argv)
	int argc;
	char *argv[];
{
	XtAppContext app_context;
	Widget board;
	Widget bouttonSortir;
	Arg args[6];
	int i=0;

	_Xdebug = 1;
 
	gettf();
	XtSetArg(args[i], XtNtitle, "Example Trois Dimension"); i++;
	toplevel = XtAppInitialize( &app_context, "top",
			NULL, 0, &argc, argv,
			fallback, args, i);
        board = XtCreateManagedWidget("board",formWidgetClass,toplevel,
                        NULL,0);
	i=0;
        XtSetArg(args[i], XtNborderWidth, 2);i++;
	XtSetArg(args[i], XtNheight,(caddr_t) 250);i++;
	XtSetArg(args[i], XtNwidth,(caddr_t) 400);i++;
	w3d = XtCreateManagedWidget("tf",threeDWidgetClass,
			board, args,i);
        TksThreeDMettreThreeD(w3d,tf,COL,LIG);
        i=0;
	XtSetArg(args[i], XtNfromVert, w3d);i++;
        bouttonSortir = XtCreateManagedWidget("Sortir  ",commandWidgetClass,
			board, args,i);
	XtAddCallback(bouttonSortir,XtNcallback,sortir,0);

	XtRealizeWidget(toplevel);
	XtAppMainLoop(app_context);
}
gettf()
{
	/* z=cos(x*x +y*y)/(1+x*x+y*y) */
	double cos();
	int i,j;
	float x,y;

	for(i=0;i<COL;i++){
		for(j=0;j<LIG;j++){
			x = (j - LIG/2)/4.0;
			y = (i - COL/2)/4.0;
			tf[i][j] = 60.0*cos(x*x +y*y)/(1+x*x+y*y);
		}
	}
}
