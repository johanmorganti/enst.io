#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _TYPES_
#include "types.h"
#endif

#ifndef _GC_
#include "gc.h"
#endif

#ifndef _BASE_
#include "base.h"
#endif

/* ---------------------------------------------------------------------- */
/*   FONCTIONS CONCERNANT LA GRAMMAIRE CONCRETE DU LANGAGE                */
/*     - CONVERSION ABSTRAIT > CONCRET (AFFICHAGE)                        */
/*     - CONVERSION CONCRET > ABSTRAIT (ANALYSE SYNTAXIQUE)               */
/* ---------------------------------------------------------------------- */

/* GRAMMAIRE DU LANGAGE : */

/* CLAUSE = ':-' TERME QUEUE            */
/*        | TERME_ID '.'                */
/*        | TERME_ID ':-' TERME QUEUE   */

/* QUEUE = '.'                          */
/*       | ',' TERME QUEUE              */

/* TERME = nombre                       */
/*       | variable                     */
/*       | variable '(' TERME ARGS      */
/*       | atome                        */
/*       | atome '(' TERME ARGS         */
/*       | '[' LISTE                    */

/* TERME_ID = idem sans la ligne 3      */

/* ARGS = ')'                           */
/*      | ',' TERME ARGS                */

/* LISTE = ']'                          */
/*       | TERME SUITE                  */

/* SUITE = ']'                          */
/*       | '|' TERME ']'                */
/*       | ',' TERME SUITE              */

/* ---------------------------------------------------------------------- */
/*  AFFICHAGE :                                                           */
/*    3 FONCTIONS MUTUELLEMENT RECURSIVES, UNE POUR CHAQUE REGLE          */
/*    DE LA GRAMMAIRE (ON SE LIMITE AUX TERMES)                           */
/* ---------------------------------------------------------------------- */

void affiche_terme (termeptr t,envptr env);
void affiche_args (listeptr l,envptr env);
void affiche_suite (termeptr t,envptr env);

void affiche_terme (termeptr t,envptr env)

{
  instptr inst;

  if (t->ttyp > 0) {
    if ((env != NULL) && est_instanciee(env,t->ttyp,&inst))
      affiche_terme(inst->tmod,inst->tenv);
    else
      printf(t->tcons.mot->mval);
      
    if (t->targs != NULL) {
      printf("(");
      affiche_terme(t->targs->elem.terme,env);
      affiche_args(t->targs->suiv,env);
    }  
  } else if (t->ttyp == typnum)
    printf("%d",t->tcons.val);
  else if (t->tcons.mot->mval[0] == listevide)
    printf("[]");
  else if (t->tcons.mot->mval[0] == listecons) {
    printf("[");
    affiche_terme(t->targs->elem.terme,env);
    affiche_suite(t->targs->suiv->elem.terme,env);
  } else {
    printf(t->tcons.mot->mval);
    if (t->targs != NULL) {
      printf("(");
      affiche_terme(t->targs->elem.terme,env);
      affiche_args(t->targs->suiv,env);
    }
  }
}

void affiche_args (listeptr l,envptr env)

{
  if (l == NULL)
    printf(")");
  else {
    printf(",");
    affiche_terme(l->elem.terme,env);
    affiche_args(l->suiv,env);
  }
}

void affiche_suite (termeptr t,envptr env)

{
  instptr inst;
  
  if ((t->ttyp > 0) && (env != NULL) && est_instanciee(env,t->ttyp,&inst))
    affiche_suite(inst->tmod,inst->tenv);
  else if (t->tcons.mot->mval[0] == listevide)
    printf("]");
  else if (t->tcons.mot->mval[0] == listecons) {
    printf(",");
    affiche_terme(t->targs->elem.terme,env);
    affiche_suite(t->targs->suiv->elem.terme,env);
  } else {
    printf("|");
    affiche_terme(t,env);
    printf("]");
  }
}

/* ---------------------------------------------------------------------- */
/*  ANALYSE LEXICALE :                                                    */
/*    4 FONCTIONS POUR RECONNAITRE LES QUATRE TYPES DE LEXEMES :          */
/*      NOMBRE, ATOME, VARIABLE ET SEPARATEUR                             */
/* ---------------------------------------------------------------------- */

#define lire() c = (*read)()

#define tnombre   1                   /* type des symboles */
#define tatome    2
#define tvariable 3
#define tautre    4

