/************************************************************************
 * PROGRAMME:  SuperMan
 * FICHIER: superman.c
 * AUTEUR:  Vincent Hascoet / Eric Lecilinet
 * DATE: 07/95   (remanie en Jan97 - Elc)
 ************************************************************************/

/************************************************************************
 * BUT   Creation d'une interface graphique (Motif) pour la gestion
 *       des Manual Pages sous Unix. L'avantage de cette application
 *       par rapport a celles existant est qu'elle ne demande pas
 *       la specification complete du nom de la page cherchee. 
 *       De plus il est tres facile d'acceder aux differentes sections.
 *       Enfin, il est possible de retrouver rapidement les pages deja
 *       consultees grace a un historique.
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <Xm/XmAll.h>
#include <X11/cursorfont.h>
#include "superfct.h"

#define MANPATH		"MANPATH"

/*************************************************************************
 *                         F A L L B A C K S                           
 *************************************************************************/

String fallbacks[] = {
  "*keyboardFocusPolicy: POINTER", /* Focus ou est la souris */

  /* Fonts */
  "*fontList: fixed",
  "*XmCascadeButton.fontList: -*-helvetica-bold-r-normal-*-12-*-*-*-*-*-*-*",
  "*XmPushButton.fontList: -*-helvetica-bold-r-normal-*-12-*-*-*-*-*-*-*",
  "*XmToggleButton.fontList: -*-helvetica-bold-r-normal-*-12-*-*-*-*-*-*-*",
  "*XmLabel.fontList: -*-helvetica-bold-o-normal-*-12-*-*-*-*-*-*-*",
  "*Search.fontList: -*-helvetica-bold-o-normal-*-12-*-*-*-*-*-*-*",
  "*Superman.fontList: -*-helvetica-bold-o-normal-*-12-*-*-*-*-*-*-*",
  "*left_joker.fontList: fixed",
  "*right_joker.fontList: fixed",

  /* Sizes */
  "SuperMan.width: 600",
  "SuperMan.height: 500",
  "*matchlist.width: 135",
  "*token.columns: 30",

  /* Colors */
  "*background: LightSteelBlue",/* default */
  "*token.background: White",
  "*mantext.background: White",
  "*matchlist.background: White",
  "*search_case.labelString: Case Sensitive",
  "*left_joker.foreground: red",
  "*right_joker.foreground: red",
  "*Superman.foreground: red",

  /* Options */
  "*left_joker.set: False",
  "*right_joker.set: False",
  "*search_case.set: True",

  /* Labels */
  "*Search.labelString: Search ManPages",
  "*search_case.labelString: Case Sensitive",
  "*left_joker.labelString: *",
  "*right_joker.labelString: *",

  /* scrollbars toujours presents */
  "*matchlist.scrollBarDisplayPolicy: STATIC",
  /* ne change pas de taille quand la liste change */
  "*matchlist.listSizePolicy: CONSTANT",
  /* activation au simple clic (au lieu de double clic: standard mais penible!) */
  "*matchlist.automaticSelection: True",
  "*matchlist.selectionPolicy: BROWSE_SELECT",
  "*token_box.spacing: 0",
  /* pas d'indicateur mais s'enclenche comme un PushButton (memes ombrages) */
  "*left_joker.indicatorOn: False",
  "*left_joker.shadowThickness: 2",
  "*right_joker.indicatorOn: False",
  "*right_joker.shadowThickness: 2",

  "help_text.labelString: Superman\\n\
-----------------------------------\\n\
\n\
Exact Search:\\n\
1. Enter token in the entry field (horizontal window at the bottom)\\n\
2. Press the Enter Key or Click on \"Search Manpages\"\\n\
3. Click once on 
\\n\
Inexact Search:\\n\
- Set the left \"*\" and/or the right \"*\" toggle buttons\\n\
- Or enter token with left and/or \"*\" characters\\n\
  (but without \"*\" characters inside the word)\\n\
- Then, same as above\\n\
\\n\
Note:\\n\
The 'Paste' button gets the selection from the current man page\\n\
and pastes it into the Token entry field\\n\
\\n\
Credits:\\n\
E. Lecolinet -V. Hascoet - ENST Paris - elc@enst.fr",

  NULL
};

