/**************************************************************************

         gc.c: "gros calcul"
	 Bertrand Dupouy && Eric Lecolinet
	 Juin 1996

***************************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <Xm/XmAll.h>
#include "disp.h"

/* ======================================================================== */
/* ======================================================================== */

void gros_calcul(int maxi, int maxj, int maxk)
{
  int i,j,k;
  double x = 555., y = 777., z = 0.243234;

  fprintf(stderr, "> Debut Gros Calcul\n");
  AfficheMessage( "> Debut Gros Calcul");

  for (i = 0; i < maxi; i++) {
    fprintf(stderr, "... toujours dans Gros Calcul: iteration %d\n", i);
    AfficheMessage("... toujours dans Gros Calcul");
    for (j = 0; j < maxj; j++)
      for (k = 0; k < maxk; k++) {
	x = x * x / x;
	y = y * y / y;
	z = z * z / z;
      }
  }

  fprintf(stderr, "< Fin Gros Calcul: x = %f, y = %f, z = %f\n", x, y, z);
  AfficheMessage( "< Fin Gros Calcul");
}

/* ======================================================================== */
/* ======================================================================== */

int main(int argc, char *argv[])
{
  XtAppContext app_context;
  extern String fallbacks[];

  /* a mettre en premier car enleve les arguments X de argc, argv */
  Widget toplevel = XtAppInitialize(&app_context, "XGrosCalc", NULL, 0, 
				    &argc, argv, fallbacks, NULL, 0);
  CreerInterface(toplevel);

  /* premier message affiche */
  AfficheMessage("Pret");

  /* lancement de la boucle d'evenements X */
  XtAppMainLoop(app_context);
}

/* ======================================================================== */