typedef struct {                      /* un symbole */
  mot smot;
  integer styp;
} symbole;

int (*read)();                        /* fonction de lecture */

integer c;                            /* caractere en cours de lecture */
symbole s;                            /* lexeme en cours de lecture */
integer nb;                           /* nombre de variables reconnues */

boolean erreur (char *msg)

{ 
  printf("ERREUR : %s\n",msg);
  return false;
}

void lire_commentaire (void)

{
  do
    lire();
  while (c != '/');

  lire();
}

boolean lire_nombre (void)

{
  integer l = 0;
  boolean fin = false;
  
  s.smot[l++] = c;

  do {
    lire();
    if ((c >= '0') && (c <= '9'))
      if (l < maxcar - 1)
        s.smot[l++] = c;
      else
        return erreur("nombre trop grand");
    else
      fin = true;
  } while (!fin);
    
  s.smot[l] = 0;
  s.styp = tnombre;
  return true;
}

boolean lire_atome (void)

{
  integer l = 0;
  boolean fin = false;
  
  if (c == '!') {
    strcpy(s.smot,"!");
    s.styp = tatome;
    lire();
    
    return true;
  } else if (c == '\'') {
    do {
      lire();
      if (c != '\'')
        if (l < maxcar - 1)
          s.smot[l++] = c;
        else
          return erreur("identificateur trop long");
      else
        fin = true;
    } while (!fin);
    
    s.smot[l] = 0;
    s.styp = tatome;
    lire();
    
    return true;
  } else {
    s.smot[l++] = c;
    
    do {
      lire();
      if (((c >= 'a') && (c <= 'z')) || 
          ((c >= 'A') && (c <= 'Z')) || 
          ((c >= '0') && (c <= '9')) || (c == '_'))
        if (l < maxcar - 1)
          s.smot[l++] = c;
        else
          return erreur("identificateur trop long");
      else
        fin = true;
    } while (!fin);
    
    s.smot[l] = 0;
    s.styp = tatome;
    return true;
  }
}

boolean lire_variable (void)

{
  integer l = 0;
  boolean fin = false;
  
  if (c == '_') {
    strcpy(s.smot,"_");
    s.styp = tvariable;
    lire();
    
    return true;      
  } else {
    s.smot[l++] = c;
    
    do {
      lire();
      if (((c >= 'a') && (c <= 'z')) || 
          ((c >= 'A') && (c <= 'Z')) || 
          ((c >= '0') && (c <= '9')) || (c == '_'))
        if (l < maxcar - 1)
          s.smot[l++] = c;
        else
          return erreur("variable trop longue");
      else
        fin = true;
    } while (!fin);
    
    s.smot[l] = 0;
    s.styp = tvariable;
    return true;
  }
}

boolean lire_separateur (void)

{
  if ((c == '(') || (c == ',') || (c == ')') || 
      (c == '[') || (c == ']') || (c == '|') || (c == '.')) {
    s.smot[0] = c;
    s.smot[1] = 0;
    s.styp = tautre;
    lire();
    
    return true;
  } else if (c == ':') {
    lire();
    if (c == '-') {
      strcpy(s.smot,":-");
      s.styp = tautre;
      lire();
      
      return true;  
    } else 
      return erreur("- attendu");;
  } else
    return erreur("separateur attendu");;
}

boolean lire_lexeme (void)

{
  while ((c == ' ') || (c == '\t') || (c == '\n') || (c == '/'))
    if (c == '/')
      lire_commentaire();
    else
      lire();
  
  if (c == EOF)
    return false;
  else if ((c == '-') || ((c >= '0') && (c <= '9')))
    return lire_nombre();
  else if ((c == '\'') || (c == '!') || ((c >= 'a') && (c <= 'z')))
    return lire_atome();
  else if ((c == '_') || ((c >= 'A') && (c <= 'Z')))
    return lire_variable();
  else
    return lire_separateur();
}

/* ---------------------------------------------------------------------- */
/*  ANALYSE SYNTAXIQUE :                                                  */
/*    6 FONCTIONS, UNE PAR REGLE DE LA GRAMMAIRE, ANALYSENT UNE SUITE DE  */
/*    LEXEMES ET CONSTRUISENT SIMULTANEMENT L'ARBRE DE SYNTAXE ABSTRAITE  */
/* ---------------------------------------------------------------------- */

