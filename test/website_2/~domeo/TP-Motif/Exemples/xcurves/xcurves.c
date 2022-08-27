
/* ********************************************************************* *
 *
 *  xcurves.c
 *  GUI pour afficher des courbes.
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */

/*---------------------------------*/
/*            include              */
/*---------------------------------*/

#include <Xm/XmAll.h>
#include "xmutil.h"
#include "xplus.h"

/*---------------------------------*/
/*           definitions           */
/*---------------------------------*/

#define nbr_curves 5

typedef struct point { double abs;
		       double ord;
		       struct point *suivant;
		     } point;

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */
/* variables globales et statiques */

Display *display = NULL;	/* Display de l'application */
Window  draw_window;    	/* Fenetre du Drawing */
GC      draw_gc = NULL;		/* Graphic Context du Drawing */
static Widget message = NULL;	/* widget pour afficher les messages */
int ExternCompteur=0;           /* Numero de la prochaine courbe stockee
                                   en memoire */

/*---------------------------------*/
/*        variables externes       */
/*---------------------------------*/

extern int ecran[];
extern double x_scale[nbr_curves];
extern double y_scale[nbr_curves];
extern double ZoomChoice;
extern int LAST;
extern int Delta_x;
extern int Delta_y;
extern char CurveName[nbr_curves][100];
extern char *Color[];
extern MainScale_x;
extern MainScale_y;

/* ==================================================== ======== ======= */
/* ressources par defaut de l'application */
/* NB: peuvent etre redefinies dans via un "fichier de ressources" */

static String fallbacks[] = {
                           
  "*scrollarea.width: 450",		/* tailles initiales */
  "*scrollarea.height: 350",
  "*scrollarea*drawing.width: 550",
  "*scrollarea*drawing.height: 450",
  "*scrollarea*drawing*background: black",
  "*scrollarea.background: HotPink",

  "*mainscale.orientation: HORIZONTAL",

  "*message.background: black",
  "*message.foreground: yellow",
  "*message.labelString: - XCURVES -",
/*"*message.fontList:  -*-courier-bold-r-*-*-*-120-75-75-*-*-iso8859-1", */
/*"*message.fontList:  kanji24",*/
  "*message.fontList:  r24",
				
  "*read_dialog.directory: .",		/* directory par defaut */
  "*read_dialog.pattern: *.graph",
  "*read_dialog.fileTypeMask: FILE_REGULAR",
  "*read_dialog.filterLabelString: Filtre",
  "*read_dialog.dirListLabelString: Repertoires",
  "*read_dialog.fileListLabelString: Fichiers",
  "*read_dialog.selectionLabelString: Fichier Graph",
  "*read_dialog.okLabelString: Read",

  "*mainbox.background: black",

  "*exit_dialog.dialogStyle: DIALOG_FULL_APPLICATION_MODAL",
  "*exit_dialog.x: 450",
  "*exit_dialog.y: 350",
  "*exit_dialog.defaultPosition: False", /* necessaire si x et y specifies */
  "*exit_dialog.messageString: Terminer l'application ?",
  "*exit_dialog.okLabelString: EXIT",
  
  NULL
};

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */
/* Afficher un message dans le widget "message" */

Message(String text)
{
  XmString xms = StringCreate(text);
  XtVaSetValues(message, XmNlabelString, xms, NULL);
  XmStringFree(xms);
}

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */
/* Fonctions de Callbacks : */
/* ===================================================================== */


