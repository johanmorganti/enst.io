/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			   	  Canvas				*
 *									*
 ***********************************************************************/
// Dimensions 230 110

import java.awt.*;
import java.applet.*;

public class ExCanvas extends Applet {
	Image image;
	myCanvas zoneImage;

	public void init() {
		resize(250, 100);

		setLayout(new GridLayout(1,2));

		Panel p1 = new Panel();

		p1.setLayout(new GridLayout(3,1));
		p1.add(new Checkbox("Option 1"));
		p1.add(new Checkbox("Option 2"));
		p1.add(new Checkbox("Option 3"));
		add(p1);	
	
		image = getImage(getCodeBase(), "./IMAGES/graduate.gif");
		zoneImage = new myCanvas(image);
		add(zoneImage);
	}

	public void paint(Graphics g) {
		g.drawRect(0, 0, size().width - 1, size().height - 1);
	}

	public Insets insets() {
		return new Insets(5, 5, 5, 5);
	}
}

class myCanvas extends Canvas {
	Image image;

	public myCanvas (Image img) {
		image = img;
		resize(100,100);
	}

	public void paint(Graphics g) {
		if (image != null) {
			g.drawImage(image, 0, 0, 100, 100, this);
		}
	}
}
