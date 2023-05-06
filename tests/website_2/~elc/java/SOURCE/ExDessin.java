/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			   	  Dessin				*
 *									*
 ***********************************************************************/

import java.awt.*;
import java.applet.*;

public class ExDessin extends Applet {

	public void init() {
		validate();
	}

	public void paint(Graphics g) {
		// Dimensions de l'espace de dessin
		int X = size().width;
		int Y = size().height;
		// Marge
		int m = 5;
		// Dimensions d'une "cellule"
		int largCell = X/4;
		int hautCell = Y/4;

		int dimx, dimy;

		Color bg = getBackground();
		Color fg = getForeground(); 

		// Ligne
		g.drawLine(m, hautCell - m, X - m, m);
		
		dimx = largCell - m;
		dimy = hautCell - m;

		// Rectangle
		g.drawRect(m, hautCell + m, dimx, dimy);

		// Rectangle 'plein'
		g.fillRect(largCell + m, hautCell + m, dimx, dimy);

		// Rectangle avec epaisseur
		g.setColor(bg); 
		g.draw3DRect(2*largCell + m, hautCell + m, dimx, dimy, true);
		g.setColor(fg);


		// Rectangle avec epaisseur 'plein'
		g.setColor(bg); 
		g.fill3DRect(3*largCell + m, hautCell + m, dimx, dimy, true);
		g.setColor(fg);

		// Rectangle avec angles arrondis
		g.drawRoundRect(m, 2*hautCell + m, dimx, dimy, 10, 10);

		// Rectangle aevc angles arrondis 'plein'
		g.fillRoundRect(largCell + m, 2*hautCell + m, dimx, dimy, 10, 10);

		// Forme ovale
		g.drawOval(2*largCell + m, 2*hautCell + m, dimx, dimy);

		// Forme ovale 'pleine'
		g.fillOval(3*largCell + m, 2*hautCell + m, dimx, dimy);

		// Arc
		g.drawArc(m, 3*hautCell + m, dimx, dimy, 0, 135);

		// Arc 'plein'
		g.fillArc(largCell + m, 3*hautCell + m, dimx, dimy, 0, 135);
		
		// Polygone
		Polygon p = new Polygon();
		p.addPoint(2*largCell + m, 3*hautCell + m);
		p.addPoint(3*largCell - m, 4*hautCell - m);
		p.addPoint(2*largCell + m, 4*hautCell - m);
		p.addPoint(3*largCell - m, 3*hautCell + m);
		g.drawPolygon(p);

		// Polygone 'plein'
		Polygon fp = new Polygon();
		fp.addPoint(3*largCell + m, 3*hautCell + m);
		fp.addPoint(4*largCell - m, 4*hautCell - m);
		fp.addPoint(3*largCell + m, 4*hautCell - m);
		fp.addPoint(4*largCell - m, 3*hautCell + m);
		g.fillPolygon(fp);
	}
}