/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			       LES APPLETS				*
 *									*
 *			        Animations				*
 *									*
 ***********************************************************************/

/* Copyright (c) 1995, 1996 Sun Microsystems, Inc. All Rights Reserved. */

import java.awt.*;
import java.applet.Applet;

public class ExAnimation extends Applet implements Runnable {
	Image[] images = new Image[10];
	int frameNumber;
	int delay;
	Image animImage;
	Graphics animGraph;
	Dimension animDim;

	Thread animThread;
	boolean threadSuspended = false;

	public void init() {
		// Nombre d'images par seconde
		int fps = 10;
		// Intervalle de temps entre deux images
		delay = 100;

		// Chargement des images
		for (int i = 1; i <= 10; i++) {
			images[i - 1] = getImage(getCodeBase(),
					"../IMAGES/duke/T" + i + ".gif");
		}
	}

	public void start() {
		if (animThread == null) {
			animThread = new Thread(this, "Animation");
			animThread.start();
		}
	}

	public void stop() {
		animThread = null;
		animGraph = null;
		animImage = null;
	}

	public boolean mouseDown (Event e, int x, int y) {
		if (threadSuspended) 
			animThread.resume();
		else
			animThread.suspend();
		
		threadSuspended = !threadSuspended;
		return true;
	}

	public void run() {
		long startTime = System.currentTimeMillis();

		while (Thread.currentThread() == animThread) {
			repaint();

			try {
				startTime += delay;
				animThread.sleep(Math.max(0, startTime - System.currentTimeMillis()));
			}
			catch (InterruptedException e) {
				break;
			}
			frameNumber++;
		}
	}

	public void paint(Graphics g) {
		if (animImage != null) {
			g.drawImage(animImage, 0, 0, this);
		}
	}

	// Reecriture de update() afin d'eviter le flashing
	// Mise en oeuvre du Double Buffering
	public void update(Graphics g) {
		if ((animGraph == null)||(size().width != animDim.width)
			|| (size().height != animDim.height)) {
			animDim = size();
			animImage = createImage(size().width,
						size().height);
			animGraph = animImage.getGraphics();
		}

		animGraph.setColor(getBackground());
		animGraph.fillRect(0, 0, size().width, size().height);
		animGraph.setColor(Color.black);

		animGraph.drawImage(images[frameNumber % 10], 0, 0, this);
		g.drawImage(animImage, 0, 0, this); 
	}
}