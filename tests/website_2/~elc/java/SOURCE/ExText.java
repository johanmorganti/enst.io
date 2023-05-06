/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			   TextField  TextArea				*
 *									*
 ***********************************************************************/
// Dimension 350 250

import java.awt.*;
import java.applet.*;


public class ExText extends Applet {
	TextField nomTF, prenomTF, telTF;
	TextArea actionTA;
	Label nomL, prenomL, telL;

	public void init() {
		GridBagLayout g = new GridBagLayout();
		GridBagConstraints gc = new GridBagConstraints();
		
		setLayout(g);

		nomL = new Label("Nom : ");
		gc.anchor = GridBagConstraints.WEST;
		g.setConstraints(nomL, gc);
		add(nomL);

		nomTF = new TextField(20);
		gc.gridwidth = GridBagConstraints.REMAINDER;
		g.setConstraints(nomTF, gc);
		add(nomTF);


		prenomL = new Label("Prenom : ");
		gc.gridwidth = GridBagConstraints.RELATIVE;
		gc.anchor = GridBagConstraints.WEST;
		g.setConstraints(prenomL, gc);
		add(prenomL);

		prenomTF = new TextField(20);
		gc.gridwidth = GridBagConstraints.REMAINDER;
		g.setConstraints(prenomTF, gc);
		add(prenomTF);


		telL = new Label("Telephone : ");
		gc.gridwidth = GridBagConstraints.RELATIVE;
		gc.anchor = GridBagConstraints.WEST;
		g.setConstraints(telL, gc);
		add(telL);

		telTF = new TextField(20);
		gc.gridwidth = GridBagConstraints.REMAINDER;
		g.setConstraints(telTF, gc);
		add(telTF);

		actionTA = new TextArea(6, 35);
		actionTA.setEditable(false);
		gc.gridwidth = GridBagConstraints.REMAINDER;
		gc.insets = new Insets(5, 5, 5, 5);
		gc.anchor = GridBagConstraints.CENTER;
		g.setConstraints(actionTA, gc);
		add(actionTA);
		
		validate();
	}

	public void paint(Graphics g) {
		g.drawRect(0, 0, size().width - 1, size().height - 1);
	}

	public Insets insets() {
		return new Insets(5, 5, 5, 5);
	}

	public boolean handleEvent (Event e) {
		StringBuffer str = new StringBuffer();

		switch (e.id) {
		case Event.ACTION_EVENT:
			str.append(((TextField)e.target).getText());
			actionTA.appendText("Entree : " + str + "\n");
			break; 

		case Event.KEY_PRESS:
			actionTA.appendText("Code touche : " + e.key + "\n");
			break;
		}
	return super.handleEvent(e);
	}

}
