 /***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  PASSAGE DE PARAMETRES				*
 *			DE LA LIGNE  DE COMMANDES			*
 *									*
 *									*
 ***********************************************************************/

// Syntaxe acceptee: ParLigneCommande -verbose -cvx -file filename

class ParLigneCommande{
	public static void main(String args[]) {

	int i;
	String curarg;

	// Affichage des parametres
	System.out.println("Parametre introduits :");
	for (i = 0; i < args.length; i++)
	   System.out.println("\tPar n." + i + " " + args[i]);

	// Etude des differents parametres
	i = 0;
	System.out.println("\nPhase de reconnaissance");
	while (i < args.length)
	{
	  // Parametre courrant
	  curarg = args[i++];

	  if (curarg.equals("-file"))
	  {
		// Il faut que le parametre suivant soit un nom de fichier
		if (i < args.length)
		  System.out.println("\tOption -file > nom fichier : " + args[i++]);
		else
		  {
		  System.err.println("\tOption -file >  nom fichier non present !");
		  break;
		  }
	  }

	  else if (curarg.equals("-verbose"))
	  {
		System.out.println("\tOption -verbose");
	  }

	  // Reconnaissance des flags
	  else if (curarg.startsWith("-"))
	  {
		// Etude de la suite de flags (cas "-xvf" par exemple)
		for (int j = 1; j < curarg.length(); j++)
			switch(curarg.charAt(j)) {
			case 'v':
				System.out.println("\tOption -v");
				break;
			case 'c':
				System.out.println("\tOption -c");
				break;
			case 'x':
				System.out.println("\tOption -x");
				break;
			default :
				System.err.println("\tOption incorrecte : -" + curarg.charAt(j));
				break;
			}
	  }

	else
	  {
	  System.out.println("\tErreur dans l'introduction des parametres");
	  System.out.println("\tUsage: ParLigneCommande -verbose -cvx -file filename");
	  break;
	  }
	} // End while

      } // End main() 
} // End class