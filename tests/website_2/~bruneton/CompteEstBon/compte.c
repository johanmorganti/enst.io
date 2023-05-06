#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) > (y) ? (y) : (x))

#define PMAX 7              /* nb de nombres pour faire les calculs */
#define PMAX4 4096          /* PMAX4 = 4^(PMAX-1) */
#define STRMAX 14           /* STRMAX = 2*PMAX taille maximale des arbres en RPN */
#define VIDE -1             /* resultat d'une operation invalide (ex 5/3) */

typedef int liste[PMAX+1];  /* une liste de nombre ou d'indices */

typedef struct {            /* description d'un element de pile */
  int base;                 /* emplacement de la premiere valeur */
  int taille;               /* nombre de valeurs */
} element;

int but;                    /* le nombre a trouver */
liste nbs;                  /* nb de nombre (nbs[0]) puis les nombres eux memes */

char solution[STRMAX];      /* meilleure solution actuelle */
int numero;                 /* numero d'obtention pour calculer les operations */
int precision;              /* meilleure precision actuelle */

int valeurs[PMAX4];         /* pile des valeurs */
int nbvaleurs;              /* taille de cette pile */
element elements[PMAX];     /* pile des elements */
int nbelements;             /* taille de cette pile */

int aval[PMAX4];            /* zones intermediaires */
int bval[PMAX4]; 

/* insere inser dans str a partir du caractere pointe par place */

void inser (char *inser, char *str, char *place)

{
  char temp[STRMAX];

  strcpy(temp,place);
  strncpy(place,inser,strlen(inser)+1);
  strcat(str,temp);
}

/* traduit la solution obtenue en une chaine de caracteres plus comprehensible */

void ecrit_solution (void)

{
  int i,taille,a,b,c;
  int pile[PMAX];
  int top = -1;

  printf("\n");
  if (precision != 0)
    printf("il n'y a pas de solution, une meilleure approximation est :\n");

  if ((taille = strlen(solution)) == 1) 
    printf("%d = %d\n",nbs[solution[0] - '0'],nbs[solution[0] - '0']);
  else
    for (i = 0; i < taille; ++i)
      if (solution[i] == '?') { 
        a = MAX(pile[top],pile[top-1]);
        b = MIN(pile[top],pile[top-1]);

        switch (numero%4) {
          case 0 : c = a+b; printf("%d + %d = %d\n",a,b,c); break;
          case 1 : c = a*b; printf("%d * %d = %d\n",a,b,c); break;
          case 2 : c = a-b; printf("%d - %d = %d\n",a,b,c); break;
          case 3 : c = a/b; printf("%d / %d = %d\n",a,b,c); break;
        }

	numero = numero/4;
	pile[--top] = c;
      } else
	pile[++top] = nbs[solution[i] - '0'];

  exit(0);
}

/* choisit T[0] nombres parmi les n premiers entiers en fonction du choix precedent */

void choix_suivant (liste T, int n)

{
  int i,j,k,p;
  int bon = FALSE;

  i = n;
  p = j = T[0];

  do {
    if (T[p] < i) {
      ++T[p]; 
      for (k = p + 1; k <= j; ++k)
        T[k] = T[k-1] + 1;
      bon = TRUE;
    } else {
      --p;
      --i;
    }	
  } while (!bon && (p != 0));
}

/* repartit les nombres de nbs dans nbs1 et nbs2 */

void repartit (liste nbs, liste inds, liste nbs1, liste nbs2)

{
  int i,indice_courant = 1;

  nbs1[0] = nbs2[0] = 0;

  for (i = 1; i <= nbs[0]; ++i) 
    if ((indice_courant <= inds[0]) && (i == inds[indice_courant])) {
      ++indice_courant;
      nbs1[++nbs1[0]] = nbs[i];
    } else
      nbs2[++nbs2[0]] = nbs[i];
}

/* ajoute un nombre sur la pile */

void empile (int nb)

{
  elements[nbelements].base = nbvaleurs;
  elements[nbelements].taille = 1;
  ++nbelements;
  valeurs[nbvaleurs] = nb;
  ++nbvaleurs;
}

/* enleve un element de la pile */

void depile (int tab[])

{
  int i,taille,offset;

  --nbelements;
  tab[0] = taille = elements[nbelements].taille;
  offset = elements[nbelements].base - 1;
  nbvaleurs -= taille;

  for (i = 1; i <= taille; ++i)
    tab[i] = valeurs[i+offset];
}

/* depile deux elements et les combine avec les quatre operations */

void combine (void)

