/*	troisDCP.c */

#include <stdio.h>
#include <X11/Intrinsic.h>	/* include standard pour une appli. utilisant*/
#include <X11/StringDefs.h>	/* le toolkit Xt */
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Shell.h>

#include <X11/Xaw/Form.h>		/* include specifique de l'appli. */
#include <X11/Xaw/Command.h>

#include "ThreeD.h"

extern int _Xdebug;

static String fallback[]={
	"*foreground:		black",
	"*background:		white",
	NULL,
};

#define CELLSIZE 8

#define COL 32
#define LIG 32

Atom T3target;

float tf[COL][LIG];

Widget toplevel;
Widget tmpfreq;
Widget board;

Boolean convert_proc(w,sel,target,typeRet,valRet,lenRet,formatRet)
	Widget w;
	Atom *sel;
	Atom *target;
	Atom *typeRet;
	XtPointer * valRet;
	unsigned long *lenRet;
	int *formatRet;
{
	char * data;
	if (*target == XA_TARGETS(XtDisplay(w))){
		printf("on me demande TARGETS\n");
		return True;
	}
	if (*target == T3target){
		int i,j;
		/* data est structure comme COL LIG float... */
		*lenRet = 2 * sizeof(int) + sizeof(float)*COL*LIG;
		*valRet = data = (char*) XtMalloc( *lenRet);
		*formatRet = 8;
		*typeRet = T3target;
		*(int*)data = COL;
		data += sizeof(int);
		*(int*)data = LIG;
		data += sizeof(int);
		for (i=0 ; i < COL;i++)
			for(j=0;j<LIG;j++){
				*(float*)data = tf[i][j] /2.0 ;
				data += sizeof(float);
			}
		return True;
	}
	return False;
}
void copy( w ,clid,calld)
	Widget w;
	XtPointer clid;
	XtPointer calld;
{
	/* save data & ownselection */
	/* il n'y a rien a sauver puisque les donnees sont dans tf */

	if ( XtOwnSelection(tmpfreq,XA_PRIMARY,CurrentTime,convert_proc,
		NULL,NULL) == False)
		XtWarning("Can't do copy\n");
}
void requestorCB(w,clid,sel,type,value,len,format)
	Widget w;
	XtPointer clid;
	Atom * sel;
	Atom * type;
	XtPointer value;
	unsigned long *len;
	int * format;
{
	int col;
	int lig;
	int i;
	int j;

	char * data = value;
	if ((value == NULL ) && (*len ==0)){
		XtWarning("NO selection\n");
		return;
	}
	col = *(int*) data;
	data +=sizeof(int);
	lig = *(int*)data;	
	data +=sizeof(int);
	for (i=0; i< col;i++)
		for (j=0;j<lig;j++){
			tf[i][j]= *(float*)data;
			data += sizeof(float);
		}
	TksThreeDMettreThreeD(tmpfreq,tf,col,lig);
}
void paste(w,clid,calld)
	Widget w;
	XtPointer clid;
	XtPointer calld;
{
	XtGetSelectionValue(tmpfreq,XA_PRIMARY,T3target,requestorCB,NULL,
		CurrentTime);
}

void sortir (w,clid,calld)
	Widget w;
	XtPointer clid;
	XtPointer calld;
{
	exit(0);
}
void trace(w,clid,calld)
	Widget w;
	XtPointer clid;
	XtPointer calld;
{
	TksThreeDMettreThreeD(tmpfreq,tf,COL,LIG);
}
main(argc,argv)
	int argc;
	char *argv[];
{
	XtAppContext app_context;
	Widget board;
	Widget bouttonSortir;
	Widget bouttonCopy;
	Widget bouttonPaste;
	Widget bouttonTrace;
	Arg args[6];
	int i=0;

	_Xdebug = 1;
 
	gettf();
	XtSetArg(args[i], XtNtitle, "Example Trois Dimension"); i++;
	toplevel = XtAppInitialize( &app_context, "top",
			NULL, 0, &argc, argv,
			fallback, args, i);
	T3target = XInternAtom(XtDisplay(toplevel),"TARGET_3D",False);

        board = XtCreateManagedWidget("board",formWidgetClass,toplevel,
                        NULL,0);
	i=0;
        XtSetArg(args[i], XtNborderWidth, 2);i++;
	XtSetArg(args[i], XtNheight,(caddr_t) 250);i++;
	XtSetArg(args[i], XtNwidth,(caddr_t) 400);i++;
	tmpfreq = XtCreateManagedWidget("tf",threeDWidgetClass,
			board, args,i);
/*        TksThreeDMettreThreeD(tmpfreq,tf,COL,LIG); */
        i=0;
	XtSetArg(args[i], XtNfromVert, tmpfreq);i++;
        bouttonSortir = XtCreateManagedWidget("Sortir  ",commandWidgetClass,
			board, args,i);
	XtAddCallback(bouttonSortir,XtNcallback,sortir,0);
        i=0;
	XtSetArg(args[i], XtNfromHoriz, bouttonSortir);i++;
	XtSetArg(args[i], XtNfromVert, tmpfreq);i++;
        bouttonCopy = XtCreateManagedWidget("Copy  ",commandWidgetClass,
			board, args,i);
	XtAddCallback(bouttonCopy,XtNcallback,copy,0);
        i=0;
	XtSetArg(args[i], XtNfromHoriz, bouttonCopy);i++;
	XtSetArg(args[i], XtNfromVert, tmpfreq);i++;
        bouttonPaste = XtCreateManagedWidget("Paste  ",commandWidgetClass,
			board, args,i);
	XtAddCallback(bouttonPaste,XtNcallback,paste,0);
        i=0;
	XtSetArg(args[i], XtNfromHoriz, bouttonPaste);i++;
	XtSetArg(args[i], XtNfromVert, tmpfreq);i++;
        bouttonTrace = XtCreateManagedWidget("Trace  ",commandWidgetClass,
			board, args,i);
	XtAddCallback(bouttonTrace,XtNcallback,trace,0);

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
