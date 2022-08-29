#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _TYPES_
#define _TYPES_
#endif

/* #define _DEBUG_ */

/* --------------------- CONSTANTES --------------------- */

#define true 1
#define false 0

#define listecons  '©'                /* nom du constructeur de listes */
#define motcons  "©"
#define listevide 'ø'                 /* nom de l'atome liste vide */
#define motvide "ø"
#define coupe '!'                     /* mon du predicat coupe-choix */

#define typnum -1                     /* termes : -nombre             */
#define typmot  0                     /*          -atome              */
                                      /* > 0      -numero de variable */

#define maxcar 61                     /* longueur maxi des identificateurs */

/* --------------------- TYPES GENERAUX --------------------- */

typedef void *ptr;                    /* pointeur general */

typedef int boolean;                  /* boolean et entiers */
typedef int integer;                  /* (taille differente selon les machines) */
typedef long longint;

typedef char mot[maxcar];             /* identificateurs */

/* --------------------- STRUCTURES DE DONNEES --------------------- */

typedef struct sliste  *listeptr;
typedef struct smot    *motptr;
typedef struct sterme  *termeptr;
typedef struct sclause *clauseptr;
typedef struct spaquet *paquetptr;
typedef struct sprog   *progptr;

typedef union {
  ptr elem;
  motptr mot;
  termeptr terme;
  clauseptr clause;
  paquetptr paquet;
} choix;
    
typedef struct sliste {               /* liste doublement chainee */
  listeptr prec,suiv;                 /* elements precedent et suivant */
  choix elem;                         /* contenu de cet element */
} listerec;

typedef struct smot {                 /* un mot */
  mot mval;                           /* identificateur pour ce mot */
  boolean mutile;                     /* mot utilise ou non */
  paquetptr mpaquet;                  /* paquet de clauses de meme nom (si il existe) */
} motrec;

typedef struct sterme {               /* un terme */
  integer ttyp;                       /* type (nombre,atome ou variable) */
  union {                             /* constructeur du terme */
    motptr mot;                       /* atome ou variable */
    integer val;                      /* nombre */
  } tcons;
  listeptr targs;                     /* NULL ou liste des arguments */
} termerec;

typedef struct sclause {              /* une clause */
  termeptr ctete;                     /* tete de la clause ou NULL */
  listeptr cqueue;                    /* liste des termes de la queue de la clause */
  listeptr cvariables;                /* liste des variables utilisees */
  integer cnb;                        /* nombre de variables utilisees */
} clauserec;

typedef struct spaquet {              /* un paquet de clauses */
  motptr pnom;                        /* nom commun des tetes de clauses */
  listeptr pclauses;                  /* liste des clauses du paquet */
} paquetrec;

typedef struct sprog {                /* un programme */
  listeptr dico;                      /* liste des symboles utilises */
  listeptr paquets;                   /* liste des paquets de clauses de ce programme */
} progrec;

/* racines des structures de donnees utilisees par le GC */

extern termeptr terme_lu;             /* terme en cours d'analyse syntaxique */
extern clauseptr clause_lue;          /* clause en cours d'analyse syntaxique */
extern progptr prog;                  /* programme principal */

/* --------------------- STRUCTURES POUR LA RESOLUTION --------------------- */

#define nb_but 30000
#define nb_env 30000
#define nb_inst 20000

typedef struct sinst   *instptr,*envptr;
typedef struct spile   *pileptr;

typedef struct sinst {                /* une instantiation */
  termeptr tmod;                      /* le terme auquel la variable est instanciee */
  envptr tenv;                        /* l’environnement de ce terme (valeur de ses variables) */
} instrec;
    
typedef struct spile {                /* un element de la pile des buts */
  termeptr but;                       /* le but a satisfaire */
  envptr env;                         /* les liaisons de variables */
  listeptr candidats;                 /* clauses candidates pour l'unification */
  pileptr butsuiv,butprec;            /* buts precedents et suivant */
    
  integer rbut,renv,rinst;            /* copie des niveaux actuels des piles */
} pilerec;

extern boolean outofmemory;           /* indicateur de depassement memoire */

extern pileptr pile;

extern pilerec *butpile;              /* pile des buts (pile principale) */
extern integer hautbutpile;

extern instrec *envpile;              /* pile des environnements : pour chaque clause en cours */
extern integer hautenvpile;           /* de recherche de satisfiabilite, on alloue un environnement */
                                      /* cad une zone qui contient la valeur des ses variables */
                        
extern instptr *instpile;             /* pile des instantations des qu'une variable est instanciee */
extern integer hautinstpile;          /* dans la pile precedente, on le consigne dans cette pile */
