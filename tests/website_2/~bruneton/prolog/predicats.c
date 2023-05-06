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

/* ---------------------------------------------------------------------- */
/*   IMPLEMENTATION DES PREDICATS PREDEFINIS                              */
/* ---------------------------------------------------------------------- */

void affiche_variables (listeptr noms,envptr env)

{
  integer i = 0;
  instptr inst;
  
  while (noms != NULL) {
    ++i;
    if (strcmp(noms->elem.mot->mval,"_") != 0)
      if (est_instanciee(env,i,&inst)) {
        printf("%s = ",noms->elem.mot->mval);
        affiche_terme(inst->tmod,inst->tenv);
        printf("   ");
      }
      
    noms = noms->suiv;
  }
  
  if (i != 0)
    printf("\n");
}

boolean nombre (pileptr p)

{
  instptr inst;
  return ((est_instanciee(p->env,1,&inst)) && (inst->tmod->ttyp == typnum));
}

boolean atome (pileptr p)

{
  instptr inst;
  return ((est_instanciee(p->env,1,&inst)) && 
          (inst->tmod->ttyp == typmot) && (inst->tmod->targs == NULL));
}

boolean variable (pileptr p)

{
  instptr inst;
  return (!(est_instanciee(p->env,1,&inst)));
}

boolean plus (pileptr p)

{
  termeptr t;
  instptr inst1,inst2,inst3;

  if ((est_instanciee(p->env,1,&inst1)) && (inst1->tmod->ttyp == typnum))
    if ((est_instanciee(p->env,2,&inst2)) && (inst2->tmod->ttyp == typnum))
      if (est_instanciee(p->env,3,&inst3))
        return ((inst3->tmod->ttyp == typnum) && 
                (inst3->tmod->tcons.val == inst1->tmod->tcons.val + inst2->tmod->tcons.val));
      else {
        if ((t = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
          return false;
        t->ttyp = typnum;
        t->tcons.val = inst1->tmod->tcons.val + inst2->tmod->tcons.val;
        t->targs = NULL;
        return instancie(p->env,3,t,NULL);
      }
    else if ((est_instanciee(p->env,3,&inst3)) && (inst3->tmod->ttyp == typnum)) {
      if ((t = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
        return false;
      t->ttyp = typnum;
      t->tcons.val = inst3->tmod->tcons.val - inst1->tmod->tcons.val;
      t->targs = NULL;
      return instancie(p->env,2,t,NULL);
    } else {
    }
  else if ((est_instanciee(p->env,2,&inst2)) && (inst2->tmod->ttyp == typnum) && 
           (est_instanciee(p->env,3,&inst3)) && (inst3->tmod->ttyp == typnum)) {
    if ((t = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
      return false;
    t->ttyp = typnum;
    t->tcons.val = inst3->tmod->tcons.val - inst2->tmod->tcons.val;
    t->targs = NULL;
    return instancie(p->env,1,t,NULL);
  }

  return false;
}

boolean mult (pileptr p)

{
  termeptr t;
  instptr inst1,inst2,inst3;
  
  if ((est_instanciee(p->env,1,&inst1)) && (inst1->tmod->ttyp == typnum))
    if ((est_instanciee(p->env,2,&inst2)) && (inst2->tmod->ttyp == typnum))
      if (est_instanciee(p->env,3,&inst3))
        return ((inst3->tmod->ttyp == typnum) && 
                (inst3->tmod->tcons.val == inst1->tmod->tcons.val * inst2->tmod->tcons.val));
      else {
        if ((t = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
          return false;
        t->ttyp = typnum;
        t->tcons.val = inst1->tmod->tcons.val * inst2->tmod->tcons.val;
        t->targs = NULL;
        return instancie(p->env,3,t,NULL);
      }
    else if ((est_instanciee(p->env,3,&inst3)) && (inst3->tmod->ttyp == typnum) &&
             (inst1->tmod->tcons.val != 0) && 
             (inst3->tmod->tcons.val % inst1->tmod->tcons.val == 0)) {
      if ((t = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
        return false;
      t->ttyp = typnum;
      t->tcons.val = inst3->tmod->tcons.val / inst1->tmod->tcons.val;
      t->targs = NULL;
      return instancie(p->env,2,t,NULL);
    } else {
    }
  else if ((est_instanciee(p->env,2,&inst2)) && (inst2->tmod->ttyp == typnum) && 
           (est_instanciee(p->env,3,&inst3)) && (inst3->tmod->ttyp == typnum) && 
           (inst2->tmod->tcons.val != 0) &&
           (inst3->tmod->tcons.val % inst2->tmod->tcons.val == 0)) {
    if ((t = (termeptr)gc_malloc(sizeof(termerec))) == NULL)
      return false;
    t->ttyp = typnum;
    t->tcons.val = inst3->tmod->tcons.val / inst2->tmod->tcons.val;
    t->targs = NULL;
    return instancie(p->env,1,t,NULL);
  }

  return false;
}

boolean supeg (pileptr p)

{
  instptr inst1,inst2;

  if ((est_instanciee(p->env,1,&inst1)) && (inst1->tmod->ttyp == typnum) && 
      (est_instanciee(p->env,2,&inst2)) && (inst2->tmod->ttyp == typnum))
    return (inst1->tmod->tcons.val >= inst2->tmod->tcons.val);
  else
    return false;
}


boolean lit (pileptr p)

{
  termeptr t;
  instptr inst;
  
  if (est_instanciee(p->env,1,&inst))
    return false;
  else {
    printf("entrez un terme : ");
      
    if ((t = analyse_terme(prog,&getchar)) != NULL)
      if (instancie(p->env,1,t,NULL)) {
        terme_lu = NULL;
        return true;
      }
      
    fflush(stdin);
    terme_lu = NULL; 
    return false;
  }
}

boolean ecrit (pileptr p)

{
  instptr inst;
  
  if (est_instanciee(p->env,1,&inst)) {
    affiche_terme(inst->tmod,inst->tenv);
    return true;
  } else
    return false;
}

boolean new_line (pileptr p)

{
  printf("\n");
  return true;
}
