
/* ********************************************************************* *
 *
 *  show.c
 *  GUI pour afficher ou sauvegarder un rasterfile.
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */



#include <Xm/XmAll.h>
#include "xmutil.h"

			       /* ressources par defaut de l'application */

String fallbacks[] = {
                            /* taille des images a l'ouverture */
  "*image.width: 450",
  "*image.height: 350",
  "*image*drawing*background: white",

  "*message.background: red",
  "*message.labelString: Couleur Test",
/*"*message.fontList:  -*-courier-bold-r-*-*-*-120-75-75-*-*-iso8859-1", */
/*"*message.fontList:  kanji24",*/
  "*message.fontList:  r24",

  "*texte.background: white",
				/* directory ou se trouvent les images */
  "*read_dialog.directory: ~/data/rasterfiles",
  "*read_dialog.pattern: *.rs",
  "*read_dialog.fileTypeMask: FILE_REGULAR",
  "*read_dialog.filterLabelString: Filtre",
  "*read_dialog.dirListLabelString: Repertoires",
  "*read_dialog.fileListLabelString: Fichiers",
  "*read_dialog.selectionLabelString: Fichier Image",
  "*read_dialog.okLabelString: Read",

  "*exit_dialog.dialogStyle: DIALOG_FULL_APPLICATION_MODAL",
  "*exit_dialog.x: 450",
  "*exit_dialog.y: 350",
  "*exit_dialog.defaultPosition: False", /* necessaire si x et y specifies */
  "*exit_dialog.messageString: Terminer l'application ?",
  "*exit_dialog.okLabelString: EXIT",
  
  NULL
};

/* ===================================================================== */
/* variable globale */

Display *display = NULL;	/* utilisé dans file.c */


/* ===================================================================== */
/* variables statiques (i.e. locales a ce fichier) */

static XImage *ximage = NULL;
static Widget message = NULL;


/* ===================================================================== */
/* ===================================================================== */
/* Afficher un message */

Message(String text)
{
  XmString xms = StringCreate(text);
  XtVaSetValues(message, XmNlabelString, xms, NULL);
  XmStringFree(xms);
}

/* ===================================================================== */
/* ===================================================================== */
/* Fonctions de Callbacks : */
/* ===================================================================== */

static void ReadProc(Widget dialog, XtPointer drawing, XtPointer calldata)
{
  char *str = 
    StringGet(((XmFileSelectionBoxCallbackStruct*)calldata)->value);

  if (str && *str) {
    if (ximage = ReadImage(str)) {
      DisplayImage((Widget)drawing, ximage);
      Message("Nouvelle image");
      XtUnmanageChild(dialog);
    }
    else Message("! Fichier non trouvé");
  }
  if(str) XtFree(str);
}

/* ===================================================================== */

static void SaveProc(Widget dialog, XtPointer client_data, XtPointer call_data)
{
  char *str =
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);

  if (str && *str) {
    if (! ximage) Message("! Pas d'image");
    else {
      Message("Image sauvegardée");
      WriteImage(str, ximage);
      XtUnmanageChild(dialog);
    }
  }
  if(str) XtFree(str);
}

/* ==================================================== ======== ======= */

void ExitProc(Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}

/* ==================================================== ======== ======= */

void MyProc(Widget w, XtPointer client_data, XtPointer call_data)
{
  String str = (String) client_data;
  if (! str) str = "[Nothing]";
  printf("> Procedure MyProc, data = %s \n", str);
}

/* ==================================================== ======== ======= */
/* ==================================================== ======== ======= */

main(int argc, char *argv[])
{
  Widget 
    toplevel, mainbox, menubar, textfield, scrollwin, drawing,
    read_dialog, save_dialog, exit_dialog;
  XtAppContext app;
  Arg args[10];
  int i;

  /* ************************************************ Initialisation *** */

  toplevel = XtVaAppInitialize(&app, "XShow", NULL, 0, &argc, argv, fallbacks, 
			       XmNallowShellResize, True,
			       NULL);

  display = XtDisplay(toplevel);	/* utilisé dans file.c */

  /* ******************************************* creation des Managers *** */


  mainbox = XmCreateMainWindow(toplevel, "mainbox", NULL, 0);
  XtManageChild(mainbox);

  menubar = XmCreateMenuBar(mainbox, "menubar", NULL, 0);
  XtManageChild(menubar);

  i = SetArgs(args, XtNumber(args),
	      /* ! ressource a specifier AVANT creation du widget */
	      XmNscrollingPolicy,        XmAUTOMATIC, 
	      XmNscrollBarDisplayPolicy, XmAS_NEEDED,
	      NULL);

  scrollwin = XmCreateScrolledWindow(mainbox, "image", args, i);
  XtManageChild(scrollwin);

  drawing = XmCreateDrawingArea(scrollwin, "drawing", NULL, 0);
  XtManageChild(drawing);

  textfield = XmCreateText(mainbox, "commande", NULL, 0);
  XtManageChild(textfield);

  message = XmCreateLabel(mainbox, "message", NULL, 0);
  XtManageChild(message);

  XtVaSetValues(mainbox,
		XmNmenuBar, menubar,
		XmNcommandWindow, textfield,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNworkWindow, scrollwin,
		XmNmessageWindow, message,
		XmNshowSeparator, True,
		NULL);

  /* ******************************************* creation des Dialogues *** */

  read_dialog = CreateDialog(mainbox, "read_dialog", 
			     XmCreateFileSelectionDialog,
			     ReadProc, (XtPointer)drawing);

  save_dialog = CreateDialog(mainbox, "save_dialog", XmCreatePromptDialog,
			     SaveProc, NULL);

  exit_dialog = CreateDialog(mainbox, "exit_dialog", XmCreateErrorDialog,
			     ExitProc, NULL);


  /* *********************************** creation des CascadeButtons *** */
  /*                                     et des PulldownMenus associes   */

  CreateMenu(menubar, "file",
	     "read...",  OpenDialog, read_dialog, 
	     "write...", OpenDialog, save_dialog, 
	     "exit...",  OpenDialog, exit_dialog, 
	     NULL);


  CreateMenu(menubar, "close",
	     "close1",  CloseDialog, read_dialog, 
	     "close2",  CloseDialog, save_dialog, 
	     "close3",  CloseDialog, exit_dialog, 
	     NULL);

  CreateMenu(menubar, "view",
	     "view1",  MyProc,  "procedure 1",
	     "view2",  MyProc,  "procedure 2",
	     "view3",  MyProc,  "etc...",
	     NULL);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}
