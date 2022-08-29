/* fichier cli_arith.c */

#include <rpc/rpc.h>
#include <sys/time.h>
#include "arith.h"
#include <stdlib.h>


void main ( int argc, char *argv[])
{
  CLIENT *cl;
  int Param, 
    *Resultat, *bid_carre_1(), *bid_rac_1();
  char c, *Hote;
  
  if (argc != 3)
    {
      printf ("Utilisation : %s hote nombre\n", argv [0]);
      exit (1);
    }
  Hote = argv [1];
  Param = atoi (argv [2]);
  
  cl = clnt_create (Hote, BID_PROG, BID_VERS, "udp");
  if (cl == NULL )
    {
      clnt_pcreateerror (Hote);
      exit (1);
    }
  
  printf (" Que faire (rac ou carre)? \n");
  scanf ("%c",&c);
  switch (c) 
    {
    case 'c' :
      Resultat = bid_carre_1 (&Param, cl);
      if (Resultat  == NULL )
	{
	  clnt_perror (cl, Hote);
	  exit (1);
	}
      printf (" %d au carre vaut : %d\n", Param, *Resultat);
      break;
    case 'r' :
      Resultat = bid_rac_1 (&Param, cl);
      if (Resultat  == NULL )
	{
	  clnt_perror (cl, Hote);
	  exit (1);
	}
      printf ("  racine de %d vaut : %d\n", Param, *Resultat);
	break;
    }	  
exit (0);
}

	
	
	
	













