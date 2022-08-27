/************************************************************************
 * PROGRAMME: superman
 * FICHIER: superfct.c
 * AUTEUR:  V. Hascoet - E. Lecolinet
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include "superfct.h"

/*
 * SplitEnv renvoie un pointeur sur un tableau
 * contenant les chaines de caracteres contenues
 * dans la variable d'environnement VarName. Le
 * nombre de ces chaines est stocke dans (*Pcount).
 * Ex: VarName=chaine1:chaine2:chaine3
 * => retourne {"chaine1","chaine2","chaine3"}
 *    (*Pcount) = (int) 3
 *
 */
char **RetrieveManDirectories(char *varname, int *card)
{
  char **table = (char**)malloc(sizeof(char*));    
  char *path = getenv(varname);
  char *s = strtok(path, ":");

  *card = 0;
  while (s) {
    table = (char**) realloc(table, (*card+2) * sizeof(char*));
    table[*card] = s;
    (*card)++;
    s = strtok(NULL,":");
  }
  table[*card] = s;		/* NULL terminated list */
  return(table);
}

/*
 * PutTree place une entree dans l'arbre
 * La fonction renvoie 1 si l'entree a ete ajoutee, 0 sinon.
 * L'entree n'est ajoute que si elle n'existe pas deja.
 *
 */
static int PutTree(Node **tree, char *path, char *name, char *ext)
{
  Entry **p_entry;
  int status = 0;

  /* Creation Initiale de l'Arbre */
  if (! *tree) {
    Node *node = (Node *) calloc(1,sizeof(Node));
    Entry *entry = (Entry *) calloc(1,sizeof(Entry));
    status = 1;
    node->name = name;
    node->L    = NULL;
    node->R    = NULL;
    entry->path = path;
    entry->suf  = ext;
    entry->next = NULL;
    node->next = entry;
    *tree = node;
  }

  /* Cas Strandard: ajout a arbre deja existant */
  else {
    int test = strcmp(name, (*tree)->name);
    if (test > 0)
      status = PutTree(&((*tree)->R), path, name, ext);
    else if (test < 0)
      status = PutTree(&((*tree)->L), path, name, ext);
    else {
      p_entry = &((*tree)->next);
      while (*p_entry) {
	if (strcmp(ext, (*p_entry)->suf) == 0) break;
	else p_entry = &((*p_entry)->next);
      }

      if (! *p_entry) {
	Entry *entry = (Entry *) calloc(1,sizeof(Entry));
	*p_entry = entry;
	status = 1;
	entry->path = path;
	entry->suf  = ext;
	entry->next = NULL;
      }
    }
  }
  return status;
}


/*
 * MakeTree cree l'arbre des Manpages grace a
 * la basepaths fournie par SplitEnv. Cette table
 * contient PathNb noms de repertoire dans lesquels
 * l'application va chercher les pages.
 * PathUse est un tableau contenant le nombre
 * d'entrees trouvees dans chaque repertoire.
 * EntryNb renvoie un pointeur sur le nombre
 * d'entrees distinctes trouvees.
 * L'arbre est stocke dans (*Tree)
 *
 */

void CreateNamesTree(PrgmDataStruct *prd, char **basepaths, int basecard)
{
  int i, baseno, fullno, fullcard = 0, taillemax = 100;
  char **fullpaths = malloc(taillemax * sizeof(char*));
  fullcard = 0;

  prd->stats = (int *) calloc(basecard, sizeof(int));
  for (i=0; i < basecard; i++) (prd->stats)[i] = 0;

  for (baseno = 0; baseno < basecard; baseno++) {
    DIR *dirman;
    struct dirent *dm;

    if (dirman = opendir(basepaths[baseno])) {
      for (dm = readdir(dirman); dm; dm = readdir(dirman)) {

	char *subdir = dm->d_name;
	if (subdir && strstr(subdir, "man") != NULL) {
	  DIR *dirp;
	  struct dirent *dp;
	  char *dirpath = malloc((strlen(basepaths[baseno])+strlen(subdir)+2)
				 * sizeof(char));
	  sprintf(dirpath, "%s/%s", basepaths[baseno], subdir);

	  if (dirp = opendir(dirpath)) {
	    for (dp = readdir(dirp); dp; dp = readdir(dirp)) {

	      char *p, *filename = dp->d_name;
	      if (filename 
		  && filename[0] != '.'	          /* pas . ou .. */
		  && (p = strrchr(filename,'.'))) {
		int 
		  namelen = p - filename,
		  extlen = strlen(p+1);
		char 
		  *name = (char*)malloc((namelen+1) * sizeof(char)),
		  *ext  = (char*)malloc((extlen+1) *sizeof(char));
		strncpy(name, filename, namelen);
		name[namelen] = '\0'; /* add NULL terminator */
		strcpy(ext, filename+namelen+1);
                /* stats */
		(prd->stats[baseno])++;
		(prd->entryNb) += PutTree(&(prd->tree), dirpath, name, ext);
	      }
	    }
	    closedir(dirp);
	  } /* if (dirp) */
	}
      }
      closedir(dirman);
    } /* if (dirman) */
  }
}

