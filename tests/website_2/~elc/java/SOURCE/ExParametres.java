/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			       LES APPLETS				*
 *									*
 *			        Parametres				*
 *									*
 ***********************************************************************/

import java.applet.*;
import java.awt.Graphics;

public class ExParametres extends Applet {
	String nomPersonne;
	String prenomPersonne;
	String adressePersonne;
	String telephonePersonne;

	public void init() {
		nomPersonne = getParameter("NOM");
		if (nomPersonne == null) {
			nomPersonne = "Parametre non fourni";
		}

		prenomPersonne = getParameter("PRENOM");
		if (prenomPersonne == null) {
			prenomPersonne = "Parametre non fourni";
		}

		adressePersonne = getParameter("ADRESSE");
		if (adressePersonne == null) {
			adressePersonne = "Parametre non fourni";
		}

		telephonePersonne = getParameter("TEL");
		if (telephonePersonne == null) {
			telephonePersonne = "Parametre non fourni";
		}

		showStatus("Lecture Parametres Effectuee"); 
	}

	public void paint(Graphics g) {
		g.drawRect(0, 0, size().width - 1, size().height - 1);
		g.drawString("Nom : " + nomPersonne, 5, 15);
		g.drawString("Prenom : " + prenomPersonne, 5, 30);
		g.drawString("Adresse : " + adressePersonne, 5, 45);
		g.drawString("Telephone : " + telephonePersonne, 5, 60);
	}

	public String[][] getParameterInfo() {
		String[][] info = {
		{"nom",		"String",	"Nom de la personne"},
		{"prenom",	"String",	"Prenom de la personne"},
		{"adresse",	"String",	"Adresse de la personne"},
		{"tel",		"String",	"Telephone de la personne"}
		};
		return info;
	}
}