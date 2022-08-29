#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _BASE_
#define _BASE_
#endif

#ifndef _TYPES_
#include "types.h"
#endif

/* ---------------------------------------------------------------------- */
/*   FONCTIONS D'ALLOCATION MEMOIRE POUR LES STRUCTURES DE DONNEES :      */
/*     - CREATION DE LISTES, DE MOTS, DE CLAUSES                          */
/*     - GESTION DES PILES POUR LA RESOLUTION                             */
/* ---------------------------------------------------------------------- */

listeptr ajoute_element (listeptr *l);
motptr ajoute_mot (progptr p,char *m);
listeptr ajoute_clause (progptr p,clauseptr c);

pileptr empile_but (void);
envptr empile_env (integer taille);
boolean empile_inst (instptr inst);
void depile_inst (integer stop);

boolean est_instanciee (envptr env,integer v,instptr *inst);
boolean instancie (envptr env,integer v,termeptr instt,envptr inste);
void constructeur (termeptr *t,envptr *env);
