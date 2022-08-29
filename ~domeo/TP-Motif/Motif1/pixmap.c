/* ********************************************************************* *
 *
 *  pixmap.c : creer un pixmap dans une fenetre
 *  [Jean-Philippe Cottin 1996].
 *
 * ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>

/* Nom du fichier contenant la Pixmap
 * !ATTENTION: le repertoire exact depend de l'installation
 */
#define NOM_FICHIER "/usr/local/X11/include/bitmaps/calculator"


int main(int argc, char *argv[])
{
  XtAppContext app;
  Widget toplevel, box, label1;
  Pixmap dessin;
  Pixel fg, bg;

  /* ************************************************ Initialisation *** */

  toplevel = XtAppInitialize(&app, "XMdemos", NULL, 0, &argc, argv, NULL,
			     NULL, 0);

  /* ****************************************** Creation des Widgets *** */

				                  /* creation du manager */
  box = XmCreateRowColumn(toplevel, "box", NULL, 0);
  XtManageChild(box);
				              /* creation du widget fils */

  /*  On prend les couleurs d'avant et d'arriere plan du 
   * "RowColumn" pour les utiliser lors de la creation du pixmap
   */
  XtVaGetValues(box,
        XmNforeground, &fg,
        XmNbackground, &bg,
        NULL);

  dessin = XmGetPixmap (XtScreen (box), NOM_FICHIER, fg, bg);
  if (dessin == XmUNSPECIFIED_PIXMAP)
    fprintf(stderr, "Je ne peux pas charger %s\n", NOM_FICHIER);
  else{
    label1 = XmCreateLabel(box,"label1", NULL,0);
    
    XtVaSetValues(label1, 
		  XmNlabelType, XmPIXMAP,
		  XmNlabelPixmap, dessin,
		  NULL);
  }

  XtManageChild(label1);

  /* ************************************** Realisation et Lancement *** */

  XtRealizeWidget(toplevel);                  /* Realisation des widgets */
  XtAppMainLoop(app);	     /* boucle infinie de gestion des evenements */
}
