/*
 *	Auteur : G.Dauphin 3/3/92
 */
#include <stdio.h>
#include <X11/Intrinsic.h>	/* include standard pour une appli. utilisant*/
#include <X11/StringDefs.h>	/* le toolkit Xt */
#include <X11/Xatom.h>
#include <X11/Shell.h>
#include <Xol/OpenLook.h>
#include <Xol/ControlAre.h>		/* include specifique de l'appli. */
#include <Xol/OblongButt.h>
#include <Xol/MenuButton.h>
#include <Xol/TextField.h>
#include <Xol/PopupWindo.h>
#include <Xol/Caption.h>

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
void verif_callback(w,clid,calld)
	Widget w;
	XtPointer clid,calld;
{
	Widget pop = (Widget)clid;
	char * filename;
	int size;
	OlTextFieldVerify *tfv = (OlTextFieldVerify*) calld;
	OlDefine ppin;

	XtVaGetValues(pop, XtNpushpin,&ppin, NULL);
	if(tfv->reason == OlTextFieldReturn){
		filename = OlTextFieldGetString(w,&size);
		printf("Le fichier est : %s\n", filename);
		if(ppin == OL_OUT)XtPopdown(pop);
	}
}
void popup_callback(w,clid,calld)
	Widget w;
	XtPointer clid,calld;
{
	Widget pop = (Widget) clid;
	XtPopup(pop, XtGrabNone);
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

void load_callback(w,clid,calld)
	Widget w;
	XtPointer clid;
	XtPointer calld;
{
	Widget t = (Widget)clid;
	char * filename;
	int size;

	filename = OlTextFieldGetString(t,&size);
	printf("Le fichier est : %s\n", filename);
}

TksMenuRec menu[] = {
	{"Menu entry 1",menu_entry_callback, (caddr_t) 1},
	{"Menu entry 2",menu_entry_callback, (caddr_t) 2},
	{"Sortir", sortirApplication,NULL},
};
Widget toplevel, bouttonParametre, bouttonMenu;

main(argc,argv)
	int argc;
	char *argv[];
{
	XtAppContext app_context;
	Widget board, menu_pane,  wb;
	Widget popup, upper,lower,caption,load,tf;
	Arg args[11];
	int i=0;

	OlToolkitInitialize(0);

	XtSetArg(args[i], XtNtitle, "Exemple de menu"); i++;
	toplevel = XtAppInitialize( &app_context, "Test",
			NULL, 0, &argc, argv,
			fallback, args, i);
        board = XtCreateManagedWidget("board",controlAreaWidgetClass,toplevel,
                        NULL,0);
	bouttonMenu = XtCreateManagedWidget("Menu1",menuButtonWidgetClass,
			board, NULL,0);
	XtSetArg(args[0], XtNlabelType, OL_POPUP);
	bouttonParametre =XtCreateManagedWidget("Load",oblongButtonWidgetClass,
                        board, args,1);
/*
 * Obtenir le menupane
 */
	i = 0;
	XtSetArg(args[0],XtNmenuPane,&menu_pane);i++;
	XtGetValues(bouttonMenu,args,i);
/*
 * Creation des boutons du Menu1
 */
	for(i=0; i<  XtNumber(menu); i++){
		wb =  XtCreateManagedWidget(menu[i].texte,
		      oblongButtonWidgetClass,menu_pane,NULL,0);
		if (menu[i].f != NULL)
			XtAddCallback(wb,XtNselect, menu[i].f,menu[i].param);
	}
/*
 * Creation du Popup pour demander le nom de fichier
 */
	popup = XtVaCreatePopupShell("loadpopup",
			popupWindowShellWidgetClass, bouttonParametre,
			XtNtitle, "Load File",
			NULL);
	XtVaGetValues(popup,
			XtNupperControlArea,&upper,
			XtNlowerControlArea,&lower,
			NULL);
	caption = XtVaCreateManagedWidget("File name : ",
			captionWidgetClass,upper,
			NULL);
	tf = XtVaCreateManagedWidget("popuptext", 
                        textFieldWidgetClass,caption,
			XtNwidth,400,
			NULL);
	XtAddCallback(tf,XtNverification,verif_callback,popup);
	load = XtVaCreateManagedWidget("popupload", 
                        oblongButtonWidgetClass,lower,
			XtNlabel, "Load",
			NULL);
	XtAddCallback(load,XtNselect,load_callback, (XtPointer)(tf));	

	XtAddCallback(bouttonParametre,XtNselect, popup_callback,popup);
	XtRealizeWidget(toplevel);
	XtAppMainLoop(app_context);
}
