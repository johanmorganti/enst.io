 /***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *									*
 *									*
 *									*
 *									*
 ***********************************************************************/

import java.io.*;


// Definition d'une classe
class StrClass {
	// VARIABLES D'INSTANCE
	// str contient la chaine que nous allons traiter
	StringBuffer str = new StringBuffer();

	// METHODE reverseStr
	// la chaine s est inversee et memorisee dans res
	public StringBuffer inverseStr(StringBuffer s){

	StringBuffer res = new StringBuffer(s.length());

	for (int i = s.length() - 1; i >= 0 ; i--)
	  res.append(s.charAt(i));

	return res; 
	}

	// METHODE afficheEsp (StringBuffer s, char c)
	// affiche les lettres de s separees par le caractere c   
	public void afficheEsp (StringBuffer s, char c) {

	for(int i = 0; i < s.length(); i++)
	{
	  System.out.print(s.charAt(i));
	  System.out.print(c);
	}
	System.out.println();
	}

	// METHODE afficheEsp (StringBuffer s) - OVERLOADING
	// affiche les lettres de s separees par un espace   
	public void afficheEsp (StringBuffer s) {
	this.afficheEsp(s,' ');
	}

	// METHODE ajouterStr
	// ajoute a str la chaine " - Objet de la classe StrClass"
	public void ajouterStr () {
	this.str.append(" - Objet de la classe StrClass");
	}

}


// Definition d'une sous-classe de StrClass
class StrClass2 extends StrClass {

	// METHODE ajouterStr - OVERRIDING
	// ajoute a str la chaine " - Objet de la classe StrClass2 - Overriding"
	public void ajouterStr () {
	this.str.append(" - Objet de la classe StrClass2 - Overriding");
	}
}

// Classe principale contenant la methode main()
class ExString {
	// MAIN
	public static void main(String args[]) {

	// Creation de deux objets (instances)
	StrClass  obj  = new StrClass();
	StrClass2 obj2 = new StrClass2();

	char temp;
	int i = 0;

	System.out.println("Introduire une chaine de caracteres : ");

	// Lecture de la chaine introduite par l'utilisateur
	// Remarquer la gestion des exceptions
	try {
	while ((temp = (char) System.in.read()) != '\n')
	  {
	   obj.str.append(temp);
	   i++;
	  }
	}
	catch(java.io.IOException e) {
	   System.out.println("Exception caught :" + e);
	}

	// Copie du contenu de la chaine de obj dans obj2	
	obj2.str.append(obj.str);


	// Appel de methodes de la classe StringBuffer
	System.out.println("\nAPPEL DE METHODES DE LA CLASSE StringBuffer");

	System.out.println("\nChaine introduite : " + obj.str);
	System.out.println("Dimension de la chaine : " + obj.str.length());
	System.out.println("Espace disponible avant reallocation de memoire : " + obj.str.capacity());

	// Appel de methodes de la classe StrClass
	System.out.println("\nAPPEL DE METHODES DE LA CLASSE StrClass");
	System.out.println("Chaine inversee : " + obj.inverseStr(obj.str));

	System.out.print("Affichage 1 : ");
	obj.afficheEsp(obj.str);
	System.out.print("Affichage 2 : ");
	obj.afficheEsp(obj.str, '*');

	// Appel de methodes reecrites pour la classe StrClass2
	System.out.println("METHODE ajouterStr de la StrClass");
	obj.ajouterStr();
	System.out.println(obj.str);
	System.out.println("METHODE ajouterStr de la StrClass2");
	obj2.ajouterStr();
	System.out.println(obj2.str);

	}

}

