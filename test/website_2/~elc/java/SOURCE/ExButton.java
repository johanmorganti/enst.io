/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			         Buttons				*
 *									*
 ***********************************************************************/

import java.awt.*;
import java.applet.*;

public class ExButton extends Applet {
	Label label;

	public void init() {
		setLayout(new FlowLayout());

		add(new Button("Bouton 1"));
		add(new Button("Bouton 2 - Plus Long"));
		add(new Button(":-)"));
		add(label = new Label("- Pas de click sur un bouton -"));
	}

	public void paint(Graphics g) {
		g.drawRect(0, 0, size().width - 1, size().height - 1);
	}

	public Insets insets() {
		return new Insets(5, 5, 5, 5);
	}

	public boolean handleEvent (Event e) {
		if (e.target instanceof Button) {
			String str = new String(((Button)e.target).getLabel());
			label.setText("Click sur " + str);
			label.setAlignment(label.CENTER);
		}
	return false;
	}

}
