#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _PREDICATS_
#define _PREDICATS_
#endif

#ifndef _TYPES_
#include "types.h"
#endif

/* ---------------------------------------------------------------------- */
/*   IMPLEMENTATION DES PREDICATS PREDEFINIS                              */
/* ---------------------------------------------------------------------- */

void affiche_variables (listeptr noms,envptr env);

boolean nombre (pileptr p);
boolean atome (pileptr p);
boolean variable (pileptr p);

boolean plus (pileptr p);
boolean mult (pileptr p);
boolean supeg (pileptr p);

boolean lit (pileptr p);
boolean ecrit (pileptr p);
boolean new_line (pileptr p);