static void ReadFile(Widget dialog, XtPointer drawing, XtPointer calldata)
{
  int tempo;                 /* numero associe a la courbe lue */
  static int compteur=0;     /* varie entre 0 et nbr_curves-1 continuement
                                en boucle */
  FILE *pt_filename;

  char s[100], *filename = 
    StringGet(((XmFileSelectionBoxCallbackStruct*)calldata)->value);

  if (filename && *filename) {
    if (True) {       /* .... a completer en: if (ReadGraphFile() == True) */
      sprintf(s, "%s", filename);
      Message(s+44); /* 44 correspond a la longueur de la chaine
			/auto/cal/students/promo96/romanett/XCURVES/ */ 
      /* permet ainsi d'afficher JUSTE le nom du fichier lu */

      sprintf(CurveName[ExternCompteur], "%s", filename);
      tempo=ReadGraph(filename);
      fprintf(stderr,"%d",tempo);
    
  
      LAST=compteur;    
  
      ChangeColor(display, draw_gc, Color[ExternCompteur]);
      afficher(tempo);

      compteur++;
  
      ExternCompteur=compteur;

      if(compteur==nbr_curves)
	{
	  compteur=0;
	  ExternCompteur=compteur;
	}

      XtUnmanageChild(dialog);
    }
    else Message("! Fichier non trouvé");
  }
  if(filename)XtFree(filename);
}



/* ===================================================================== */

static void SaveFile(Widget dialog, XtPointer client_data, XtPointer call_data)
{
  char *str =
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);

  if (str && *str)
    {
/*
    if (! graph) Message("! Pas de graphique");
    else {
      Message("Image sauvegardée");
      WriteGraph(str, graph);
      XtUnmanageChild(dialog);
    }
*/
  }
  if(str) XtFree(str);
}

/* ==================================================== ======== ======= */

static void ExitProg(Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */

/*static void SetColor(Widget w, XtPointer color_name, XtPointer call_data)
{
  Color[LAST] = (char *)malloc(strlen((String)color_name));
  strcpy(Color[LAST], color_name);
  NewScreen();  
}*/

static void SetColor0(Widget w, XtPointer color_name, XtPointer call_data)
{
  Color[0] = strdup(color_name);
  NewScreen();
}

static void SetColor1(Widget w, XtPointer color_name, XtPointer call_data)
{
  Color[1] = strdup(color_name);
  NewScreen();
}

static void SetColor2(Widget w, XtPointer color_name, XtPointer call_data)
{
  Color[2] = strdup(color_name);
  NewScreen();
}

static void SetColor3(Widget w, XtPointer color_name, XtPointer call_data)
{
  Color[3] = strdup(color_name);
  NewScreen();
}

static void SetColor4(Widget w, XtPointer color_name, XtPointer call_data)
{
  Color[4] = strdup(color_name);
  NewScreen();
}

/* ==================================================== ======== ======= */

static void SelectColor(Widget dialog, XtPointer xxx, XtPointer call_data)
{
  char *color_name;
  int i,n;
  char s[100], *color_string = 
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);

  if (color_string && *color_string)
    {
      color_name=(char *)malloc(strlen(color_string));
      if ((i= sscanf(color_string, "%d %s", &n,color_name)) == 2)
      if (ChangeColor(display, draw_gc, color_name))
	{
   	  fprintf(stderr,"%s %d\n",color_name,ExternCompteur);
	  Color[n] = strdup(color_name);

	  fprintf(stderr,"%s\n",Color[n]);
	  sprintf(s, "Nouvelle couleur = %s", color_name);
	  Message(s);
	  XtUnmanageChild(dialog);		/* enlever le dialog */
	  ChangeColor(display, draw_gc, Color[n]);
	  NewScreen();
	  free(color_name);
	}
    else Message("Couleur inconnue");
    }

  if(color_name) XtFree(color_name);
}

/* ==================================================== ======== ======= */

static void SelectSize(Widget dialog, XtPointer drawing, XtPointer call_data)
{
  char s[100], *size_string = 
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);

  if (size_string && *size_string) {
    int n, w, h;
    if ((n = sscanf(size_string, "%d %d", &w, &h)) == 2) {
      XtVaSetValues(drawing, XmNwidth, w, XmNheight, h, NULL);
      sprintf(s, "Nouvelle taille = %d x %d", w, h);
      Message(s);
      XtUnmanageChild(dialog);		/* enlever le dialog */
    }
    else Message("Dimensions incorrectes");
  }
  if (size_string) XtFree(size_string);
}
/*======================================================================*/
/*static void choicemainscale(Widget mainscale, XtPointer call_data)
{
  int n, w;
   char s[100], *size_string = 
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);

  if ((n = sscanf(size_string, "%d", &w)) == 1)
    {
     MainScale_x=MainScale_y=w;
     NewScreen();
   }
}*/
/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */

