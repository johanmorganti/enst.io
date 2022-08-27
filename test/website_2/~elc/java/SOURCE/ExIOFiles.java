 /***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			   LES ENTREES / SORTIES			*
 *									*
 *				Fichiers				*
 *									*
 ***********************************************************************/

import java.io.*;

class ExIOFiles {
	public static void main (String args[]) {
		DataOutputStream fileout;
		DataInputStream filein;

		String str = new String("Hello World !");


	// Ecriture de chaque lettre de str sur une ligne differente
	// de fileout.
	try {
		fileout = new DataOutputStream(new FileOutputStream("outfile.txt"));
		for (int i = 0; i < str.length(); i++)
			{
			fileout.writeChars("Lettre n. " + i + "\t " + str.charAt(i) + "\n");
			}

		fileout.close();
	}
	catch (IOException e) {
		System.out.println("Exception : " + e);
	}

	//Lecture et affichage sur l'ecran du contenu du fichier
	try {
		filein = new DataInputStream(new FileInputStream("outfile.txt"));
		boolean EOF = false;
		char c;

		try {
			while (!EOF)
			{
				while ((c = filein.readChar()) != '\n')
					System.out.print(c);

			System.out.println();
			}
			filein.close();
		}
		catch (EOFException e) {
			EOF = true;
		}
	}
	catch (FileNotFoundException e) {
		System.out.println("Exception :" + e);
	}
	catch (IOException e) {
		System.out.println("Exception :" + e);
	}
		

	}

}

