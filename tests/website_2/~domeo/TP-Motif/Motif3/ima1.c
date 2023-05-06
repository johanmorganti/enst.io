/* ********************************************************************* *
 *
 *  ima1.c : 
 *  affiche la position du pointeur quand on bouge la souris
 *  en maintenant le bouton1 enfonce
 *
 *  [E. Lecolinet / L. Robert - ENST - 1997]
 *
 * ********************************************************************* */


#include <stdio.h>
#include <Xm/XmAll.h>

/* ==================================================== ======== ======= */
/* structure prd (program data) : 
 * contient donnees necessaires aux differentes fonctions
 */

static struct {
  Widget 
    position,		/* widget qui affichera la position de la souris */
    message,
    drawing;
} prd;

/* ==================================================== [Elc:97] ======= */
/* ==================================================== ======== ======= */
/* Definition des Actions */

/* ==================================================== ======== ======= */
/* Action pour afficher la postion du pointeur */

static void ShowPos(Widget drawing, XEvent *event,
		     String *params, Cardinal *num_params)
{
  char s[100];
  XmString xms;
  int x, y;

   /* a t'on appuye sur le bouton de la souris ou bouge la souris ? */
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
  "*drawing.translations: <Btn1Down>: ShowPos()\\n\
<Btn1Motion>: ShowPos()",
  
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
  };

  toplevel = XtAppInitialize(&app_context, "XIma", NULL, 0, &argc, argv, 
			     fallbacks, argtop, XtNumber(argtop));

  XtAppAddActions(app_context, actions, XtNumber(actions));

  CreateDrawing(toplevel);

  XtRealizeWidget(toplevel);
  XtAppMainLoop(app_context);
  return 0;
}

/* ==================================================== [TheEnd] ======= */
/* ==================================================== [Elc:97] ======= */



