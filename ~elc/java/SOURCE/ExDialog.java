/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			   	Dialogues				*
 *									*
 ***********************************************************************/
// Pas de Dimension 

import java.awt.*;
import java.applet.*;

public class ExDialog extends Applet {
	public void init() {
		simpleFrame frame = new simpleFrame();
		frame.resize(200,100);
		frame.show();
	}

} 

class simpleFrame extends Frame {
	myDialog dial;
	Button bouton;

	public simpleFrame() {
		setLayout(new GridLayout(2,1));

		setTitle("Premiere Fenetre");

		Label label = new Label("- Appuyez sur le bouton -");
		label.setAlignment(label.CENTER);
		add(label);
		bouton = new Button("CLICK");
		add(bouton);
	}

	public boolean handleEvent (Event e) {
		if (e.id == Event.WINDOW_DESTROY) {
			dispose();
		}
	return super.handleEvent(e);
	}

	public boolean action (Event e, Object obj) {
		if (e.target == bouton) {
			if (dial == null)
			{
				dial = new myDialog(this, "Boite de dialogue");
				dial.show();
			}
			else
			{
				dial.show();
			}
		}
	return false;
	}
}

class myDialog extends Dialog {
	Button quitButton;
	Panel p;
	
	public myDialog (Frame f, String title) {
		super(f, title, false);

		setLayout(new GridLayout(2,1));

		Panel p = new Panel();
		p.setLayout(new FlowLayout());
		p.add(new Label("Mot de Passe : "));
		p.add(new TextField(10));
		add(p);

		quitButton = new Button("Fermer");
		add(quitButton);

		resize(250, 120);
	}

	public boolean action (Event e, Object obj) {
		if (e.target == quitButton) {
			this.hide();
			
		}
	return false;
	}
}