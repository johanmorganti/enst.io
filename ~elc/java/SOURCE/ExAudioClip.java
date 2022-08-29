/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			       LES APPLETS				*
 *									*
 *			        AudioClip				*
 *									*
 ***********************************************************************/

// Ce programme ne marche pas - trouverle bug
  
import java.applet.*;
import java.net.*;
import java.awt.*;

public class ExAudioClip extends Applet implements Runnable {
	AudioClip audio;
	URL audioURL;

	Thread audioThread;
	boolean threadSuspended = false;

	public void init() {
		resize(70, 20);
	}

	public void start() {
		if (audioThread == null) {
		audioThread = new Thread(this, "Audio");
		audioThread.start();
		}
	}

	public void run() {
		try {
		// URL correspondant au fichier
		audioURL = new URL(getCodeBase(), "../IMAGES/looney-tunes.au");
		showStatus("Fichier looney-tunes.au charge");
		}
		catch (MalformedURLException e) {
			showStatus("Erreur initialisation");
		}
		// Chargement et execution du fichier
		audio = getAudioClip(audioURL);
		audio.loop();
	}

	public synchronized void stop() {
		if (audioThread != null) {
			audioThread.stop();
			audioThread = null;
		}
	}

	public void paint(Graphics g) {
		g.drawRect(0, 0, size().width - 1, size().height - 1);
		g.drawString("AudioClip", 5, 15);
	}

	// Possibilite d'interrompre l'execution avec la souris
	public boolean mouseDown (Event e, int x, int y) {
		if (threadSuspended)
			audioThread.resume();
		else
			audioThread.suspend();
		
		threadSuspended = !threadSuspended;
		return true;
	}
}