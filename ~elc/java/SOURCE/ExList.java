/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			          List					*
 *									*
 ***********************************************************************/
// Dimension 300 100

import java.awt.*;
import java.applet.*;


public class ExList extends Applet {
	Label label = new Label("- Pas de selection -");
	List couleurs, numeros;

	public void init() {

		couleurs = new List(3, false);
		couleurs.addItem("Blanc");
		couleurs.addItem("Bleu");
		couleurs.addItem("Noir");
		couleurs.addItem("Rouge");
		couleurs.addItem("Vert");

		numeros = new List(3, true);
		numeros.addItem("Un");
		numeros.addItem("Deux");
		numeros.addItem("Trois");
		numeros.addItem("Quatre");
		numeros.addItem("Cinq");
		numeros.addItem("Six");
		numeros.addItem("Sept");
		numeros.addItem("Huit");
		numeros.addItem("Neuf");
		

		GridBagLayout g = new GridBagLayout();
		GridBagConstraints gc = new GridBagConstraints();
		
		setLayout(g);
		add(couleurs);

		gc.gridwidth = GridBagConstraints.REMAINDER;
		g.setConstraints(numeros, gc);
		add(numeros);

		gc.gridwidth = GridBagConstraints.REMAINDER;
		g.setConstraints(label, gc);
		add(label);

		validate();
	}

	public void paint(Graphics g) {
		g.drawRect(0, 0, size().width - 1, size().height - 1);
	}

	public Insets insets() {
		return new Insets(5, 5, 5, 5);
	}

	public boolean handleEvent (Event e) {
		if (e.target instanceof List) {
			List list = (List)e.target;
			StringBuffer str = new StringBuffer();
			int sIndex;

			switch (e.id) {
			case Event.LIST_SELECT:
				sIndex = ((Integer)e.arg).intValue();
				str.append("Selection de " + list.getItem(sIndex));
				break;

			case Event.LIST_DESELECT:
				sIndex = ((Integer)e.arg).intValue();
				str.append("Deselection de " + list.getItem(sIndex));
				break;

			}
			label.setText(str.toString());
			label.setAlignment(label.CENTER);
		}
	return false;
	}

}
