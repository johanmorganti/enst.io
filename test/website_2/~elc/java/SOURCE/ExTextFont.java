/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			   	  Texte					*
 *									*
 ***********************************************************************/
// Dimension 500 120

import java.awt.*;
import java.applet.*;

public class ExTextFont extends Applet {
	public void init() {
		validate();
	}

	public void paint(Graphics g) {
		Font font = getFont();

		g.drawRect(0, 0, size().width - 1, size().height - 1);

		g.setFont(font = new Font(font.getName(), font.getStyle(), 25));
		draw(g, font, 10, 50);

		g.setFont(font = new Font(font.getName(), font.ITALIC, 12));
		draw(g, font, 10, 70);

		g.setFont(font = new Font(font.getName(), font.BOLD, 8));
		draw(g, font, 10, 90);
	}

	public void draw(Graphics g, Font f, int x, int y) {
		g.drawString("Font : " + f.getName() + " de dimension " + f.getSize() + " et style " + f.getStyle(), x, y);
	}

}