 /***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *									*
 *			      LES PROPRIETES     			*
 *									*
 *									*
 ***********************************************************************/

import java.util.*;
import java.io.*;

class ExProprietes {
	public static void main (String args[]) {
	Properties liste = new Properties(System.getProperties());
	Enumeration enum = liste.propertyNames();

	// Nom de la priopriete courante
	String propname = new String();
	// Object representant la propriete courante
	Object prop = new Object();

	// Fichier contenant la propriete a ajouter
	FileInputStream propFile;

	// Affichage Proprietes predefinies
	System.out.println("LISTE PROPRIETES PAR DEFAUT");
	while (enum.hasMoreElements())
		{
		prop = enum.nextElement();
		propname = prop.toString();
		System.out.print(prop);
		System.out.println(" --> "+ liste.getProperty(propname));
		}

	// Ajout d'une propriete
	// Le fichier Proprietes.txt contient la ligne suivante:
	// Nouvelle.Prop Propriete Ajoutee

	try {
		propFile = new FileInputStream("Proprietes.txt");
		liste.load(propFile);
		System.setProperties(liste);
	}
	catch (java.io.FileNotFoundException e) {
		System.out.println("Fichier non trouve");
	}
	catch (java.io.IOException e) {
		System.out.println("Exception : " + e);
	}
	
	// Affichage de la nouvelle propriete
	System.out.println("\nAUTRES PROPRIETES");
	System.out.println("Valeur Nouvelle.Prop : " + liste.getProperty("Nouvelle.Prop"));

	// Affichage d'une autre propriete (qui n'existe pas)
	System.out.println("\nValeur Autre.Prop : " + liste.getProperty("Autre.Prop", "Propriete non trouvee"));
	}
}