/* ********************************************************************* *
 *
 *  ima2.c : comme ima1.c + affichage graphique :
 *           - bouton 1 : dessin a main levee
 *           - bouton 2 : affiche la position du pointeur
 *           - bouton 3 : efface tout
 *
 *  [Eric Lecolinet - ENST - 1997]
 *
 * ********************************************************************* */


#include <stdio.h>
#include <Xm/XmAll.h>

/* epaisseur de la trace */
#define TRACE_WIDTH	0

/* couleur de la trace */
#define TRACE_COLOR	"blue"

/* ==================================================== ======== ======= */
/* structure prd (program data) : 
 * contient donnees necessaires aux differentes fonctions
 */

static struct {
  Widget 
    position,		/* widget qui affichera la position de la souris */
    message,
    drawing;
  int lastx, lasty;
  GC gc_trace;
} prd;

/* ==================================================== [Elc:97] ======= */
/* ==================================================== ======== ======= */
/* Definition des Actions */

/* ==================================================== ======== ======= */
/* Action pour afficher la position de la souris dans le widget "Position" */

static void ShowPos(Widget drawing, XEvent *event,
		     String *params, Cardinal *num_params)
{
  char s[100];
  XmString xms;
  int x, y;

  /* a-t'on appuye sur le bouton de la souris ou bouge la souris ? */
  if (event->type == MotionNotify)
    x = event->xmotion.x,  y = event->xmotion.y;
  else if (event->type == ButtonPress)
    x = event->xbutton.x,  y = event->xbutton.y;
  else return;

  /* afficher la position (et convertir en XmString) */
  sprintf (s, " %03d %03d ", x, y);
  xms = XmStringCreateLocalized(s);
  XtVaSetValues (prd.position, XmNlabelString, xms, NULL);
  XmStringFree(xms);
}

/* ==================================================== ======== ======= */
/* Action pour tracer a main levee dans le DrawingArea */

static void StartTrace(Widget drawing, XEvent *event,
		       String *params, Cardinal *num_params)
{
  prd.lastx = event->xbutton.x,
  prd.lasty = event->xbutton.y;
}


static void Trace(Widget drawing, XEvent *event,
		  String *params, Cardinal *num_params)
{
  int  x = event->xmotion.x,  y = event->xmotion.y;

  XDrawLine(XtDisplay(drawing), XtWindow(drawing), prd.gc_trace, 
	    prd.lastx, prd.lasty, x, y);
				
  XFlush(XtDisplay(drawing));    /* => affichage immediat du graphique */

  prd.lastx = x,  prd.lasty = y;
}


static void Clear(Widget drawing, XEvent *event,
		  String *params, Cardinal *num_params)
{
  XClearWindow(XtDisplay(drawing), XtWindow(drawing));
}

/* ==================================================== ======== ======= */
/* Fonction utile qui affiche le Widget a l'ecran et renvoie ce Widget */

Widget Show(Widget w)
{
  XtManageChild(w);
  return(w);
}

/* ==================================================== ======== ======= */
/* Fonction qui cree l'interface graphique */

void CreateDrawing(Widget parent)
{
  Widget form = Show(XmCreateForm(parent, "mainbox", NULL, 0));

  /* Creation de la boite a info (position + message) */
  Arg args1[] = {{XmNorientation, XmHORIZONTAL}};
  Widget infobox = Show(XmCreateRowColumn(form, "infobox", 
					  args1, XtNumber(args1)));

  /* Label pour afficher la position */
  Widget position = prd.position = 
    Show(XmCreateLabel(infobox, "position", NULL, 0));

  /* Label pour afficher un eventuel message */
  Widget message = prd.message =
    Show(XmCreateLabel(infobox, "message", NULL, 0));

  /* Creation de la zone d'affichage graphique */
  Arg args3[] = {
    /* gestion automatique des affichages qunad on bouge les ascenseurs */
    {XmNscrollingPolicy, XmAUTOMATIC}, 
    /* scrollbars presents seulement si c'est necessaire */
    {XmNscrollBarDisplayPolicy, XmAS_NEEDED},
    /*{XmNvisualPolicy, XmVARIABLE},*/
  };

  /* ScrolledWindow (fenetre avec ascenseur) contenant la zone d'affichage */
  Widget scrollw = Show(XmCreateScrolledWindow(form, "scrollwin", 
					args3, XtNumber(args3)));

  /* DrawingArea : widget dedie a l'affichage graphique */
  Widget drawing = prd.drawing =
    Show(XmCreateDrawingArea(scrollw, "drawing", NULL, 0));

  /* Specifier les attachements des enfants du Form Manager */
  XtVaSetValues(infobox,
		XmNtopAttachment,   (XtArgVal) XmATTACH_FORM,
		XmNleftAttachment,  (XtArgVal) XmATTACH_FORM,
		XmNrightAttachment, (XtArgVal) XmATTACH_FORM,
		NULL);

  XtVaSetValues(scrollw,
		XmNtopAttachment,   (XtArgVal) XmATTACH_WIDGET,
		XmNtopWidget,       (XtArgVal) infobox,
		XmNleftAttachment,  (XtArgVal) XmATTACH_FORM,
		/* attacher en bas et a droite pour permettre */
		/* d'agrandir la ScrolledWindow a la souris */
		XmNrightAttachment,  (XtArgVal) XmATTACH_FORM,
		XmNbottomAttachment, (XtArgVal) XmATTACH_FORM,
		NULL);
}