integer ajoute_variable (progptr p,mot m,listeptr *v,motptr *n)

{
  listeptr l;
  integer i = 1;
 
  l = *v;
  while ((l != NULL) && ((strcmp(l->elem.mot->mval,m) != 0) || (strcmp(m,"_") == 0))) {
    l = l->suiv;
    ++i;
  }
  
  if (l == NULL) {
    if ((*n = ajoute_mot(p,m)) == NULL)
      return 0;
    if ((l = ajoute_element(v)) == NULL)
      return 0;
      
    l->elem.mot = *n;
    ++nb;
  } else
    *n = l->elem.mot;
    
  return i;
}

termeptr lire_terme (progptr p,listeptr *v,termeptr *pt,boolean terme_id);
termeptr lire_liste (progptr p,listeptr *v,termeptr *pt);
termeptr lire_suite (progptr p,listeptr *v,termeptr *pt);
boolean lire_args  (progptr p,listeptr *v,listeptr *args);

termeptr lire_terme (progptr p,listeptr *v,termeptr *pt,boolean terme_id)

{
  termeptr t;
  listeptr l;
  
  if (!lire_lexeme())
    return NULL;

  if ((*pt = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
    return NULL;
  
  t = *pt;
  t->tcons.mot = NULL;
  t->targs = NULL;

  if (s.styp == tnombre) {
    t->ttyp = typnum;
    t->tcons.val = (integer)atoi(s.smot);
    return t;
  } else if (s.styp == tvariable) {
    if ((t->ttyp = ajoute_variable(p,s.smot,v,&(t->tcons.mot))) == 0)
      return NULL;
    if (!terme_id && (c == '(')) {
      if (!lire_lexeme())
        return NULL;
      if ((l = ajoute_element(&(t->targs))) == NULL)
        return NULL;
      if (lire_terme(p,v,&(l->elem.terme),false) == NULL)
        return NULL;
      if (!lire_args(p,v,&(t->targs)))
        return NULL;
    }
    return t;
  } else if (s.styp == tatome) {
    t->ttyp = typmot;
    if ((t->tcons.mot = ajoute_mot(p,s.smot)) == NULL)
      return NULL;
    if (c == '(') {
      if (!lire_lexeme())
        return NULL;
      if ((l = ajoute_element(&(t->targs))) == NULL)
        return NULL;
      if (lire_terme(p,v,&(l->elem.terme),false) == NULL)
        return NULL;
      if (!lire_args(p,v,&(t->targs)))
        return NULL;
    }
    return t;
  } else if (s.smot[0] == '[')
    return lire_liste(p,v,pt);
  else {
    erreur("nombre,atome ou variable attendu");
    return NULL;
  }
}

termeptr lire_liste (progptr p,listeptr *v,termeptr *pt)

{
  termeptr t;
  listeptr l;

  t = *pt;
  t->ttyp = typmot;
  
  if (c == ']') {
    if (!lire_lexeme())
      return NULL;
    if ((t->tcons.mot = ajoute_mot(p,motvide)) == NULL)
      return NULL;
  } else {
    if ((t->tcons.mot = ajoute_mot(p,motcons)) == NULL)
      return NULL;
    if ((l = ajoute_element(&(t->targs))) == NULL)
      return NULL;
    if (lire_terme(p,v,&(l->elem.terme),false) == NULL)
      return NULL;
    if ((l = ajoute_element(&(t->targs))) == NULL)
      return NULL;
    if (lire_suite(p,v,&(l->elem.terme)) == NULL)
      return NULL;
  }
  return t;
}

termeptr lire_suite (progptr p,listeptr *v,termeptr *pt)

{
  listeptr l;

  if (!lire_lexeme())
    return NULL;

  if (s.styp != tautre) {
    erreur("],| ou , attendu");
    return NULL;
  }
   
  if (s.smot[0] != '|') {
    if ((*pt = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
      return NULL;
    (*pt)->ttyp = typmot;
    (*pt)->tcons.mot = NULL;
    (*pt)->targs = NULL;
  }
  
  if (s.smot[0] == ']') {
    if (((*pt)->tcons.mot = ajoute_mot(p,motvide)) == NULL)
      return NULL;
    return *pt;
  } else if (s.smot[0] == '|') {
    if (lire_terme(p,v,pt,false) == NULL)
      return NULL;
    if (!lire_lexeme())
      return NULL;
    if ((s.styp != tautre) || (s.smot[0] != ']')) {
      erreur("] attendu");
      return NULL;
    }
    return *pt;
  } else if (s.smot[0] == ',') {
    if (((*pt)->tcons.mot = ajoute_mot(p,motcons)) == NULL)
      return NULL;
    if ((l = ajoute_element(&((*pt)->targs))) == NULL)
      return NULL;
    if (lire_terme(p,v,&(l->elem.terme),false) == NULL)
      return NULL;
    if ((l = ajoute_element(&((*pt)->targs))) == NULL)
      return NULL;
    if (lire_suite(p,v,&(l->elem.terme)) == NULL)
      return NULL;
    return *pt;
  } else {
    erreur("],| ou , attendu");
    return NULL;
  }
}

boolean lire_args  (progptr p,listeptr *v,listeptr *args)

{
  termeptr t;
  listeptr l;
  
  if (!lire_lexeme())
    return false;
    
  if (s.styp != tautre)
    return erreur(", ou ) attendu");
    
  if (s.smot[0] == ')')
    return true;
  else if (s.smot[0] == ',') {
    if ((l = ajoute_element(args)) == NULL)
      return false;
    if (lire_terme(p,v,&(l->elem.terme),false) == NULL)
      return false;
    if (!lire_args(p,v,args))
      return false;
    return true;
  } else
    return erreur(", ou ) attendu");
}

boolean lire_queue (progptr p,listeptr *v,listeptr *queue)

{
  listeptr l;
  
  if (!lire_lexeme())
    return false;
  if (s.styp != tautre)
    return erreur(", ou . attendu");
    
  if (s.smot[0] == '.')
    return true;
  else if (s.smot[0] == ',') {
    if ((l = ajoute_element(queue)) == NULL)
      return false;
    if (lire_terme(p,v,&(l->elem.terme),false) == NULL)
      return false;
    if (!lire_queue(p,v,queue))
      return false;
    return true;
  } else
    return erreur(", ou . attendu");  
}

clauseptr lire_clause (progptr p)

{  
  listeptr l;
  
  if ((clause_lue = (clauseptr)gc_malloc(sizeof(clauserec))) == NULL)
    return NULL;

  clause_lue->ctete = NULL;
  clause_lue->cqueue = NULL;
  clause_lue->cvariables = NULL;
  
  if (c == ':') {
    if (!lire_lexeme())
      return NULL;
    if ((l = ajoute_element(&(clause_lue->cqueue))) == NULL)
      return NULL;    
    if (lire_terme(p,&(clause_lue->cvariables),&(l->elem.terme),false) == NULL)
      return NULL;
    if (!lire_queue(p,&(clause_lue->cvariables),&(clause_lue->cqueue)))
      return NULL;
    
    clause_lue->cnb = nb;
    return clause_lue;
  } else {
    if (lire_terme(p,&(clause_lue->cvariables),&(clause_lue->ctete),true) == NULL)
      return NULL;
    if (!lire_lexeme())
      return NULL;
    if (s.styp != tautre) {
      erreur(". ou :- attendu");
      return NULL;
    }
    if (s.smot[0] == '.') {
      clause_lue->cnb = nb;
      return clause_lue;
    }  else if (s.smot[0] == ':') {
      if ((l = ajoute_element(&(clause_lue->cqueue))) == NULL)
        return NULL;    
      if (lire_terme(p,&(clause_lue->cvariables),&(l->elem.terme),false) == NULL)
        return NULL;
      if (!lire_queue(p,&(clause_lue->cvariables),&(clause_lue->cqueue)))
        return NULL;
        
      clause_lue->cnb = nb;
      return clause_lue;
    } else {
      erreur(". ou :- attendu");
      return NULL;
    }
  }
}

/* ---------------------------------------------------------------------- */
/*  FONCTIONS PUBLIQUES :                                                 */
/* ---------------------------------------------------------------------- */

termeptr analyse_terme (progptr p,int (*in)())

{
  listeptr vars = NULL;
    
  read = in;
  nb = 0;
  lire();
  
  return lire_terme(p,&vars,&terme_lu,true);
}

clauseptr analyse_clause (progptr p,int (*in)())

{
  read = in;
  nb = 0;
  lire();
  
  return lire_clause(p);  
}
