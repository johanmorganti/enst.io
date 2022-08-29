/* ===================================================================== *
 *
 * fichier donnees.c
 * 
 * ===================================================================== */

#include <stdio.h>
#include <string.h>
#include "tri.h"
#include "donnees.h"

/* ===================================================================== */

Lire(ITEM items[], int *nb_items)
{
  int i;

  printf("*** Entrer les donnees et taper . pour terminer\n\n");

  for (i = 0; TRUE; i++) {

    printf("> nom    : ");
    scanf("%s", items[i].nom);
    if (items[i].nom[0] == '.') break;

    printf("> prenom : ");
    scanf("%s", items[i].prenom);

    printf("> age    : ");
    scanf("%d", &(items[i].age));
    putchar('\n');
  }
  printf("--- Fin de la saisie\n\n");
  *nb_items = i;
}

/* ===================================================================== */

Afficher(ITEM items[], int nb_items)
{
  int i;
  printf("\n*** Liste des donnees :\n\n");

  for (i = 0; i < nb_items; i++) {
    printf("Nom    : %s \n",   items[i].nom);
    printf("Prenom : %s \n",   items[i].prenom);
    printf("Age    : %d \n\n", items[i].age);
  }
}

/* ====================================================== The End ====== */