/* ==================================================== ======== ======= */
/* Fonction qui cree le Graphic Context qui servira a afficher le graphique */

GC CreateTraceGC(Widget drawing)
{
  Screen *scr = XtScreen(drawing);
  Display *disp = XtDisplay(drawing);
  Colormap cmap = DefaultColormapOfScreen(scr);
  XColor xcolor, unused;
  XGCValues gcval;

  /* imposer couleur noire en cas d'erreur */
  if (! XAllocNamedColor(disp, cmap, TRACE_COLOR, &xcolor, &unused))
    xcolor.pixel = BlackPixelOfScreen(scr);

  gcval.line_width = TRACE_WIDTH;
  gcval.foreground = xcolor.pixel;
  gcval.join_style = JoinRound;
  gcval.cap_style  = CapRound;

  return XCreateGC(disp, XtWindow(drawing), 
		   GCLineWidth | GCForeground | GCJoinStyle | GCCapStyle,
		   &gcval);
}

/* ==================================================== ======== ======= */
/* Fallbacks : definitions par defaut si pas de fichier de ressources */

String fallbacks[] = {
  "*keyboardFocusPolicy: EXPLICIT",    /*POINTER=>pas besoin de cliquer */
  "*background: lightsteelblue",
  "*foreground: black",
				/* taille intiale de la scrolledwin */
  "*scrollwin.width:  400",
  "*scrollwin.height: 300",
				/* taille intiale du drawing */
  "*drawing.width:  500",
  "*drawing.height: 400",
  "*drawing.background: white",

  "*position.background: black",
  "*position.foreground: white", 
  "*position.labelString: .000 000 ",

  "*message.labelString: Ready ...                                ",
  "*message.alignment: ALIGNMENT_BEGINNING",
  "*message.recomputeSize: True",

  /* translations pour gerer les mouvements de la souris dans le drawing */
  "*drawing.translations: <Btn1Down>: ShowPos() StartTrace()\\n\
<Btn1Motion>: ShowPos() Trace()\\n\
<Btn2Down>: ShowPos()\\n\
<Btn2Motion>: ShowPos()\\n\
<Btn3Down>: Clear()",
  
  NULL				/* !! a ne pas oublier !! */
};

/* ==================================================== [Elc:97] ======= */
/* ==================================================== ======== ======= */

int main(int argc, char *argv[])
{
  XtAppContext app_context;
  Widget toplevel;
  Arg argtop[] = {{XmNtitle, (XtArgVal)"xima -elc@enst.fr"}};

  XtActionsRec actions[] = {
    {"ShowPos",    ShowPos},
    {"Trace",      Trace},
    {"StartTrace", StartTrace},
    {"Clear",      Clear},
  };

  toplevel = XtAppInitialize(&app_context, "XIma", NULL, 0, &argc, argv, 
			     fallbacks, argtop, XtNumber(argtop));

  XtAppAddActions(app_context, actions, XtNumber(actions));

  CreateDrawing(toplevel);
  XtRealizeWidget(toplevel);

  /* Creer le Graphic Context qui servira a afficher le graphique
   * !!ATTENTION: doit etre cree APRES XtRealizeWidget
   * (car les Windows n'existent qu'apres l'appel a cette fonction)
   */
  prd.gc_trace = CreateTraceGC(prd.drawing);

  XtAppMainLoop(app_context);
  return 0;
}

/* ==================================================== [TheEnd] ======= */
/* ==================================================== [Elc:97] ======= */

