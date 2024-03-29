/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* modal.c -- demonstrate modal dialogs.  Display two pushbuttons
 * each activating a modal dialog.
 */
#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

/* main() --create a pushbutton whose callback pops up a dialog box */
main(argc, argv)
char *argv[];
{
    XtAppContext app;
    Widget toplevel, button, rowcolumn;
    void pushed();

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    rowcolumn = XtCreateManagedWidget ("rowcolumn",
        xmRowColumnWidgetClass, toplevel, NULL, 0);

    button = XtCreateManagedWidget ("Application Modal",
        xmPushButtonWidgetClass, rowcolumn, NULL, 0);
    XtAddCallback (button, XmNactivateCallback,
        pushed, XmDIALOG_FULL_APPLICATION_MODAL);
    button = XtCreateManagedWidget ("System Modal",
        xmPushButtonWidgetClass, rowcolumn, NULL, 0);
    XtAddCallback (button, XmNactivateCallback, pushed,
        XmDIALOG_SYSTEM_MODAL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* pushed() --the callback routine for the main app's pushbutton.
 * Create either a full-application or system modal dialog box.
 */
void
pushed(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    static Widget dialog;
    XmString t;
    extern void dlg_callback();
    unsigned char modality = (unsigned char) client_data;

    /* See if we've already created this dialog -- if so,
     * we don't need to create it again.  Just re-pop it up.
     */
    if (!dialog) {
        Arg args[5];
        int n = 0;
        XmString ok = XmStringCreateLocalized ("OK");
        XtSetArg(args[n], XmNautoUnmanage, False); n++;
        XtSetArg(args[n], XmNcancelLabelString, ok); n++;
        dialog = XmCreateInformationDialog (widget, "notice", args, n);
        XtAddCallback (dialog, XmNcancelCallback, dlg_callback, NULL);
        XtUnmanageChild (
            XmMessageBoxGetChild (dialog, XmDIALOG_OK_BUTTON));
        XtUnmanageChild (
            XmMessageBoxGetChild (dialog, XmDIALOG_HELP_BUTTON));
    }
    t = XmStringCreateLocalized ("You must reply to this message now!");
    XtVaSetValues (dialog,
        XmNmessageString,    t,
        XmNdialogStyle,      modality,
        NULL);
    XmStringFree (t);
    XtManageChild (dialog);
    XtPopup  (XtParent (dialog), XtGrabNone);
}

void
dlg_callback(dialog, client_data, call_data)
Widget dialog;
XtPointer client_data;
XtPointer call_data;
{
    XtPopdown (XtParent (dialog));
}