/*************************************************************************
 *                         C A L L B A C K S : forward defs
 *************************************************************************/

static void BuildSearchListCB(Widget w, XtPointer, XtPointer);
static void DisplayListManpageCB(Widget w, XtPointer, XtPointer);
static void PrintManpageCB(Widget w, XtPointer, XtPointer);
static void ToggleStateCB(Widget w, XtPointer, XtPointer);
static void ClearCB(Widget w, XtPointer, XtPointer);
static void PasteCB(Widget w, XtPointer, XtPointer);
static void QuitCB(Widget w, XtPointer, XtPointer);

/*************************************************************************/
/* UTILITAIRES */
/*************************************************************************/

/* Fonction pour ajouter une fonction de callback. Le "callback_name"
 * Peut etre NULL dans certaisn cas, auquel cas un "callback_name"
 * dependant du widget est automatiquement choisi
 */
Widget AddCallback(Widget w, String callback_name, 
					XtCallbackProc callback, void *data)
{
  if (callback_name)
    XtAddCallback(w, callback_name, callback, data);
  
  else {
    WidgetClass wclass = XtClass(w);
  
    /* ajoute XmNactivateCallback si PushButton, etc */
    if  (wclass == xmPushButtonWidgetClass
	 || wclass == xmCascadeButtonWidgetClass
	 || wclass == xmArrowButtonWidgetClass
	 || wclass == xmDrawnButtonWidgetClass
	 /* !!ATT: TextField => Activate mais Text => ValueChanged */
	 || wclass == xmTextFieldWidgetClass)
      XtAddCallback(w, XmNactivateCallback, callback, data);

    /* ajoute XmNvalueChangedCallback si ToggleButton, etc */
    else if  (wclass == xmToggleButtonWidgetClass
	      ||wclass == xmTextWidgetClass
	      ||wclass == xmScaleWidgetClass)
      XtAddCallback(w, XmNvalueChangedCallback, callback, data);
  }
  return w;
}

void HideCB(Widget w, XtPointer target_w, XtPointer call_data)
{
  XtUnmanageChild((Widget)target_w);
}
 
void ShowCB(Widget w, XtPointer target_w, XtPointer call_data)
{
  XtManageChild((Widget)target_w);
}

Widget Show(Widget w)
{
  XtManageChild(w);
  return w;
}

Widget ShowAndCall(Widget w, XtCallbackProc callback, void *data)
{
  AddCallback(w, NULL, callback, data);
  XtManageChild(w);
  return w;
}


void BusyCursor(Widget w, Boolean busy)
{
  Display *display;
  static cursor_defined = False;
  static Cursor busy_cursor;

  if (! w) return;
  display = XtDisplay(w);

  if (! cursor_defined) {
    cursor_defined = True;
    busy_cursor = XCreateFontCursor(display, XC_watch);
    /*ima_cursor = XCreateFontCursor(display, XC_crosshair);*/
  }
  if (busy) XDefineCursor(display, XtWindow(w), busy_cursor);
  else XUndefineCursor(display, XtWindow(w));
  XFlush(display);
}

/*
 * Ajoute la chaine str a la fin du XmText w.
 */
void TextPuts(Widget  w, char   *str)
{
    XmTextPosition last_pos;
    last_pos = XmTextGetLastPosition(w);
    XmTextInsert(w, last_pos, str);
}

/*************************************************************************/
/*                       F U N C T I O N S                      */
/*************************************************************************/