/*
 * BuildListExactName cherche toutes les pages dont le name est 'name'.
 * Si aucune entree n'est trouvee, la fonction ne fait rien.
 * Si une seule entree est trouvee, la manpage
 * correspondante est affichee dans le XmText data->manW.
 * Si plusieurs entrees sont trouvees, leurs
 * noms sont affiches dans la XmList data->listW.
 *
 */
static void BuildListExactName(PrgmDataStruct *data, char *name)
{
  Node *tree = data->tree;

  while (tree) {
    int test = strcmp(name, tree->name);
    if (test > 0) tree = tree->R;
    else if (test < 0) tree = tree->L;
    else {
      Entry *entry = tree->next;
      int test, i, matchnb = 0;

      do {
	matchnb++;
	entry = entry->next;
      }
      while(entry);

      data->matchNb = matchnb;
      data->matchName = (char**) realloc(data->matchName,
					 matchnb * sizeof(char *));
      data->matchEntry = (Entry**) realloc(data->matchEntry,
					   matchnb * sizeof(Entry *));
      entry = tree->next;
      data->matchName[0] = tree->name;
      data->matchEntry[0] = entry;

      for (i=1; i < matchnb; i++) {
	entry = entry->next;
	data->matchName[i] = tree->name;
	data->matchEntry[i] = entry;
      }
      break;			/* sortie du while */
    }
  }
}

/*
 *  cree de maniere recursive
 * la listes des entrees dont le nom commence par name,
 * la recherche est case-insensitive. Quelque soit le
 * nombre d'entrees trouvees, elle sont repertoriees dans
 * la XmList data->listW.
 *
 */
static void RecursiveBuildListNotInName(PrgmDataStruct *prd, Node *Tree, 
					char  *name, int leftStar)
{
    Entry *Ent;
    int
	i,
	sensitive,
	cmp,
	match = 0,
	MatchNb = 0;
    char str[200];

    if (Tree!=NULL) {
	if (Tree->L) RecursiveBuildListNotInName(prd, Tree->L, name, leftStar);
	sensitive = prd->caseSensitive;
	if (sensitive) cmp = !strncmp(Tree->name,name,strlen(name));
	else !strncasecmp(Tree->name,name,strlen(name));
	if (prd->rightStar 
	    ? (prd->caseSensitive
	       ? !strncmp(Tree->name,name, strlen(name))
	       : !strncasecmp(Tree->name,name,strlen(name)))
	    : (leftStar
	       ? (prd->caseSensitive
		  ? !strcmp(Tree->name + strlen(Tree->name) - strlen(name),name)
		  : !strcasecmp(Tree->name + strlen(Tree->name) - strlen(name),name))
	       : !strcasecmp(Tree->name,name))) {
	    /* comptage des suffixes */
	    Ent = Tree->next;
	    do {
		MatchNb++;
		Ent = Ent->next;
	    }
	    while(Ent!=NULL);
	    
	    prd->matchNb += MatchNb;
	    prd->matchName = (char**) realloc(prd->matchName,
						 (prd->matchNb+1)*sizeof(char *));
	    prd->matchEntry = (Entry**) realloc(prd->matchEntry,
						 (prd->matchNb+1)*sizeof(Entry *));
	    Ent = Tree->next;
	    prd->matchName[prd->matchNb-MatchNb] = Tree->name;
	    prd->matchEntry[prd->matchNb-MatchNb]  = Ent;
	    for (i=prd->matchNb-MatchNb+1;i<prd->matchNb;i++) {
		Ent=Ent->next;
		prd->matchName[i] = Tree->name;
		prd->matchEntry[i]  = Ent;
	    }
	    for(i=0;i<prd->matchNb;i++) {
		sprintf(str,"%s/%s.%s",
			prd->matchEntry[i]->path,
			prd->matchName[i],
			prd->matchEntry[i]->suf);
	    }
	}    
    }
    if (Tree->R) RecursiveBuildListNotInName(prd, Tree->R, name, leftStar);
}

