 /***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *					 				* 
 *			       LES  THREADS				*
 *			       			     			*
 *									*
 *									*
 ***********************************************************************/

// Cet exemple est cesne montrer la presence de time-slicing sur Unix
// Le temps CPU dedie a chaque thread est gere par le systeme.
 
class ExThreads {
	public static void main (String args[]) {
		
		ExThreadSimple Thread1 = new ExThreadSimple();
		ExThreadSimple Thread2 = new ExThreadSimple();

		// Choix des noms associes aux Threads
		Thread1.setName("n.1");
		Thread2.setName("n.2");

		// Demarrage - les deux threads ont la meme priorite initiale
		Thread1.start();
		Thread2.start();

	}
}

class ExThreadSimple extends Thread {
	public void run() {
	for (int i = 0; i < 400000; i++)
		{
		if ((i % 50000) == 0)
			System.out.println("Thread " + this.getName() + 
			" de priorite " + this.getPriority() + 
			" en execution - Iteration n." + i);

		// Il faudrait introduire ici une instruction 'yield()'
		// pour que le thread rende la main. Il ne faut pas que
		// le programme depende du systeme sur lequel il est
		// execute

		}
	System.out.println("Thread " + this.getName() + " TERMINE");
	}
}