static Widget CreateMenus(Widget mainbox, Widget *histmenu)
{
  Widget menubar, cascade, menu;

  /* MenuBar */
  Show(menubar = XmCreateMenuBar(mainbox, "menubar", NULL, 0));

  /* File Menu */
  menu = XmCreatePulldownMenu(menubar, "file_menu", NULL, 0);
  Show(cascade = XmCreateCascadeButton(menubar, "File", NULL, 0));
  /* attacher le menu au bouton correspondant */
  XtVaSetValues(cascade, XmNsubMenuId, menu);

  /* Boutons du Menu */
  ShowAndCall(XmCreatePushButton(menu, "Print", NULL, 0),
	      PrintManpageCB, NULL);
  ShowAndCall(XmCreatePushButton(menu, "Quit",NULL, 0),
	      QuitCB, NULL);

  /* History Menu */
  *histmenu = menu = XmCreatePulldownMenu(menubar, "hist_menu", NULL, 0);
  Show(cascade = XmCreateCascadeButton(menubar, "History", NULL, 0));
  XtVaSetValues(cascade, XmNsubMenuId, menu);

  Show(cascade = XmCreateCascadeButton(menubar, "Superman", NULL, 0));
  /* HelpButton (a droite du menubar) */
  XtVaSetValues(menubar, XmNmenuHelpWidget, (XtPointer) cascade, NULL);

  return menubar;
}

/*************************************************************************/

static Widget CreateListBox(Widget mainbox, Widget *list)
{
  /* indique le titre d'un Frame */
  Arg  frametitle[] = {{XmNchildType, XmFRAME_TITLE_CHILD}};
  Widget listbox;

  /* Match List */
  Show(listbox = XmCreateFrame(mainbox, "listbox", NULL, 0));
  /* Titre du Frame */
  Show(XmCreateLabel(listbox, "ManPages", frametitle, XtNumber(frametitle)));

  /* Liste des ManPages */
  Show(*list =  XmCreateScrolledList(listbox, "matchlist", NULL, 0));
  /* Choix dans la liste (Activation, DbleClic, KEnter) */
  XtAddCallback(*list, XmNdefaultActionCallback, DisplayListManpageCB, NULL);
  /* Choix dans la liste (Selection, SimpleClic, KSpace) */
  XtAddCallback(*list, XmNsingleSelectionCallback, DisplayListManpageCB, NULL);
  XtAddCallback(*list, XmNbrowseSelectionCallback, DisplayListManpageCB, NULL);

  return listbox;
}

/*************************************************************************/

static Widget CreateSearchBox(Widget mainbox, Widget *search_entry)
{
  /* pour rendre RowColumns Horizontaux */
  Arg horiz[] = {{XmNorientation, (XtArgVal)XmHORIZONTAL}};
  Widget searchbox, frame, box, bw;

  /* Search */
  Show(searchbox = XmCreateRowColumn(mainbox, "searchbox", horiz, XtNumber(horiz)));

  /* Search ManPage Button */
  ShowAndCall(XmCreatePushButton(searchbox, "Search", NULL, 0),
	      BuildSearchListCB, NULL);

  /* Search Text entoure de 2 Boutons Jokeru le tout dans un Frame */
  Show(frame = XmCreateFrame(searchbox, "frame1", NULL, 0));
  Show(box = XmCreateRowColumn(frame, "token_box", horiz, XtNumber(horiz)));

  bw = ShowAndCall(XmCreateToggleButton(box, "left_joker", NULL, 0),
		   ToggleStateCB, "LEFT");
  /* Cet appel sert a initialiser le mode correspondant au Toggle selon
   * la valeur a laquelle il a ete initialise dans le fichier de resources
   */
  ToggleStateCB(bw, "LEFT", NULL);

  ShowAndCall(*search_entry = XmCreateTextField(box, "token", NULL, 0),
	      BuildSearchListCB, NULL);
  
  bw = ShowAndCall(XmCreateToggleButton(box, "right_joker", NULL, 0),
		   ToggleStateCB, "RIGHT");
  ToggleStateCB(bw, "RIGHT", NULL);

  /* Case Sensitive */
  Show(frame = XmCreateFrame(searchbox, "frame2", NULL, 0));
  Show(box = XmCreateRowColumn(frame, "com_box", horiz, XtNumber(horiz)));

  ShowAndCall(XmCreatePushButton(box, "Clear", NULL, 0),
	      ClearCB, NULL);
  
  ShowAndCall(XmCreatePushButton(box, "Paste", NULL, 0),
	      PasteCB, NULL);

  bw = ShowAndCall(XmCreateToggleButton(box, "search_case", NULL, 0),
		   ToggleStateCB, "CASE");
  ToggleStateCB(bw, "CASE", NULL);

  return searchbox;
}

