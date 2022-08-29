/************************************************************************
 * PROGRAMME: superman
 * FICHIER: superfct.h
 * AUTEUR:  Vincent HASCOET
 * DATE: 07/95   (remanie en Jan97 - Elc)
 ************************************************************************/

typedef struct entry {
    char *suf;
    char *path;
    struct entry *next;
} Entry;

typedef struct node {
    char   *name;
    Entry  *next;
    struct node *L,*R;
} Node;

typedef struct prgmdatastruct {
    Widget toplevelW;		/* Toplevel Widget */
    /* Tree data */
    Node   *tree;		/* Racine de l'arbre */
    int    *stats;		/* Nombre de pages dans chaque repertoire */
    int     entryNb;		/* Nombre de pages differentes */
    int     overallEntryNb;	/* Nombre total de pages */
    /* MatchList data */
    Widget  listW;		/* XmScrolledList des pages trouvees */
    int     matchNb;		/* Nombre de pages trouves */
    char  **matchName;		/* Nom des pages trouvees */
    Entry **matchEntry;		/* Element de la liste chainee correspondant */
    /* History data */
    Widget  histW;		/* PulldownMenu de l'historique */
    int     histNb;		/* Nombre d'entrees de l'historique */
    char  **histName;		/* Nom des pages de l'historique */
    Entry **histEntry;		/* Element de la liste chainee correspondant */
    Widget *histButton;		/* PushButtons de l'historique */
    /* Text data */
    Widget  manW;		/* Text affichant la page choisie */
    Widget  searchboxW;		/* boite contenant le search */
    Widget  searchentryW;		/* TextField contenant le search pattern */
    char   *currentName;	/* Nom de la page affichee */
    Entry  *currentEntry;	/* Element de la liste chainee correspondant */
    /* Search data */
    int     leftStar;		/* Wildcard en debut du search pattern */
    int     rightStar;		/* Wildcard en fin du search pattern */
    int     caseSensitive;	/* Indicateur de 'case-sensitivite' */
} PrgmDataStruct;

/*************************************************************************
 *             M I S C E L L A N E O U S   F U N C T I O N S             
 *************************************************************************/

extern char **RetrieveManDirectories(char *varname, int *card);
extern void CreateNamesTree(PrgmDataStruct *prd, 
			    char **basepaths, int basecard);
extern void BuildList(PrgmDataStruct *data, char *name, 
		      int leftStar, int rightStar);
extern void DisplayHistManpageCB(Widget w, XtPointer, XtPointer);
extern void DisplayManpage(PrgmDataStruct *data, char *name, Entry *entry);
