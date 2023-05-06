/*
 *	Auteur : G.Dauphin 3/3/92
 */


#include <stdio.h>
#include <X11/Intrinsic.h>	/* include standard pour une appli. utilisant*/
#include <X11/StringDefs.h>	/* le toolkit Xt */
#include <X11/Xatom.h>
#include <X11/Shell.h>

#include <X11/Xaw/Form.h>		/* include specifique de l'appli. */
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>

typedef struct _MenuRec {
        char * texte;
        void (*f)();
        caddr_t param;
        Widget w;
} TksMenuRec;

static String fallback[]={
	"*foreground:		black",
	"*background:		white",
	NULL,
};


void sortirApplication();
void fixerParametre();
void CreerMenuDuBoutton();
void menu_entry_callback();

TksMenuRec menuFichier[] = {
	{"Menu entry 1",menu_entry_callback, (caddr_t) 1},
	{"Menu entry 2",menu_entry_callback, (caddr_t) 2},
	{"Sortir", sortirApplication,NULL},
};

Widget toplevel;
Widget bouttonParametre;
Widget pshellParametre;		/* popup pour paramtre */
Widget dialogParametre;
Widget okParametre;
Widget bouttonFichier;

void FinDialogParametre();
void FinDialogFichier();
char textdialog[]="\0                                          ";

main(argc,argv)
	int argc;
	char *argv[];
{
	XtAppContext app_context;
	Widget board;
	Widget menuBoard;
	Widget textw;
	Arg args[11];
	static XtActionsRec actions[] = {
		{"FinDialogParametre",FinDialogParametre},
	};
	int i=0;

	XtSetArg(args[i], XtNtitle, "Exemple de menu"); i++;
	toplevel = XtAppInitialize( &app_context, "Test",
			NULL, 0, &argc, argv,
			fallback, args, i);
	XtAppAddActions( XtWidgetToApplicationContext(toplevel),
                    actions, XtNumber(actions));
	/* creation du popup parametre */
	pshellParametre = XtCreatePopupShell("pshellParametre",
		transientShellWidgetClass,
		toplevel, NULL,0);
	i=0;
	XtSetArg(args[i],XtNlabel,"Entree un parametre");i++;
	XtSetArg(args[i],XtNvalue,textdialog);i++;
	dialogParametre = XtCreateManagedWidget("dialogParametre",
		dialogWidgetClass, pshellParametre,args,i);
	okParametre = XtCreateManagedWidget("ok",commandWidgetClass,
		dialogParametre,NULL,0);
	/* pour que le return soit OK */
	textw = XtNameToWidget(dialogParametre,"value");
	XtOverrideTranslations(textw,
		XtParseTranslationTable("<Key>Return: FinDialogParametre()"));
	XtAddCallback(okParametre,XtNcallback,FinDialogParametre,NULL);

        board = XtCreateManagedWidget("board",formWidgetClass,toplevel,
                        NULL,0);

	i=0;
	XtSetArg(args[i], XtNbottom, XtChainTop);i++;
	XtSetArg(args[i], XtNtop, XtChainTop);i++;
	XtSetArg(args[i], XtNleft, XtChainLeft);i++;
	XtSetArg(args[i], XtNright, XtChainLeft);i++;
	/* contient les bouttons pour les menus */
	menuBoard = XtCreateManagedWidget("menuBoard",formWidgetClass,board,
                        args,i);
	bouttonFichier = XtCreateManagedWidget("Fichier",commandWidgetClass,
			menuBoard, args,i);
	i--;
	XtSetArg(args[i],XtNfromHoriz,bouttonFichier);i++;
	bouttonParametre =XtCreateManagedWidget("Nb Signaux",commandWidgetClass,
                        menuBoard, args,i);
	CreerMenuDuBoutton(bouttonFichier,"Fichier",menuFichier,
			XtNumber(menuFichier));
	XtAddCallback(bouttonParametre,XtNcallback,fixerParametre,0);

	XtRealizeWidget(toplevel);
	XtAppMainLoop(app_context);
}

void sortirApplication()
{
	exit(0);
}
void menu_entry_callback( w, clid, calld)
	Widget w;
	int clid;
	caddr_t calld;
{
	printf("je choisi le menu %d\n",clid);
}

void fixerParametre(w,client_data,call_data)
        Widget w;
        caddr_t client_data, call_data;
{
	Position x,y;
	Dimension wi,h;
	Arg arg[2];
	int i;

	i = 0;
	XtSetArg(arg[i],XtNwidth,&wi); i++;
	XtSetArg(arg[i],XtNheight,&h); i++;
	XtGetValues(bouttonParametre,arg,i);
	XtTranslateCoords(bouttonParametre, 0, h, &x , &y);
	i=0;
	XtSetArg(arg[i],XtNx,x);i++;
	XtSetArg(arg[i],XtNy,y);i++;
	XtSetValues(pshellParametre,arg,i);
	XtPopup(pshellParametre, XtGrabNone);
}