/*************************************************************************/
/*                       M A I N   F U N C T I O N                       */
/*************************************************************************/

/* Structure contenant les donnees du Programme
 * cette structure peut etre librement utilisee par les fonctions de callback
 * incluses dans ce fichier
 */
static PrgmDataStruct prd;

main(int argc, char *argv[]) 
{
    Widget  mainbox, menubar, listbox, mantext;
    XtAppContext app;

    /*************************************************************************/
    /*                             W I D G E T S                             */
    /*************************************************************************/

    /* TopLevel & MainBox */

    /* Titre du programme (affiche sur la Decoration du WM */
    Arg title[] = {{XmNtitle, (XtArgVal)"SuperMan"}};
    prd.toplevelW = XtAppInitialize(&app, "SuperMan", NULL, 0, &argc, argv,
			       fallbacks, title, XtNumber(title));

    Show(mainbox = XmCreateForm(prd.toplevelW, "mainbox", NULL, 0));

    menubar = CreateMenus(mainbox, &(prd.histW));
    listbox = CreateListBox(mainbox, &(prd.listW));
    prd.searchboxW = CreateSearchBox(mainbox, &(prd.searchentryW));

    {				/* Manual Text */
      Arg args[] = {
	{XmNeditable, False},
	{XmNeditMode, XmMULTI_LINE_EDIT},
      };
      Show(mantext = XmCreateScrolledText(mainbox, "mantext", args, XtNumber(args)));
      prd.manW = mantext;
    }

    /*************************************************************************/
    /* Attachments */
    /*************************************************************************/
  
    XtVaSetValues(menubar,
		  XmNtopAttachment, XmATTACH_FORM,
		  XmNleftAttachment, XmATTACH_FORM,
		  XmNrightAttachment, XmATTACH_FORM,
		  NULL);
    XtVaSetValues(listbox,
		  XmNtopAttachment, XmATTACH_WIDGET,
		  XmNtopWidget, (XtArgVal)menubar,
		  XmNleftAttachment, XmATTACH_FORM,
		  XmNbottomAttachment, XmATTACH_WIDGET,
		  XmNbottomWidget, (XtArgVal)prd.searchboxW,
		  NULL);	

    /* !!ATTENTION: c'est le PARENT qu'il faut attacher 
     * (le Text est contenu dans une ScrolledWindow)
     */
    XtVaSetValues(XtParent(mantext),
		  XmNtopAttachment, XmATTACH_WIDGET,
		  XmNtopWidget, (XtArgVal)menubar,
		  XmNleftAttachment, XmATTACH_WIDGET,
		  XmNleftWidget, (XtArgVal)listbox,
		  XmNrightAttachment, XmATTACH_FORM,
		  XmNbottomAttachment, XmATTACH_WIDGET,
		  XmNbottomWidget, (XtArgVal)prd.searchboxW,
		  NULL);	
    XtVaSetValues(prd.searchboxW,
		  XmNbottomAttachment, XmATTACH_FORM,
		  XmNleftAttachment, XmATTACH_FORM,
		  XmNrightAttachment, XmATTACH_FORM,
		  NULL);

/*************************************************************************/
/*          I N I T I A L I S A T I O N   D E S   D O N N E E S          */
/*************************************************************************/
    {
      char buffer[BUFSIZ];
      int i, OverallEntryNb = 0;	/* Nombre de pages total */

      /*
       * Generation de l'arbre
       */
      int basecard = 0;
      char **basepaths = RetrieveManDirectories(MANPATH, &basecard);

      CreateNamesTree(&prd, basepaths, basecard);

      prd.entryNb = 0;		/* Nombre de pages differentes */

      /*
       * Affichage des stats MANPATH
       */
      for (i=0; i < basecard; i++) {
        OverallEntryNb += prd.stats[i];
        sprintf(buffer," %6d entries in %s\n", prd.stats[i], basepaths[i]);
	TextPuts(mantext,buffer);
      }

      sprintf(buffer," ------\n");
      TextPuts(mantext,buffer);
      sprintf(buffer," %6d entries in MANPATH\n", OverallEntryNb);
      TextPuts(mantext,buffer);
      sprintf(buffer," Number of distinct entries : %d (%d duplicates)\n",
	      prd.entryNb,
	      OverallEntryNb - prd.entryNb);
      TextPuts(mantext,buffer);

      /*
       * Initialisation des donnees
       */
      prd.overallEntryNb = OverallEntryNb;
      prd.matchNb = 0;
      prd.matchName = (char**)  calloc(1,sizeof(char*));
      prd.matchEntry  = (Entry**) calloc(1,sizeof(Entry*));
      prd.histNb = 0;
      prd.histName = (char**)  calloc(1,sizeof(char*));
      prd.histEntry = (Entry**) calloc(1,sizeof(Entry*));
      prd.histButton = (Widget*) calloc(1,sizeof(Widget));
      prd.leftStar = 0;
      prd.rightStar = 0;
      prd.caseSensitive = 1;
      prd.currentName = NULL;
      prd.currentEntry = NULL;
    }
/*************************************************************************/
/*          R E A L I S A T I O N   E T   L A N C E M E N T              */
/*************************************************************************/
    
    XtRealizeWidget(prd.toplevelW);
    XtAppMainLoop(app);
}