/*
 * RecursiveBuildListInName cree de maniere recursive
 * la listes des entrees dont le nom contient name,
 * la recherche est case-insensitive. Quelque soit le
 * nombre d'entrees trouvees, elle sont repertoriees dans
 * la XmList prd->listW.
 *
 */
void RecursiveBuildListInName(PrgmDataStruct *prd, Node *Tree, char  *name)
{
    Entry *Ent;
    int
	test,
	i,
	match = 0,
	MatchNb = 0;
    static char TName[100];
    static int UpperLower='a'-'A';

    
    if (Tree!=NULL) {
	if (Tree->L!=NULL) RecursiveBuildListInName(prd, Tree->L, name);
	/*printf("%8d %8d %s\n",Tree->L,Tree->R,Tree->name);*/
	strcpy(TName,Tree->name);
	if (!prd->caseSensitive)
	    for (i=0;i<strlen(TName);i++)
		if ((TName[i]>='A')&&(TName[i]<='Z'))
		    TName[i]+= UpperLower;
	if (strstr(TName,name)!=NULL) {
	    /* comptage des suffixes */
	    Ent = Tree->next;
	    do {
		MatchNb++;
		Ent = Ent->next;
	    }
	    while(Ent!=NULL);
	    /*printf("%d matches\n",MatchNb);*/
	    
	    prd->matchNb += MatchNb;
	    prd->matchName = (char**) realloc(prd->matchName,
						 (prd->matchNb+1)*sizeof(char *));
	    prd->matchEntry = (Entry**) realloc(prd->matchEntry,
						 (prd->matchNb+1)*sizeof(Entry *));

	    Ent = Tree->next;
	    prd->matchName[prd->matchNb-MatchNb] = Tree->name;
	    prd->matchEntry[prd->matchNb-MatchNb]  = Ent;
	    for (i=prd->matchNb-MatchNb+1;i<prd->matchNb;i++) {
		Ent=Ent->next;
		prd->matchName[i] = Tree->name;
		prd->matchEntry[i]  = Ent;
	    }
	}    
    }
    if (Tree->R!=NULL) RecursiveBuildListInName(prd, Tree->R, name);
}


static char *FormatManpage(char *dirname, char *filename)
{
  FILE *pipe;
  int  val;
  int  allocCount = 1;
  char command[300];
  char *buffer = (char*) calloc(BUFSIZ, sizeof(char));
  char *ptr = buffer;
  char *end = buffer + BUFSIZ - 1;
  char *subdir, *mandir = NULL, *manname = NULL;

  if (!dirname || !filename 
      || ! (mandir = strdup(dirname))
      || ! (subdir = strrchr(mandir, '/')))
    return;
  
  /* Principe:
   * - enlever le dernier subdirectory de 'dirname'
   * - et le concatener avec 'filename' de maniere a ce que:
   *        mandir  = dirname <dont on a retranche> subdir
   *        manname = subdir/filename
   *
   * la raison de cette etrangete est que les man pages peuvent 
   * eventuellement faire reference a d'autres man pages auquel cas la
   * reference est toujours du type:
   *        .so manXX/trucmuche.YY
   *
   * il faut donc faire un 'cd' dans 'mandir' puis un troff sur 'manname'
   * pour que tout se passe bien dans ce cas de figure
   */

  *subdir++ = '\0';
  manname = malloc((strlen(subdir) + strlen(filename) +2) * sizeof(char));
  sprintf(manname, "%s/%s", subdir, filename);

  /* sprintf(command, "cd %s;tbl %s |neqn|nroff -man", path, name); */
  sprintf(command, "cd %s; nroff -man %s", mandir, manname);

  if (pipe = popen(command, "r")) {
    while ((val = getc(pipe)) != EOF) {
      if (val == '\b') {		/* backspace */
	ptr--;
      }
      else {
	if (ptr == end) {
	  allocCount++;
	  buffer = (char*) realloc(buffer, allocCount*BUFSIZ*sizeof(char));
	  ptr = buffer + (allocCount-1)*BUFSIZ - 1;
	  end = buffer + allocCount*BUFSIZ - 1;
	}
	*ptr++ = (char) val;
      }
    }
    pclose(pipe);
  }

  if (manname) free(manname);
  if (mandir) free(mandir);

  if (ptr > buffer) {
    *ptr = '\0';
    return(buffer);
  }
  else return(NULL);
}
    