{
  int am,bm,a,b;
  int i,j,k;

  depile(bval);
  depile(aval);
  bm = bval[0];
  am = aval[0];
  elements[nbelements].base = nbvaleurs;
  elements[nbelements].taille = 4*am*bm;
  ++nbelements;

  k = nbvaleurs;
  nbvaleurs += 4*am*bm;

  for (j = 1; j <= bm; ++j)
    if ((b = bval[j]) != VIDE) 
      for (i = 1; i <= am; ++i)
	if ((a = aval[i])!= VIDE)
	  valeurs[k++] = a+b;
	else
	  valeurs[k++] = VIDE;
    else
      for (i = 1; i <= am; ++i)
	valeurs[k++] = VIDE;

  for (j = 1; j <= bm; ++j)
    if ((b = bval[j]) != VIDE) 
      for (i = 1; i <= am; ++i)
	if ((a = aval[i]) != VIDE)
	  valeurs[k++] = a*b;
	else
	  valeurs[k++] = VIDE;
    else
      for (i = 1; i <= am; ++i)
	valeurs[k++] = VIDE;

  for (j = 1; j <= bm; ++j)
    if ((b = bval[j]) != VIDE) 
      for (i = 1; i <= am; ++i)     
	if ((a = aval[i])!= VIDE)
	  valeurs[k++] = abs(a-b);
	else
	  valeurs[k++] = VIDE;
    else
      for (i = 1; i <= am; ++i)
	valeurs[k++] = VIDE;

  for (j = 1; j <= bm; ++j)
    if (((b = bval[j]) != VIDE) && (b != 0))
      for (i = 1; i <= am; ++i)
	if (((a = aval[i]) != VIDE) && (a != 0))
	  if (a > b)
	    if (a%b == 0)
	      valeurs[k++] = a/b;
	    else
	      valeurs[k++] = VIDE;
	  else if (b%a == 0)
	    valeurs[k++] = b/a;
	  else
	    valeurs[k++] = VIDE;
	else
	  valeurs[k++] = VIDE;
    else
      for (i = 1; i <= am; ++i)
	valeurs[k++] = VIDE;
}

/* calcule le nombre obtenu par la suite d'operations stockees dans arbre */

void evalue_arbre (char arbre[])

{
  int i,taille;

  nbelements = 0;
  nbvaleurs = 0;
  taille = strlen(arbre);

  for (i = 0; i < taille; ++i)
    if (arbre[i] == '?')
      combine();
    else
      empile(nbs[arbre[i] - '0']);

  for (i = 0; i < nbvaleurs; ++i)
    if (abs(valeurs[i] - but) < precision) {
      strcpy(solution,arbre);
      numero = i;
      precision = abs(valeurs[i] - but);
    }

  if (precision == 0)
    ecrit_solution();
}

/* procedure recursive qui engendre tous les calculs possibles avec les nombres nbs */
/* chaque calcul est stocke sous forme d'une chaine en notation polonaise inverse  */

void liste_arbre (char arbre[], int nbliste, liste listes[])

{
  char new_arbre[STRMAX];
  liste new_listes[PMAX];
  liste *src,ind;
  int tot,i,j,k;

  if (nbliste == 0)
    evalue_arbre(arbre);
  else {
    src = &(listes[nbliste-1]);

    if ((tot = (*src)[0]) == 1) {
      strcpy(new_arbre,arbre);
      strstr(new_arbre,"#")[0] = (*src)[1] + '0';
      liste_arbre(new_arbre,nbliste-1,listes);
    } else {
      k = tot;     /* nb de facons de prendre i nombres parmi tot */

      for (i = 0; i < nbliste-1; ++i)
	for (j = 0; j <= listes[i][0]; ++j)
	  new_listes[i][j] = listes[i][j];

      for (i = 1; i <= tot/2; ++i) {
	ind[0] = i;        
	for (j = 1; j <= i; ++j)
	  ind[j] = j;
        
	if (tot == 2*i)
	  k = k/2;

	for (j = 1; j <= k; ++j) {
	  strcpy(new_arbre,arbre);
	  inser("#?",new_arbre,strstr(new_arbre,"#")+1);
	  repartit(*src,ind,new_listes[nbliste],new_listes[nbliste-1]);
	  liste_arbre(new_arbre,nbliste+1,new_listes);
	  choix_suivant(ind,tot);
	}

	k = (k*(tot-i))/(i+1);
      }
    }
  }
}

void recherches (void)

{
  int i,j,k;
  liste ind;

  k = nbs[0];     /* nb de facons de prendre i nombres parmi nbs[0] */

  for (i = 1; i <= nbs[0]; ++i) {
    ind[0] = i;    
    for (j = 1; j <= i; ++j)
      ind[j] = j;

    for (j = 1; j <= k; ++j) {
      liste_arbre("#",1,&ind);
      choix_suivant(ind,nbs[0]);
    }
    k = (k*(nbs[0]-i))/(i+1);
  }
  
  ecrit_solution();
}

/* demande les donnees a l'utilisateur, initialise les variables globales, et lance les calculs */

#define ERREUR(msg) { printf("%s\n",msg); fflush(stdout); exit(-1); }

void main (void)

{
  int i;
  char s[255];

  printf("parametre p : ");
  gets(s);
  sscanf(s,"%d",&(nbs[0]));
  if ((nbs[0] < 3) || (nbs[0] > 7))
    ERREUR("p doit etre compris entre 3 et 7");

  for (i = 1; i <= nbs[0]; ++i) {
    printf("nombre %d : ",i);
    gets(s);
    sscanf(s,"%d",&(nbs[i]));
    if (((nbs[i] < 1) || (nbs[i] > 10)) && ((nbs[i]%25 != 0) || (nbs[i]/25 < 1) || (nbs[i]/25 > 4))) 
      ERREUR("les nombres doivent etre dans l'ensemble {1,...,10,25,50,75,100}");
  }

  printf("nombre a trouver : ");
  gets(s);
  sscanf(s,"%d",&but);
  if ((but < 100) || (but > 999))
    ERREUR("le nombre a trouver doit etre compris entre 100 et 999");

  numero = 1;
  strcpy(solution,"1");
  precision = abs(nbs[1] - but);

  recherches();
}