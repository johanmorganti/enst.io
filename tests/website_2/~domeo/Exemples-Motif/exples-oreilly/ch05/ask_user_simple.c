/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* ask_user_simple.c -- create a pushbutton that posts a dialog box
 * that asks the user a question that requires an immediate
 * response.  The function that asks the question actually
 * posts the dialog that displays the question, waits for and
 * returns the result.
 */
#include <X11/Intrinsic.h>
#include <Xm/DialogS.h>
#include <Xm/SelectioB.h>
#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>

XtAppContext app;

#define YES 1
#define NO  2

/* main() --create a pushbutton whose callback pops up a dialog box */
main(argc, argv)
int argc;
char *argv[];
{
    Widget parent, button, toplevel;
    XmString label;
    void pushed();

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    label = XmStringCreateLocalized ("/bin/rm *");
    button = XtVaCreateManagedWidget ("button",
        xmPushButtonWidgetClass, toplevel,
        XmNlabelString,          label,
        NULL);
    XtAddCallback (button, XmNactivateCallback,
        pushed, "Remove Everything?");
    XmStringFree (label);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* pushed() --the callback routine for the main app's pushbutton. */
void
pushed(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    char *question = (char *) client_data;

    if (AskUser (widget, question) == YES)
        puts ("Yes");
    else
        puts ("No");
}

/*
 * AskUser() -- a generalized routine that asks the user a question
 * and returns the Yes/No response.
 */
int
AskUser(parent, question)
Widget parent;
char *question;
{
    static Widget dialog;
    XmString text, yes, no;
    static int answer;
    extern void response();
    extern XtAppContext app;

    if (!dialog) {
        dialog = XmCreateQuestionDialog (parent, "dialog", NULL, 0);
        yes = XmStringCreateLocalized ("Yes");
        no = XmStringCreateLocalized ("No");
        XtVaSetValues (dialog,
            XmNdialogStyle,        XmDIALOG_FULL_APPLICATION_MODAL,
            XmNokLabelString,      yes,
            XmNcancelLabelString,  no,
            NULL);
        XtSetSensitive (
            XmMessageBoxGetChild (dialog, XmDIALOG_HELP_BUTTON),
            False);
        XtAddCallback (dialog, XmNokCallback, response, &answer);
        XtAddCallback (dialog, XmNcancelCallback, response, &answer);
        XmStringFree (yes);
        XmStringFree (no);
    }
    answer = 0;
    text = XmStringCreateLocalized (question);
    XtVaSetValues (dialog,
        XmNmessageString,      text,
        NULL);
    XmStringFree (text);
    XtManageChild (dialog);
    XtPopup (XtParent (dialog), XtGrabNone);

    /* while the user hasn't provided an answer, simulate main loop.
     * The answer changes as soon as the user selects one of the
     * buttons and the callback routine changes its value.
     */
    while (answer == 0)
        XtAppProcessEvent (app, XtIMAll);

    XtPopdown (XtParent (dialog));
    return answer;
}

/* response() --The user made some sort of response to the
 * question posed in AskUser().  Set the answer (client_data)
 * accordingly and destroy the dialog.
 */
void
response(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    int *answer = (int *) client_data;
    XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;

    switch (cbs->reason) {
        case XmCR_OK:
            *answer = YES;
            break;
        case XmCR_CANCEL:
            *answer = NO;
            break;
        default:
            return;
    }
}
