/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* wm_delete.c -- demonstrate how to bind the Close button in the
 * window manager's system menu to the "cancel" button in a dialog.
 */
#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/Protocols.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, button;
    XtAppContext app;
    void activate();

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize  (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    button = XtCreateManagedWidget ("Push Me", xmPushButtonWidgetClass,
        toplevel, NULL, 0);
    XtAddCallback (button, XmNactivateCallback, activate, NULL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* Create and popup an ErrorDialog indicating that the user may have
 * done something wrong.  The dialog contains an OK and Cancel button,
 * but he can still choose the Close button in the titlebar.
 */
void
activate(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
    Widget dialog, shell;
    void response();
    XmString t = XmStringCreateLocalized ("Warning: Delete All Files?");
    Atom WM_DELETE_WINDOW;
    Arg args[5];
    int n;

    /* Make sure the VendorShell associated with the dialog does not
     * react to the user's selection of the Close system menu item.
     */
    n = 0;
    XtSetArg (args[n], XmNmessageString, t); n++;
    XtSetArg (args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
    dialog = XmCreateWarningDialog (w, "notice", args, n);
    XmStringFree (t);

    /* add callback routines for ok and cancel -- desensitize help */
    XtAddCallback (dialog, XmNokCallback, response, NULL);
    XtAddCallback (dialog, XmNcancelCallback, response, NULL);
    XtSetSensitive (XmMessageBoxGetChild (dialog, 
        XmDIALOG_HELP_BUTTON), False);

    XtManageChild (dialog);

    /* Add a callback for the WM_DELETE_WINDOW protocol */
    shell = XtParent (dialog);
    WM_DELETE_WINDOW = XmInternAtom 
        (XtDisplay (w), "WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback (shell, WM_DELETE_WINDOW, response, dialog);
}

/* callback for the OK and Cancel buttons in the dialog -- may also be
 * called from the WM_DELETE_WINDOW protocol message sent by the wm.
 */
void
response (widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;
    Widget dialog;

    if  (cbs->reason == XmCR_OK) 
        puts ("Yes");
    else
        puts ("No");

#ifdef MOTIF_1_2_3
    if  (cbs->reason == XmCR_PROTOCOLS)
#else
    if (cbs->reason != XmCR_CANCEL && cbs->reason != XmCR_OK)
#endif
        /* we passed the dialog as client data for the protocol callback */
        dialog =  (Widget) client_data;
    else
        dialog = widget;

    XtDestroyWidget (dialog);
}
