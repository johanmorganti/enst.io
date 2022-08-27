#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _SYNTAXE_
#define _SYNTAXE_
#endif

#ifndef _TYPES_
#include "types.h"
#endif

/* ---------------------------------------------------------------------- */
/*   FONCTIONS CONCERNANT LA GRAMMAIRE CONCRETE DU LANGAGE                */
/*     - CONVERSION ABSTRAIT > CONCRET (AFFICHAGE)                        */
/*     - CONVERSION CONCRET > ABSTRAIT (ANALYSE SYNTAXIQUE)               */
/* ---------------------------------------------------------------------- */

void affiche_terme (termeptr t,envptr env);
termeptr analyse_terme (progptr p,int (*in)());
clauseptr analyse_clause (progptr p,int (*in)());
