/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			         Choice					*
 *									*
 ***********************************************************************/
// Dimension 300 45

import java.awt.*;
import java.applet.*;


public class ExChoice extends Applet {
	Label label = new Label("- Pas de selection -");
	Choice choix = new Choice();

	public void init() {

		choix.addItem("Blanc");
		choix.addItem("Bleu");
		choix.addItem("Noir");
		choix.addItem("Rouge");
		choix.addItem("Vert");

		setLayout(new FlowLayout());
		add(choix);
		add(label);

	}

	public void paint(Graphics g) {
		g.drawRect(0, 0, size().width - 1, size().height - 1);
	}

	public Insets insets() {
		return new Insets(5, 5, 5, 5);
	}

	public boolean handleEvent (Event e) {
		if (e.target instanceof Choice) {
			String str = new String(choix.getSelectedItem());

			label.setText("Selection : " + str);
			label.setAlignment(label.CENTER);
		}
	return false;
	}

}
