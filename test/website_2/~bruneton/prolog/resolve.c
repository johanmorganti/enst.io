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

#ifndef _SYNTAXE_
#include "syntaxe.h"
#endif

#ifndef _PREDICATS_
#include "predicats.h"
#endif

/* ---------------------------------------------------------------------- */
/*   ALGORITHME DE RESOLUTION                                             */
/* ---------------------------------------------------------------------- */

pileptr exemplarise (clauseptr c)

{
  listeptr l;
  pileptr po,p,prec,suiv;
  integer taille;
  
  prec = NULL;
  if (pile == NULL)
    suiv = NULL;
  else
    suiv = pile->butsuiv;

  if ((po = empile_but()) == NULL)
    return NULL;
  if ((po->env = empile_env(c->cnb)) == NULL)
    return NULL;
  
  l = c->cqueue;
  
  if (l == NULL) {
    po->but = NULL;
    po->butprec = pile;
    po->butsuiv = suiv;
  } else 
    while (l != NULL) {
      if (l == c->cqueue) {
        p = po;
        p->butprec = pile;
      } else {
        if ((p = empile_but()) == NULL)
          return NULL;
        p->env = po->env;
        p->butprec = prec;
      }
      
      p->but = l->elem.terme;
      p->butsuiv = suiv;
      if (prec != NULL)
        prec->butsuiv = p;
        
      prec = p;
      l = l->suiv;
    }
    
  return po;
}

void trouve_candidats (void)

{
  paquetptr p;
  termeptr t;
  envptr e;
  
  t = pile->but;
  
  if (t == NULL)
    pile->candidats = (listeptr)1; /* quelconque mais <> NULL (sinon retour arriere) */
  else {
    e = pile->env;
    constructeur(&t,&e);
    p = t->tcons.mot->mpaquet;
    if (p != NULL)
      pile->candidats = p->pclauses;
    else
      pile->candidats = NULL;
  }
}

boolean unification (termeptr a,termeptr b,envptr enva,envptr envb)

{
  instptr inst;
  listeptr la,lb;
  boolean bon;
  termeptr ca;
  envptr cenva;
  
  if (a->targs == NULL)
    if (b->targs == NULL)
      if ((a->ttyp > 0) || (b->ttyp > 0) ||
          ((a->ttyp == typmot) && (b->ttyp == typmot) && (a->tcons.mot == b->tcons.mot)) || 
          ((a->ttyp == typnum) && (b->ttyp == typnum) && (a->tcons.val == b->tcons.val))) {
        if (a->ttyp > 0)
          if (est_instanciee(enva,a->ttyp,&inst))
            return unification(inst->tmod,b,inst->tenv,envb);
          else
            return instancie(enva,a->ttyp,b,envb);
        else if (b->ttyp > 0)
          if (est_instanciee(envb,b->ttyp,&inst))
            return unification(a,inst->tmod,enva,inst->tenv);
          else
            return instancie(envb,b->ttyp,a,enva);
        else
          return true;
      } else 
        return false;
    else if (a->ttyp > 0)
      if (est_instanciee(enva,a->ttyp,&inst))
        return unification(inst->tmod,b,inst->tenv,envb);
      else
        return instancie(enva,a->ttyp,b,envb);
    else
      return false;
  else if (b->targs == NULL)
    if (b->ttyp > 0)
      if (est_instanciee(envb,b->ttyp,&inst))
        return unification(a,inst->tmod,enva,inst->tenv);
      else
        return instancie(envb,b->ttyp,a,enva);
    else
      return false;
  else { 
    if (a->ttyp == typmot)
      if (b->ttyp == typmot)
        bon = (a->tcons.mot == b->tcons.mot);
      else
        bon = false;
    else
      if (b->ttyp == typmot) {
        ca = a;
        cenva = enva;
        constructeur(&ca,&cenva);
        bon = ((ca->ttyp == typmot) && (ca->targs == NULL) && (ca->tcons.mot == b->tcons.mot));
      } else
        bon = false;
      
    if (bon) {
      la = a->targs;
      lb = b->targs;
      
      while ((la != NULL) && (lb != NULL) && bon) {
        bon = unification(la->elem.terme,lb->elem.terme,enva,envb);
        la = la->suiv;
        lb = lb->suiv;
      }
    
      return (bon && (la == NULL) && (lb == NULL));
    } else
      return false;
  }
}