static void Clear(Widget w, XtPointer client_data, XtPointer call_data)
{
  int i;

  XClearWindow(display, draw_window);
  ZoomChoice=1;
  Delta_x=0;
  Delta_y=0;
  ChangeColor(display, draw_gc, Color[nbr_curves]);
  AfficherAxes();

  for(i=0;i<=nbr_curves-1;i++)
    ecran[i]=0;
  Message("- XCURVES -");
}





/* ==================================================== ======== ======= */

static void DrawLine(Widget w, XtPointer client_data, XtPointer call_data)
{
  XDrawLine(display, draw_window, draw_gc, 25, 50, 350, 250);
}

/* ==================================================== ======== ======= */

static void DrawRect(Widget w, XtPointer client_data, XtPointer call_data)
{
  XDrawRectangle(display, draw_window, draw_gc, 50, 100, 250, 150);

}

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */

static void SelectText(Widget dialog, XtPointer clientd, XtPointer calld)
{
  char s[100], *text = 
    StringGet(((XmSelectionBoxCallbackStruct*)calld)->value);

  if (text && *text) {
    XDrawString(display, draw_window, draw_gc, 100, 200, text, strlen(text));

    XFlush(display);		/* forcer affichage immediat */
    XtUnmanageChild(dialog);
    free(text);
  }
}




/* ==================================================== ======== ======= */

static void SelectRect(Widget dialog, XtPointer drawing, XtPointer call_data)
{
  char s[100], *size_string = 
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);

  if (size_string) {
    int n, x, y, w, h;
    if ((n = sscanf(size_string, "%d %d %d %d", &x, &y, &w, &h)) == 4) {
      XDrawRectangle(display, draw_window, draw_gc, x, y, w, h);
      sprintf(s, "Rectangle = size: %dx%d at: %d,%d", w, h, x, y);
      Message(s);
      XtUnmanageChild(dialog);		/* enlever le dialog */
    }
    else Message("Dimensions incorrectes");
  }
}

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */
/* Rafraichissements du drawing */

static void Refresh(Widget drawing, XtPointer clientd, XtPointer calld)
{
  static no = 0;
  int j=0;

  XExposeEvent *xev =
    &(((XmDrawingAreaCallbackStruct*)calld)->event->xexpose);

  if (xev->count == 0)
    {

      fprintf(stderr, "Refresh du drawing no %d \n", no++);
  
      for(j=0;j<=nbr_curves-1;j++)
	{
	  if(ecran[j])
	    {
	      ChangeColor(display, draw_gc, Color[j]);
	      afficher(j);
	    }
	}
      ChangeColor(display, draw_gc, Color[nbr_curves]);
      AfficherAxes();
   
  }

}
/*==================================================== [ NR ] ==========*/

static void NormalZoom(void)
{
  ZoomChoice=1;
  Delta_x=0;
  Delta_y=0;
  NewScreen();
  Message("Zoom *1 centre en (0,0)");
}



/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */
/* Creation de l'Interface */

