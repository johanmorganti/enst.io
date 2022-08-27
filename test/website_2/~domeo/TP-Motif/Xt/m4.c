/* xm4.c : creer un pulldown menu */

#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <Xm/Form.h>
#include <Xm/PushBG.h>


/* **************************************** fallbacks **** */

String fallback[] = {
	"*labelise.labelString: Good Bye World",
	"*bouton.labelString: Click and Die",
	NULL,
};


/* **************************************** callbacks **** */

		     /* actions provoquees par les boutons */

void action(w, client_data, call_data)
	Widget w; caddr_t client_data, call_data;
{
        char *str = (char *) client_data;
        printf ("> Bouton [%s] selectionne' \n", str);
}

				/* aide interactive */

void aide(w, client_data, call_data)
	Widget w; caddr_t client_data, call_data;
{
        int no_info = (int) client_data;

	if (no_info == 1)
	  printf ("! Help : info #1 : bla bla bla \n");
	else
	  printf ("! Help : info inconnue\n");
}

				/* termine le programme */

void sortir(w, client_data, call_data)
	Widget w; caddr_t client_data, call_data;
{
	exit(0);
}


/* ********************************************************************* */
/* ********************************************************************* */

main(argc,argv)
	int argc;
	char *argv[];
{
	Widget 
	  toplevel, form, label, menubar, pulldown,
	  menubar_buttons[3], buttons[3];
	XtAppContext app;
	XmString xm_string;
	Arg args[10];
	int i;


	/* **************************************** initialisation **** */

	toplevel = XtAppInitialize(&app,"Test", 
				   (XrmOptionDescList) NULL, 0,
				   &argc, argv, 
				   fallback,
				   (ArgList) NULL, 0);


	/* *********************** creation du conteneur principal **** */

	i = 0;
	XtSetArg(args[i], XmNwidth, 250); i++;
	XtSetArg(args[i], XmNheight, 125); i++;

	form = XmCreateForm(toplevel, "form", args, i);
	XtManageChild(form);


	/* ********************************** creation du Menu Bar **** */

	i = 0;
	XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
	XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
	XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;

	menubar = XmCreateMenuBar(form, "menubar", args, i);
	XtManageChild(menubar);;


	/* *********************** creation des boutons du MenuBar **** */

				/* pulldown menu pour le 1er bouton */

	pulldown = XmCreatePulldownMenu(menubar, "pulldown", NULL, 0);

				/* creation du bouton "Menu" */
	i = 0;
	xm_string = XmStringCreate("Menu", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, xm_string); i++;
	XtSetArg(args[i], XmNsubMenuId, pulldown); i++;

	menubar_buttons[0] = XmCreateCascadeButton(menubar, "menuB", args, i);
	XmStringFree(xm_string);

				/* creation du bouton "Quit" */
	i = 0;
	xm_string = XmStringCreate("Quit", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, xm_string); i++;

	menubar_buttons[1] = XmCreateCascadeButton(menubar, "quitB", args, i);
	XmStringFree(xm_string);
	XtAddCallback(menubar_buttons[1], XmNactivateCallback, sortir, NULL);

				/* creation du bouton "Help" */
	i = 0;
	xm_string = XmStringCreate("Help", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, xm_string); i++;

	menubar_buttons[2] = XmCreateCascadeButton(menubar, "helpB", args, i);
	XmStringFree(xm_string);
	XtAddCallback(menubar_buttons[2], XmNactivateCallback, aide, 1);

				/* indiquer que Help est un bouton special */
	i = 0;
	XtSetArg(args[i], XmNmenuHelpWidget, menubar_buttons[2]); i++;
	XtSetValues(menubar, args, i);

				/* manager les 3 boutons de la barre */

	XtManageChildren(menubar_buttons, 3);


	/* ***************************** creation des boutons du menu **** */

				/* 1er bouton du menu */
	i = 0;
	xm_string = XmStringCreate("1e Entree", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, xm_string); i++;

	buttons[0] = XmCreatePushButtonGadget(pulldown, "bouton0", args, i);
	XmStringFree(xm_string);
	XtAddCallback(buttons[0], XmNactivateCallback, action, "Entree 1");

				/* 2eme bouton du menu */
	i = 0;
	xm_string = XmStringCreate("2e Entree", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, xm_string); i++;

	buttons[1] = XmCreatePushButtonGadget(pulldown, "bouton1", args, i);
	XmStringFree(xm_string);
	XtAddCallback(buttons[1], XmNactivateCallback, action, "Entree 2");

				/* 3eme bouton du menu */
	i = 0;
	xm_string = XmStringCreate("Sortie", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(args[i], XmNlabelString, xm_string); i++;

	buttons[2] = XmCreatePushButtonGadget(pulldown, "bouton2", args, i);
	XmStringFree(xm_string);
	XtAddCallback(buttons[2], XmNactivateCallback, sortir, NULL);

				/* manager les 3 boutons du menu */
	XtManageChildren(buttons, 3);

	/* ********************************************* lancement **** */

	XtRealizeWidget(toplevel);
	XtAppMainLoop(app);
}