void DisplayManpage(PrgmDataStruct *prd, char *name, Entry *entry)
{
  char pagename[300];
  char *txt;
  int i, AlreadyThere;

  if (prd->currentEntry==entry) 
    XmTextSetTopCharacter(prd->manW, 0);
  else {
    sprintf(pagename,"%s.%s", name, entry->suf);
    txt = FormatManpage(entry->path, pagename);
    XmTextSetString(prd->manW, txt);
    XmTextSetTopCharacter(prd->manW, 0);
    prd->currentEntry = entry;
    prd->currentName = name;

    /* Mise a jour de l'historique */
    AlreadyThere=0;
    for(i=0; i<prd->histNb; i++)
      if (prd->histEntry[i]==entry) AlreadyThere=1;

    if (!AlreadyThere) {
      /* FIXME Doit reordonner les boutons (Youngest first),
	 quelquesoit AlreadyThere. */
      prd->histNb++;
      prd->histName = (char**) realloc(prd->histName,
					prd->histNb*sizeof(char*));
      prd->histEntry = (Entry**) realloc(prd->histEntry,
					  prd->histNb*sizeof(Entry*));
      prd->histButton = (Widget*)  realloc(prd->histButton,
					    prd->histNb*sizeof(Widget));
      for (i=prd->histNb-1; i>0; i--) {
	prd->histEntry[i] = prd->histEntry[i-1];
	prd->histName[i] = prd->histName[i-1];
	prd->histButton[i] = prd->histButton[i-1];	
      }

      sprintf(pagename,"%s(%s)", name, entry->suf);
      prd->histButton[0] = XmCreatePushButton(prd->histW, pagename,
					       NULL, 0);
      XtAddCallback(prd->histButton[0], XmNactivateCallback,
		    DisplayHistManpageCB, NULL);
	    
      XtManageChild(prd->histButton[0]);
      prd->histEntry[0] = entry;
      prd->histName[0] = name;
    }
  }
}


static void UpdateList(PrgmDataStruct *prd)
{
    XmString *item;
    char str[100];
    int i;

    item = (XmString *) calloc(prd->matchNb, sizeof(XmString));
    for(i=0;i<prd->matchNb;i++) {
	sprintf(str,"%s(%s)",
		prd->matchName[i],
		(prd->matchEntry[i])->suf);
    item[i] = XmStringCreate(str, XmFONTLIST_DEFAULT_TAG);
    }
    XmListDeleteAllItems(prd->listW);
    XmListAddItems(prd->listW, item, prd->matchNb, 1);
    for(i=0;i<prd->matchNb;i++) XmStringFree(item[i]);
    XtFree((XtPointer) item);
}


/*
 * Cette fonction  etudie la syntaxe du champs et cree la liste des
 * manpages correspondantes. Si une seule page est trouvee, elle
 * est affichee. 
 * Type de recherche:
 * "string" -> recherche exacte, case-sensitive (BuildListExactName)
 * "string*" -> recherche des pages commencant par string,
 *              case-insensitive (BuildListBeginName)
 * "*string" ou "*string" -> recherche des pages contenant string,
 *                           case-insensitive (BuildListInName)/*
 * Cree la liste des pages correspondant a la chaine name, en tenant
 * compte des differents etats des ToggleButtons
 *
 */
void BuildList(PrgmDataStruct *prd, char *name, int leftStar, int rightStar)
{
    char *str;
    int	i, length, UpperLower, MatchType;

    MatchType = 2*leftStar + rightStar;
    length = strlen(name);
    UpperLower = (int) ('a'-'A');
    
    if (length!=0) {
	prd->matchNb = 0;
	free(prd->matchName);
	free(prd->matchEntry);
	prd->matchName = (char**) calloc(1, sizeof(char*));
	prd->matchEntry = (Entry**) calloc(1, sizeof(Entry*));

	switch (MatchType) {
	case 0:
	    /* EXACT */
	  if (prd->caseSensitive) BuildListExactName(prd, name);
	  else RecursiveBuildListNotInName(prd, prd->tree, name, leftStar);
	  break;
	case 1:
	case 2:
	case 3:
	  if (!prd->caseSensitive)
	    for (i=0;i<strlen(name);i++)
	      if ((name[i]>='A')&&(name[i]<='Z'))
		name[i]+= UpperLower;
	  if (MatchType==3)
	    RecursiveBuildListInName(prd, prd->tree, name);
	  else
	    RecursiveBuildListNotInName(prd, prd->tree, name, leftStar);
	  break;
	}
	
	UpdateList(prd);
    }
}