void FinDialogParametre(w,d,call_data)
        Widget w;
	caddr_t d;
        caddr_t call_data;
{
	String s;

	XtPopdown(pshellParametre);
	s = XawDialogGetValueString(dialogParametre);
	printf("valeur du parametre: %s\n",s);
}




#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>

static void menupopup();
static void menupopdown();
static void setentry();

void CreerMenuDuBoutton(button,label,mliste,nmenu)
	Widget button;
	char * label;
	TksMenuRec * mliste;
	int nmenu;
{
	Widget pshell;
	Widget menulabel;
	Widget menubox;
	Widget wb;
	int i;

	pshell = XtCreatePopupShell("pshell",simpleMenuWidgetClass,
			button,NULL,0);
	XtOverrideTranslations(button,
		XtParseTranslationTable(
			"<EnterWindow>: highlight()\n\
			 <LeaveWindow>: reset()\n\
			 <Btn1Down>: set() notify() reset()MenuPopup(pshell)"));

	XtAddCallback(button,XtNcallback,menupopup,pshell);


	for(i=0; i< nmenu; i++){
		wb = mliste[i].w = XtCreateManagedWidget(mliste[i].texte,
		      smeBSBObjectClass,pshell,NULL,0);
		if (mliste[i].f != NULL)
			XtAddCallback(wb,XtNcallback, mliste[i].f,mliste[i].param);
		XtAddCallback(wb,XtNcallback,
			menupopdown,pshell);
	}
}
static void menupopup(w,pshell,call_data)
	Widget w;
	Widget pshell;
	caddr_t call_data;
{
	Arg arg[2];
	Position x,y;
	int i=0;

	XtTranslateCoords(w, (Position) 0, (Position) 0,
			&x, &y);
	XtSetArg(arg[i],XtNx,x+10);i++;
	XtSetArg(arg[i],XtNy,y+10);i++;
	XtSetValues(pshell,arg,i);
	/*XtPopupSpringLoaded(pshell);*/
}

static void menupopdown(w,pshell,call_data)
        Widget w;
        Widget pshell;
        caddr_t call_data;
{
	XtPopdown(pshell);
}

typedef struct _EntryInfo {
	TksMenuRec * liste;
	int dimliste;
	unsigned int *entry;
} EntryInfo;

static Pixmap mark = NULL;

#define check_width 9
#define check_height 8
static char check_bits[] = {
   0x00, 0x01, 0x80, 0x01, 0xc0, 0x00, 0x60, 0x00,
   0x31, 0x00, 0x1b, 0x00, 0x0e, 0x00, 0x04, 0x00
};

void CreerMenuExclusifDuBoutton(button,label,mliste,nmenu,entry)
	Widget button;
	char * label;
	TksMenuRec * mliste;
	unsigned int nmenu;
	unsigned int *entry;
{
	Widget pshell;
	Widget menulabel;
	Widget menubox;
	Widget wb;
	EntryInfo * entryinfo;
	int i,j;
	Arg args[2];

	if(!mark)
		mark = XCreateBitmapFromData(XtDisplay(button),
			RootWindowOfScreen(XtScreen(button)),
			check_bits, check_width, check_height);
	if (nmenu == 0) return;
	if( *entry >= nmenu) *entry = 0;
	entryinfo = (EntryInfo *) XtMalloc( sizeof(EntryInfo));
	entryinfo->liste = mliste;
	entryinfo->dimliste = nmenu;
	entryinfo->entry = entry;

	pshell = XtCreatePopupShell("pshelle",simpleMenuWidgetClass,
			button,NULL,0);
	XtOverrideTranslations(button,
		XtParseTranslationTable(
			"<EnterWindow>: highlight()\n\
			 <LeaveWindow>: reset()\n\
			 <Btn1Down>: set() notify() reset()MenuPopup(pshelle)"));

	XtAddCallback(button,XtNcallback,menupopup,pshell);

	XtSetArg(args[0],XtNleftMargin,16);
	XtSetArg(args[1],XtNleftBitmap,mark);
	for(i=0; i< nmenu; i++){
		j = (i == *entry)? 2 : 1 ;
		wb = mliste[i].w = XtCreateManagedWidget(mliste[i].texte,
		      smeBSBObjectClass,pshell,args,j);
		if (mliste[i].f != NULL)
			XtAddCallback(wb,XtNcallback,mliste[i].f,mliste[i].param);
		XtAddCallback(wb,XtNcallback,
			menupopdown,pshell);
		XtAddCallback(wb,XtNcallback,
			setentry,entryinfo);
	}
}
static void setentry(w,ei,cd)
	Widget w;
	EntryInfo * ei;
	caddr_t cd;
{
	Arg args[1];
	int i;
	unsigned int mis;

	mis = *(ei->entry);
	for(i=0; i < ei->dimliste; i++)
		if( w == ei->liste[i].w) break;
	if (i == mis) return;
	XtSetArg(args[0],XtNleftBitmap,mark);
	XtSetValues(w,args,1);
	XtSetArg(args[0],XtNleftBitmap,None);
	XtSetValues( ei->liste[mis].w,args,1);
	*(ei->entry) = i;
}

