/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* simple_help.c -- create a PushButton that posts a dialog box
 * that entices the user to press the help button.  The callback
 * for this button displays a new dialog that gives help.
 */
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, button;
    XtAppContext app;
    XmString label;
    void pushed();

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    label = XmStringCreateLocalized ("Push Me");
    button = XtVaCreateManagedWidget ("button",
        xmPushButtonWidgetClass, toplevel,
        XmNlabelString,          label,
        NULL);
    XtAddCallback (button, XmNactivateCallback,
        pushed, "You probably need help for this item.");
    XmStringFree (label);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

#define HELP_TEXT "This is the help information.\nNow press 'OK'"

/* pushed() -- the callback routine for the main app's pushbutton. */
void
pushed(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
    char *text = (char *) client_data;
    Widget dialog;
    XmString t = XmStringCreateLocalized (text);
    Arg args[5];
    int n;
    void help_callback(), help_done();

    n = 0;
    XtSetArg (args[n], XmNautoUnmanage, False); n++;
    XtSetArg (args[n], XmNmessageString, t); n++;
    dialog = XmCreateMessageDialog (XtParent(w), "notice", args, n);
    XmStringFree (t);

    XtUnmanageChild (
        XmMessageBoxGetChild (dialog, XmDIALOG_CANCEL_BUTTON));

    XtAddCallback (dialog, XmNokCallback, help_done, NULL);
    XtAddCallback (dialog, XmNhelpCallback, help_callback, HELP_TEXT);

    XtManageChild (dialog);
    XtPopup (XtParent (dialog), XtGrabNone);
}

/*
 * help_callback() -- callback routine for the Help button in the 
 * original dialog box that displays an InformationDialog based on the 
 * help_text parameter.
 */
void
help_callback(parent, client_data, call_data)
Widget parent;
XtPointer client_data;
XtPointer call_data;
{
    char *help_text = (char *) client_data;
    Widget dialog;
    XmString text;
    void help_done();
    Arg args[5];
    int n;

    n = 0;
    text = XmStringCreateLtoR (help_text, XmFONTLIST_DEFAULT_TAG);
    XtSetArg (args[n], XmNmessageString, text); n++;
    XtSetArg (args[n], XmNautoUnmanage, False); n++;
    dialog = XmCreateInformationDialog (parent, "help", args, n);
    XmStringFree (text);

    XtUnmanageChild (  /* no need for the cancel button */
        XmMessageBoxGetChild (dialog, XmDIALOG_CANCEL_BUTTON));
    XtSetSensitive (   /* no more help is available. */
        XmMessageBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    /* the OK button will call help_done() below */
    XtAddCallback (dialog, XmNokCallback, help_done, NULL);

    /* display the help text */
    XtManageChild (dialog);
    XtPopup (XtParent (dialog), XtGrabNone);
}

/* help_done() -- called when user presses "OK" in dialogs. */
void
help_done(dialog, client_data, call_data)
Widget dialog;
XtPointer client_data;
XtPointer call_data;
{
    XtDestroyWidget (dialog);
}
