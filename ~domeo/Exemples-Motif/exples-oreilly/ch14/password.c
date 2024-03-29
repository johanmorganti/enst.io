/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* password.c -- prompt for a password. All input looks like
 * a series of *'s.  Store the actual data typed by the user in
 * an internal variable.  Don't allow paste operations.  Handle
 * backspacing by deleting all text from insertion point to the
 * end of text.
 */
#include <Xm/Text.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <ctype.h>

void check_passwd();
char *passwd; /* store user-typed passwd here. */

main(argc, argv)
int argc;
char *argv[];
{
    Widget        toplevel, text_w, rowcol;
    XtAppContext  app;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    rowcol = XtVaCreateWidget ("rowcol",
        xmRowColumnWidgetClass, toplevel,
        XmNorientation, XmHORIZONTAL,
        NULL);

    XtVaCreateManagedWidget ("Password:",
        xmLabelGadgetClass, rowcol, NULL);
    text_w = XtVaCreateManagedWidget ("text_w",
        xmTextWidgetClass, rowcol, NULL);

    XtAddCallback(text_w, XmNmodifyVerifyCallback, check_passwd, NULL);
    XtAddCallback(text_w, XmNactivateCallback, check_passwd, NULL);

    XtManageChild (rowcol);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* check_passwd() -- handle the input of a password. */
void
check_passwd(text_w, client_data, call_data)
Widget        text_w;
XtPointer     client_data;
XtPointer     call_data;
{
    char *new;
    int len;
    XmTextVerifyCallbackStruct *cbs = 
        (XmTextVerifyCallbackStruct *) call_data;

    if (cbs->reason == XmCR_ACTIVATE) {
        printf ("Password: %s\n", passwd);
        return;
    }

    if (cbs->startPos < cbs->currInsert) {   /* backspace */
        cbs->endPos = strlen (passwd);       /* delete from here to end */
        passwd[cbs->startPos] = 0;           /* backspace--terminate */
        return;
    }

    if (cbs->text->length > 1) {
        cbs->doit = False;  /* don't allow "paste" operations */
        return;             /* make the user *type* the password! */
    }

    new = XtMalloc (cbs->endPos + 2); /* new char + NULL terminator */
    if (passwd) {
        strcpy (new, passwd);
        XtFree (passwd);
    } else
        new[0] = NULL;
    passwd = new;
    strncat (passwd, cbs->text->ptr, cbs->text->length);
    passwd[cbs->endPos + cbs->text->length] = 0;

    for (len = 0; len < cbs->text->length; len++)
        cbs->text->ptr[len] = '*';
}
