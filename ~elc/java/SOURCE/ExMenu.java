/***********************************************************************
 *									*
 *		             LE LANGAGE JAVA				*
 *									*
 *		            Andrej  Sobkowski				*
 *									*
 *									*
 *			  INTERFACES GRAPHIQUES				*
 *									*
 *			   	   Menu					*
 *									*
 ***********************************************************************/
// Pas de Dimension 

import java.awt.*;
import java.applet.*;


public class ExMenu extends Applet {
	
	public void init() {
		myFrame frame = new myFrame();
		frame.resize(400,200);
		frame.show();
	}

	public void paint(Graphics g) {
	}

	public Insets insets() {
		return new Insets(5, 5, 5, 5);
	}
}

class myFrame extends Frame {
	TextArea actionTA = new TextArea(5, 30);

	public myFrame() {
		MenuBar mbar;
		Menu file, edit, help;
		MenuItem newM, open, save, quit;
		MenuItem undo, cut, copy, paste;
		MenuItem index, about;
		CheckboxMenuItem checkMenu1, checkMenu2;
 	 
		mbar = new MenuBar();
		setMenuBar(mbar);		

		file = new Menu("File");
		mbar.add(file);
		newM = new MenuItem("New");
		file.add(newM);
		open = new MenuItem("Open");
		file.add(open);
		save = new MenuItem("Save");
		file.add(save);
		file.addSeparator();
		quit = new MenuItem("Quit");
		file.add(quit);

		edit = new Menu("Edit");
		mbar.add(edit);
		undo = new MenuItem("Undo");
		edit.add(undo);
		cut = new MenuItem("Cut");
		edit.add(cut);
		copy = new MenuItem("Copy");
		edit.add(copy);
		paste = new MenuItem("Paste");
		paste.disable();
		edit.add(paste);

		help = new Menu("Help");
		mbar.add(help);
		mbar.setHelpMenu(help);
		index = new MenuItem("Index");
		help.add(index);
		about = new MenuItem("About...");
		help.add(about);
		help.addSeparator();
		checkMenu1 = new CheckboxMenuItem("Option1");
		help.add(checkMenu1);
		checkMenu2 = new CheckboxMenuItem("Option2");
		help.add(checkMenu2);	

		setLayout(new BorderLayout());
		actionTA.setEditable(false);
		add("Center", actionTA);
	
		setTitle("Exemple de Menu");

	}

	public boolean handleEvent (Event e) {
		if (e.id == Event.WINDOW_DESTROY) {
			dispose();
		}
	return super.handleEvent(e);
	}

	public boolean action (Event e, Object obj) {
		if (e.target instanceof MenuItem) {
			actionTA.appendText("Selection : " + obj + "\n");
		}
	return false;
	}
	
}
