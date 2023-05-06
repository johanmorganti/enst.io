/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* warning.c -- show a use of the WarningMsg() function.
 */
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

/*
 * WarningMsg() -- Inform the user that she is about to embark on a
 * dangerous mission and give her the opportunity to back out.
 */
void
WarningMsg(parent, client_data, call_data)
Widget parent;
XtPointer client_data;
XtPointer call_data;
{
    static Widget dialog;
    XmString text, ok_str, cancel_str;
    char *msg = (char *) client_data;

    if (!dialog)
        dialog = XmCreateWarningDialog (parent, "warning", NULL, 0);
    text = XmStringCreateLtoR (msg, XmFONTLIST_DEFAULT_TAG);
    ok_str = XmStringCreateLocalized ("Yes");
    cancel_str = XmStringCreateLocalized ("No");
    XtVaSetValues (dialog,
        XmNmessageString,     text,
        XmNokLabelString,     ok_str,
        XmNcancelLabelString, cancel_str,
        XmNdefaultButtonType, XmDIALOG_CANCEL_BUTTON,
        NULL);
    XmStringFree (text);
    XmStringFree (ok_str);
    XmStringFree (cancel_str);

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
    XtAddCallback (pb, XmNactivateCallback, WarningMsg, 
        "Do you really want to delete all files?");

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