/*************************************************************************/
/*                         C A L L B A C K S                             */
/*************************************************************************/

/* NB:
 * Cette fct est appelee directement par le programme a l'initialisation
 * afin de tenir compte des options definies dans le fichier de resources
 * => ne pas utiliser call_data qui vaut alors NULL
 */

static void ToggleStateCB(Widget toggle, XtPointer p_cas, XtPointer calld)
{
  char *cas = (char*)p_cas;
  Boolean val;
  XtVaGetValues(toggle, XmNset, &val, NULL);

  if (!cas) return;
  else if (strcmp(cas, "LEFT")==0) prd.leftStar = val;
  else if (strcmp(cas, "RIGHT")==0) prd.rightStar = val;
  else if (strcmp(cas, "CASE")==0) prd.caseSensitive = val;
}

static void ClearCB(Widget w, XtPointer xxx, XtPointer zzz)
{
  XmTextFieldSetString(prd.searchentryW, "");
}


static void PasteCB(Widget w, XtPointer xxx, XtPointer zzz)
{
  XmTextFieldSetString(prd.searchentryW, XmTextGetSelection(prd.manW));
}


static void QuitCB(Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}

/*
 * Callback appele lors de la premiere selection dans le mantext. Il essaye
 * de determiner si la selection est un mot convenable puis effectue une
 * recherche exacte sur ce mot afin de mettre la liste a jour
 */

