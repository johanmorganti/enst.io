/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* dialog.c -- your typical Hello World program using
 * an InformationDialog.
 */
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

void
pushed(w)
Widget w;
{
    Widget dialog;
    extern void ok_pushed(), cancel_pushed(), help_pushed();
    Arg args[5];
    int n = 0;
    XmString m = XmStringCreateLocalized ("This is a message.");
    XmString t = XmStringCreateLocalized ("Message");

    XtSetArg (args[n], XmNautoUnmanage, False); n++;
    XtSetArg (args[n], XmNmessageString, m); n++;
    XtSetArg (args[n], XmNdialogTitle, t); n++;
    dialog = XmCreateMessageDialog (w, "notice", args, n);
    XtAddCallback (dialog, XmNokCallback, ok_pushed, "Hi");
    XtAddCallback (dialog, XmNcancelCallback, cancel_pushed, "Foo");
    XtAddCallback (dialog, XmNhelpCallback, help_pushed, NULL);
    XmStringFree (m);
    XmStringFree (t);
    
    XtManageChild (dialog);
    XtPopup (XtParent (dialog), XtGrabNone);
}

main(argc, argv)
int argc;
char *argv[];
{
    XtAppContext app;
    Widget toplevel, pb;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);
    pb = XtVaCreateManagedWidget ("Button",
        xmPushButtonWidgetClass, toplevel, NULL);
    XtAddCallback (pb, XmNactivateCallback, pushed, NULL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* ok_pushed() --the OK button was selected.  */
void
ok_pushed(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    char *message = (char *) client_data;

    printf ("OK was selected: %s\n", message);
    XtDestroyWidget (widget);
}

/* cancel_pushed() --the Cancel button was selected.  */
void
cancel_pushed(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    char *message = (char *) client_data;

    printf ("Cancel was selected: %s\n", message);
    XtDestroyWidget (widget);
}

/* help_pushed() --the Help button was selected.  */
void
help_pushed(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    printf ("Help was selected\n");
}
