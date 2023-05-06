/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			       LES APPLETS				*
 *									*
 *				  Clock					*
 *									*
 ***********************************************************************/

/* Copyright (c) 1995, 1996 Sun Microsystems, Inc. All Rights Reserved. */

/* Improvements by Andrej Sobkowski (c) 1996 :-) */

import java.awt.Graphics;
import java.util.Date;

public class Clock extends java.applet.Applet implements Runnable {

    Thread clockThread;

    public void start() {
        if (clockThread == null) {
            clockThread = new Thread(this, "Clock");
            clockThread.start();
        }
    }

    public void run() {
        while (clockThread != null) {
            repaint();
            try {
                clockThread.sleep(1000);
            } catch (InterruptedException e){
            }
        }
    }

    public void paint(Graphics g) {
        Date now = new Date();

	StringBuffer m0 = new StringBuffer();
	StringBuffer s0 = new StringBuffer();

	int h = now.getHours();
	int m = now.getMinutes();
	int s = now.getSeconds();

	// Les lignes suivantes sont utiles quand le nombre de secondes
	// et minutes sont inferieures a 10 - la visualisation normale
	// donnerait "16:1:9" au lieu de "16:01:09"
	if (m < 10)
		m0.append('0');
	else
		m0.setLength(0);

	if (s < 10)
		s0.append('0');
	else
		s0.setLength(0);

        g.drawString(h + ":" + m0 +  m + ":" + s0 + s, 5, 10);
    }

    public void stop() {
        clockThread.stop();
        clockThread = null;
    }
}