boolean recherches (progptr p,clauseptr c)

{
  boolean solutions = false,arret = false,retour;
  integer copie1,copie2,copie3;
  
  pileptr essai,temp;
  clauseptr d;
  termeptr t;
  envptr e;
 
  pile = NULL;
  if ((pile = exemplarise(c)) == NULL)
    return false;

  trouve_candidats();
  
  while (pile != NULL) {
    retour = true;
    copie1 = hautbutpile;
    copie2 = hautenvpile;
    copie3 = hautinstpile;
    
    while ((pile->candidats != NULL) && retour) {
      d = pile->candidats->elem.clause;
      
      if ((essai = exemplarise(d)) == NULL)
        return false;
      
      if (unification(pile->but,d->ctete,pile->env,essai->env))
        retour = false;
      else {
        if (outofmemory) 
          return false;

        hautbutpile = copie1;
        hautenvpile = copie2;
        depile_inst(copie3);
      }
      
      pile->candidats = pile->candidats->suiv;
    }
    
    if (!retour) {
      pile->rbut = copie1;
      pile->renv = copie2;
      pile->rinst = copie3;
     
      t = pile->but;
      e = pile->env;
      constructeur(&t,&e);

      if (t->tcons.mot->mval[0] == coupe) {
        temp = pile;

        if (pile->butprec->butsuiv != pile)
          pile->butprec->candidats = NULL;
        else
          while (temp != NULL) {
            temp->candidats = NULL;
            temp = temp->butprec;
            if ((temp != NULL) && (temp->env == pile->env) && 
              ((temp->butprec == NULL) || (temp->butprec->butsuiv != temp))) {
            if (temp->butprec != NULL)
              temp->butprec->candidats = NULL;
            temp->candidats = NULL;
            temp = NULL;
          }
        }
      }
      
      if (strcmp(t->tcons.mot->mval,"quit") == 0)
        exit(0);

      if (strcmp(t->tcons.mot->mval,"echec") == 0)
        retour = true;
 
      if (strcmp(t->tcons.mot->mval,"nombre") == 0)
        retour = !nombre(essai);
      if (strcmp(t->tcons.mot->mval,"atome") == 0)
        retour = !atome(essai);
      if (strcmp(t->tcons.mot->mval,"variable") == 0)
        retour = !variable(essai);
 
      if (strcmp(t->tcons.mot->mval,"plus") == 0)
        retour = !plus(essai);
      if (strcmp(t->tcons.mot->mval,"mult") == 0)
        retour = !mult(essai);
      if (strcmp(t->tcons.mot->mval,"supeg") == 0)
        retour = !supeg(essai);
 
      if (strcmp(t->tcons.mot->mval,"lit") == 0)
        retour = !lit(essai);
      if (strcmp(t->tcons.mot->mval,"ecrit") == 0)
        retour = !ecrit(essai);
      if (strcmp(t->tcons.mot->mval,"return") == 0)
        retour = !new_line(essai);
       
      if (outofmemory)
        return false;

      pile = essai;
      trouve_candidats();
    }
    
    while ((pile != NULL) && ((pile->but == NULL) || retour))
      if (retour) {
        retour = false;
        pile = pile->butprec;
        if ((pile != NULL) && (pile->but != NULL)) {
          hautbutpile = pile->rbut;
          hautenvpile = pile->renv;
          depile_inst(pile->rinst);
        }
      } else if (pile->candidats == NULL)
        retour = true;
      else {
        pile->candidats = NULL;
        
        if (pile->butsuiv != NULL) {
          pile->butsuiv->butprec = pile;
          pile = pile->butsuiv;
          trouve_candidats();
        } else {
          retour = true;
          solutions = true;
          affiche_variables(c->cvariables,envpile);
        }
      }
  }

  return solutions;
}

void resout_prog (progptr p,clauseptr c)

{ 
  if (c->ctete == NULL)
    if (recherches(p,c))
      printf("oui\n");
    else if (outofmemory)
      printf("memoire insuffisante\n");
    else
      printf("non\n");
  else
    if (ajoute_clause(p,c) == NULL)
      printf("memoire insuffisante\n");
    else
      printf("OK\n");
      
  hautbutpile = 0;
  hautenvpile = 0;
  hautinstpile = 0;
}
