/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* prompt_phone.c -- a complex problem for XmNmodifyVerifyCallback.
 * prompt for a phone number by filtering digits only from input.
 * Don't allow paste operations and handle backspacing.
 */
#include <Xm/Text.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <ctype.h>
#include <stdio.h>

void check_phone();

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

    XtVaCreateManagedWidget ("Phone Number:",
        xmLabelGadgetClass, rowcol, NULL);
    text_w = XtVaCreateManagedWidget ("text_w",
        xmTextWidgetClass, rowcol, NULL);

    XtAddCallback (text_w, XmNmodifyVerifyCallback, check_phone, NULL);
    XtAddCallback (text_w, XmNmotionVerifyCallback, check_phone, NULL);

    XtManageChild (rowcol);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

void
check_phone(text_w, client_data, call_data)
Widget     text_w;
XtPointer  client_data;
XtPointer  call_data;
{
    char c;
    int len = XmTextGetLastPosition (text_w);
    XmTextVerifyCallbackStruct *cbs = 
        (XmTextVerifyCallbackStruct *) call_data;

    if (cbs->reason == XmCR_MOVING_INSERT_CURSOR) {
        if (cbs->newInsert != len)
	    cbs->doit = False;
        return;
    }

    /* no backspacing, typing or stuffing in middle of string */
    if (cbs->currInsert < len) {
        cbs->doit = False;
        return;
    }

    if (cbs->text->length == 0) { /* backspace */
        if (cbs->startPos == 3 || cbs->startPos == 7)
            cbs->startPos--;      /* delete the hyphen too */
        return;
    }

    if (cbs->text->length > 1) { /* don't allow clipboard copies */
        cbs->doit = False;
        return;
    }

    /* don't allow non-digits or let the input exceed 12 chars */
    if (!isdigit (c = cbs->text->ptr[0]) || len >= 12)
        cbs->doit = False;
    else if (len == 2 || len == 6) {
        cbs->text->ptr = XtRealloc (cbs->text->ptr, 2);
        cbs->text->length = 2;
        cbs->text->ptr[0] = c;
        cbs->text->ptr[1] = '-';
    }
}
