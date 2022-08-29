/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			         Checkbox				*
 *									*
 ***********************************************************************/
// Dimension 200 100

import java.awt.*;
import java.applet.*;


public class ExCheckbox extends Applet {
	Label label = new Label("- Pas de selection -");

	public void init() {

		Panel p1 = new Panel();

		p1.setLayout(new GridLayout(3,1));
		p1.add(new Checkbox("Check 1"));
		p1.add(new Checkbox("Check 2"));
		p1.add(new Checkbox("Check 3"));


		Panel p2 = new Panel();;

		CheckboxGroup cbg = new CheckboxGroup();
		p2.setLayout(new GridLayout(3,1));
		p2.add(new Checkbox("Check 4", cbg, false));
		p2.add(new Checkbox("Check 5", cbg, true));
		p2.add(new Checkbox("Check 6", cbg, false));

		GridBagLayout g = new GridBagLayout();
		GridBagConstraints gc = new GridBagConstraints();
		
		setLayout(g);
		add(p1);

		gc.gridwidth = GridBagConstraints.REMAINDER;
		g.setConstraints(p2, gc);
		add(p2);

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
		if (e.target instanceof Checkbox) {
			String str = new String(((Checkbox)e.target).getLabel());
			label.setText("Selection de " + str);
			label.setAlignment(label.CENTER);
		}
	return false;
	}

}