static void BuildSearchListCB(Widget w, XtPointer xxx, XtPointer zzz)
{
  char *pat, *pattern = XmTextGetString(prd.searchentryW);   
  int ll, 
    leftStar = prd.leftStar,	/* valeurs du Toggle */
    rightStar = prd.rightStar;

  /* verifier qu'il y a bien qq chose achercher */
  if (pattern && pattern[0]) {

    /* Retours Visuels: 
     * changer curseur souris et interdire de cliquer sur Search
     */
    BusyCursor(prd.toplevelW, True);
    XtSetSensitive(prd.searchboxW, False);
    /* mettre a jour l'affichage immediatement */
    XmUpdateDisplay(w);

    pat = pattern;
    /* cas ou on a entre les etoiles textuellement */
    if (pat[0] == '*') {
      pat++;
      leftStar = True;
    }

    ll = strlen(pat);
    if (pat[ll-1] == '*') {
      pat[ll-1] = '\0';
      rightStar = True;
    }

    /* construire et afficher la liste des matches */
    BuildList(&prd, pat, leftStar, rightStar);
    XtFree(pattern);

    /* si UN SEUL match afficher la page ET effacer le  searchentry  */
    if (prd.matchNb == 1) {
      DisplayManpage(&prd, prd.matchName[0], prd.matchEntry[0]);
      XtVaSetValues(prd.searchentryW, XmNvalue, "", NULL);
    }
    /* si aucun match effacer la page du man */
    else if (prd.matchNb == 0)
      XtVaSetValues(prd.manW, XmNvalue, "", NULL);

    /* annuler retours visuels */
    BusyCursor(prd.toplevelW, False);
    XtSetSensitive(prd.searchboxW, True);
  }
}


/*
 * Callback appele lors du choix d'un item dans la liste des pages.
 * Affiche la page correspondante
 */
static void DisplayListManpageCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmListCallbackStruct *lcs  = (XmListCallbackStruct*) call_data;

    /* Retours Visuels: changer curseur souris et interdire de cliquer sur Search */
    BusyCursor(prd.toplevelW, True);
    XtSetSensitive(prd.listW, False);
    /* mettre a jour l'affichage immediatement */
    XmUpdateDisplay(w);

    DisplayManpage(&prd, prd.matchName[lcs->item_position - 1],
		   prd.matchEntry[lcs->item_position - 1]);
    /* effacer le  searchentry  */
    XtVaSetValues(prd.searchentryW, XmNvalue, "", NULL);

    BusyCursor(prd.toplevelW, False);
    XtSetSensitive(prd.listW, True);
}

 
/*
 * Callback appele lors du choix d'une page dans l'historique.
 * Affiche la page en question.
 * NB: appel externe !
 */
void DisplayHistManpageCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    char
	*ButtonName = XtName(w),
	PageName[100];
    int i = 0;

    /* Retours Visuels: changer curseur souris et interdire de cliquer sur Search */
    BusyCursor(prd.toplevelW, True);
    XtSetSensitive(prd.listW, False);
    /* mettre a jour l'affichage immediatement */
    XmUpdateDisplay(w);

    do {
	sprintf(PageName,"%s(%s)", prd.histName[i], prd.histEntry[i]->suf);
	i++;
    }
    while(strcmp(ButtonName,PageName));
    DisplayManpage(&prd, prd.histName[i-1], prd.histEntry[i-1]);

    /* annuler retours visuels */
    BusyCursor(prd.toplevelW, False);
    XtSetSensitive(prd.listW, True);
}


/*
 * Callback appele lors de l'activation du bouton Print
 * Imprime le contenu du texte en landscape
 */
static void PrintManpageCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    char command[300];
    FILE *PipeIn;
    char *ptr, *TextString;
    
    TextString = XmTextGetString(prd.manW);

    if (prd.currentEntry==NULL)
	sprintf(command,"mp -lo -s \"MANPATH statistics\" | lpr");
    else
	sprintf(command,"mp -lo -s \"%s(%s)\" | lpr",
		prd.currentName,
		prd.currentEntry->suf);
	    
    PipeIn = popen(command,"w");
    for (ptr = TextString; ptr < TextString+strlen(TextString); ptr++)
	fputc(*ptr,PipeIn);
    fputc(EOF,PipeIn);
    pclose(PipeIn);
}