main(int argc, char *argv[])
{
  Widget 
    toplevel, mainbox, menubar, textfield, scrollarea,/*scrollarea2,*/drawing,
    read_dialog, save_dialog, color_dialog, size_dialog, rect_dialog,
    text_dialog, exit_dialog, scale_dialog,affimem_dialog,zoom_dialog,pulldown

    /*mainscale*/;
  XtAppContext app;
  Arg args[10];
  int i;

  /* ************************************************ Initialisation *** */

  toplevel = XtVaAppInitialize(&app, "XCurves", NULL, 0, &argc, argv, 
			       fallbacks, 
			       XmNallowShellResize, True,
			       NULL);
  display = XtDisplay(toplevel);

  /* ******************************************* creation des Managers *** */

  mainbox = XmCreateMainWindow(toplevel, "mainbox", NULL, 0);
  XtManageChild(mainbox);

  menubar = XmCreateMenuBar(mainbox, "menubar", NULL, 0);
  XtManageChild(menubar);

 /* mainscale=XmCreateScale(menubar,"mainscale",NULL,0);*/
 /* XtAddCallback(mainscale, XmNexposeCallback, choicemainscale, NULL);*/
 /* XtManageChild(mainscale);*/

  i = SetArgs(args, XtNumber(args),
	      /* ! ressource a specifier AVANT creation du widget */
	      XmNscrollingPolicy,        XmAUTOMATIC, 
	      XmNscrollBarDisplayPolicy, XmAS_NEEDED,
	      NULL);

 
 

  scrollarea = XmCreateScrolledWindow(mainbox, "scrollarea", args, i);
  XtManageChild(scrollarea);

  drawing = XmCreateDrawingArea(scrollarea, "drawing", NULL, 0);
  XtAddCallback(drawing, XmNexposeCallback, Refresh, NULL);
  XtManageChild(drawing);







/*===============================================================*/



/*  scrollarea2 = XmCreateScrolledWindow(mainbox, "scrollarea2", args, i);
  XtManageChild(scrollarea2);*/

  textfield = XmCreateText(/*scrollarea2*/mainbox, "commande", NULL, 0);
  XtManageChild(textfield);

  message = XmCreateLabel(mainbox, "message", NULL, 0);
  XtManageChild(message);

  XtVaSetValues(mainbox,
		XmNmenuBar, menubar,
		XmNcommandWindow, textfield,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNworkWindow, scrollarea,
		/*XmNworkWindow,scrollarea2,*/
		XmNmessageWindow, message,
		XmNshowSeparator, True,
		NULL);

  /* ******************************************* creation des Dialogues *** */

  read_dialog = CreateDialog(mainbox, "read_dialog", 
			     XmCreateFileSelectionDialog,
			     ReadFile, (XtPointer)drawing);

  save_dialog = CreateDialog(mainbox, "save_dialog", XmCreatePromptDialog,
			     SaveFile, NULL);

  size_dialog = CreateDialog(mainbox, "size_dialog", XmCreatePromptDialog,
			     SelectSize, (XtPointer)drawing);

  color_dialog = CreateDialog(mainbox, "color_dialog", XmCreatePromptDialog,
			     SelectColor, NULL);

  text_dialog = CreateDialog(mainbox, "text_dialog", XmCreatePromptDialog,
			     SelectText, NULL);

  exit_dialog = CreateDialog(mainbox, "exit_dialog", XmCreateErrorDialog,
			     ExitProg, NULL);

  scale_dialog= CreateDialog(mainbox, "scale_dialog", XmCreatePromptDialog,
			     SelectScale, NULL);
  zoom_dialog= CreateDialog(mainbox, "zoom_dialog", XmCreatePromptDialog,
			     SelectZoom,NULL);
  
  affimem_dialog= CreateDialog(mainbox, "Call_Back_dialog", XmCreatePromptDialog,
			     AffiMem, NULL);
  

  /* ******************************************* Dialogues complexe *** */

  rect_dialog = CreateDialog(mainbox, "rect_dialog", XmCreatePromptDialog,
			     SelectRect, NULL);

  /* *********************************** creation des CascadeButtons *** */
  /*                                     et des PulldownMenus associes   */

  CreateMenu(menubar, "File",
	     "Read", OpenDialog, read_dialog, 
	    /* "Save", OpenDialog, save_dialog, */
	     "Check List", Bilan, NULL,
	     "Exit",  OpenDialog, exit_dialog, 
	     NULL);

  CreateMenu(menubar, "Graph",
	   /*  "DrawLine",  DrawLine, NULL,
	     "DrawRect",  DrawRect, NULL,*/
	     "Clear Screen",     Clear,    NULL,
	     "Delete Memory",  ToutVider, NULL,
	     "Call Back", OpenDialog, affimem_dialog,
	    /* "Rect",   OpenDialog, rect_dialog,
	     "Text",   OpenDialog, text_dialog,
	     "Size",   OpenDialog, size_dialog,*/
	     NULL);

  pulldown=CreateMenu(menubar, "Colors",

	   /*  "Red",    SetColor, "red",
	     "Blue",   SetColor, "blue",
	     "Green",  SetColor, "green",
	     "Orange", SetColor, "orange",
	     "Yellow", SetColor, "yellow",
	     "Black",  SetColor, "black",
	     "Purple", SetColor, "purple",
	     "Pink",   SetColor, "pink",
	     "HotPink",SetColor, "HotPink",
	     "Violet", SetColor, "violet",*/

	     "Custom", OpenDialog, color_dialog,
	     NULL);
	   
	CreateMenu(pulldown,"courbe0",          
	   
	   "Red",    SetColor0, "red",
	   "Blue",   SetColor0, "blue",
	   "Green",  SetColor0, "green",
	   "Orange", SetColor0, "orange",
	   "Yellow", SetColor0, "yellow",
	   "Black",  SetColor0, "black",
	   "Purple", SetColor0, "purple",
	   "Pink",   SetColor0, "pink",
	   "HotPink",SetColor0, "HotPink",
	   "Violet", SetColor0, "violet",
		   NULL);
	
          CreateMenu( pulldown,"Courbe 1",
	   
	   "Red",    SetColor1, "red",
	   "Blue",   SetColor1, "blue",
	   "Green",  SetColor1, "green",
	   "Orange", SetColor1, "orange",
	   "Yellow", SetColor1, "yellow",
	   "Black",  SetColor1, "black",
	   "Purple", SetColor1, "purple",
	   "Pink",   SetColor1, "pink",
	   "HotPink",SetColor1, "HotPink",
	   "Violet", SetColor1, "violet",
		     NULL);

          CreateMenu( pulldown,"Courbe 2",
	   
	   "Red",    SetColor2, "red",
	   "Blue",   SetColor2, "blue",
	   "Green",  SetColor2, "green",
	   "Orange", SetColor2, "orange",
	   "Yellow", SetColor2, "yellow",
	   "Black",  SetColor2, "black",
	   "Purple", SetColor2, "purple",
	   "Pink",   SetColor2, "pink",
	   "HotPink",SetColor2, "HotPink",
	   "Violet", SetColor2, "violet",
		     NULL);

          CreateMenu( pulldown,"Courbe 3",
	   
	   "Red",    SetColor3, "red",
	   "Blue",   SetColor3, "blue",
	   "Green",  SetColor3, "green",
	   "Orange", SetColor3, "orange",
	   "Yellow", SetColor3, "yellow",
	   "Black",  SetColor3, "black",
	   "Purple", SetColor3, "purple",
	   "Pink",   SetColor3, "pink",
	   "HotPink",SetColor3, "HotPink",
	   "Violet", SetColor3, "violet",
		     NULL);


          CreateMenu( pulldown,"Courbe 4",
	   
	   "Red",    SetColor4, "red",
	   "Blue",   SetColor4, "blue",
	   "Green",  SetColor4, "green",
	   "Orange", SetColor4, "orange",
	   "Yellow", SetColor4, "yellow",
	   "Black",  SetColor4, "black",
	   "Purple", SetColor4, "purple",
	   "Pink",   SetColor4, "pink",
	   "HotPink",SetColor4, "HotPink",
	   "Violet", SetColor4, "violet",
		     NULL);


/*======================ECHELLE===NR==================================*/


 CreateMenu(menubar, "Scale",
	    "Custom", OpenDialog,scale_dialog,
	    "Undo Zoom",NormalZoom,NULL,
	    "Zoom"  ,OpenDialog,zoom_dialog,
	    NULL);





  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);
				/* imperativement apres XtRealize ! */
  draw_window = XtWindow(drawing);


  /*
   * draw_gc = XDefaultGCOfScreen(XtScreen(drawing));
   */
  {
    unsigned long gcmask;
    XGCValues gcvalues;
    Display * dpy = XtDisplay(toplevel); 

    gcmask = GCFunction | GCForeground |GCPlaneMask;
    gcvalues.function = GXcopy;
    gcvalues.foreground = BlackPixel(dpy,DefaultScreen(dpy));
    gcvalues.plane_mask = AllPlanes;
    draw_gc = XCreateGC(dpy, XtWindow(toplevel), gcmask, &gcvalues);
  }
  XtAppMainLoop(app);
}








