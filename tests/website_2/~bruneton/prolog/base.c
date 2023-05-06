#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _TYPES_
#include "types.h"
#endif

#ifndef _GC_
#include "gc.h"
#endif

/* ---------------------------------------------------------------------- */
/*   FONCTIONS D'ALLOCATION MEMOIRE POUR LES STRUCTURES DE DONNEES :      */
/*     - CREATION DE LISTES, DE MOTS, DE CLAUSES                          */
/*     - GESTION DES PILES POUR LA RESOLUTION                             */
/* ---------------------------------------------------------------------- */

listeptr ajoute_element (listeptr *l)

{
  listeptr m;
  
  if ((m = (listeptr)gc_malloc(sizeof(listerec))) == NULL)
    return NULL;

  m->suiv = NULL;
  m->elem.elem = NULL;
  
  if (*l == NULL) {
    m->prec = m;
    *l = m;
  } else {
    m->prec = (*l)->prec;
    m->prec->suiv = m; 
    (*l)->prec = m;
  }

  return m;
}

void enleve_element (listeptr *l,listeptr m)

{
  if (m->suiv != NULL)
    m->suiv->prec = m->prec;
    
  if (m == *l)
    *l = m->suiv;
  else {
    m->prec->suiv = m->suiv;
    if (m->suiv == NULL)
      (*l)->prec = m->prec;
  }
}

motptr ajoute_mot (progptr p,char *m)

{
  listeptr l;
  motptr e;

  l = p->dico;
  while ((l != NULL) && (strcmp(l->elem.mot->mval,m) != 0)) 
    l = l->suiv;

  if (l != NULL)
    return l->elem.mot;
  else {
    if ((l = ajoute_element(&(p->dico))) == NULL)
      return NULL;
    if ((e = (motptr)gc_malloc(sizeof(motrec))) == NULL)
      return NULL;
    
    l->elem.mot = e;
    strcpy(e->mval,m);
    e->mpaquet = NULL;
    return e;
  }
}

listeptr ajoute_clause (progptr p,clauseptr c)

{
  listeptr l,m;
  paquetptr pq;
  
  l = p->paquets;
  while ((l != NULL) && (l->elem.paquet->pnom != c->ctete->tcons.mot))
    l = l->suiv;
    
  if (l == NULL) {
    if ((m = ajoute_element(&(p->paquets))) == NULL)
      return NULL;
      
    if ((pq = (paquetptr)gc_malloc(sizeof(paquetrec))) == NULL) {
      enleve_element(&(p->paquets),m);
      return NULL;
    }
      
    m->elem.paquet = pq;
    pq->pnom = c->ctete->tcons.mot;
    pq->pclauses = NULL;

    if ((l = ajoute_element(&(pq->pclauses))) == NULL) {
      enleve_element(&(p->paquets),m);
      return NULL;
    }
  } else {
    pq = l->elem.paquet;
    if ((l = ajoute_element(&(pq->pclauses))) == NULL)
      return NULL;
  }

  l->elem.clause = c;
  c->ctete->tcons.mot->mpaquet = pq;
  
  return l;
}

pileptr empile_but (void)

{
  pileptr res;
  
  if (hautbutpile < nb_but) {
    res = &(butpile[hautbutpile]);
    ++hautbutpile;
    return res;
  } else {
    outofmemory = true;
    return NULL;
  }
}

envptr empile_env (integer taille)

{
  ptr res;
  integer i;
  
  if (hautenvpile + taille - 1 < nb_env) {
    for (i = 0;i < taille;++i) {
      envpile[hautenvpile+i].tmod = NULL;
      envpile[hautenvpile+i].tenv = NULL;
    }
  
    res = &(envpile[hautenvpile]);
    hautenvpile += taille;
  
    return res;
  } else {
    outofmemory = true;
    return NULL;
  }
}

boolean empile_inst (instptr inst)

{
  if (hautinstpile < nb_inst) {
    instpile[hautinstpile] = inst;
    hautinstpile++;
    return true;
  } else {
    outofmemory = true;
    return false;
  }
}

void depile_inst (integer stop)

{
  while (hautinstpile > stop) {
    --hautinstpile;
    instpile[hautinstpile]->tmod = NULL;
  }
}

boolean est_instanciee (envptr env,integer v,instptr *inst)

{
  *inst = &(env[v-1]);
  return ((*inst)->tmod != NULL);
}

boolean instancie (envptr env,integer v,termeptr instt,envptr inste)

{
  instptr inst;
  
  inst = &(env[v-1]);
  inst->tmod = instt;
  inst->tenv = inste;
  
  return empile_inst(inst);
}


void constructeur (termeptr *t,envptr *env)

{
  instptr inst;
  
  while (((*t)->ttyp > 0) && (est_instanciee(*env,(*t)->ttyp,&inst))) {
    *t = inst->tmod;
    *env = inst->tenv;
  }
}
