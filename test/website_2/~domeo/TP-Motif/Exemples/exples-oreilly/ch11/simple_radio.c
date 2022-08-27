/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* simple_radio.c -- demonstrate a simple radio box by using
 * XmVaCreateSimpleRadioBox().  Create a box with 3 toggles:
 * "one", "two" and "three".  The callback routine prints
 * the most recently selected choice.
 */
#include <Xm/RowColumn.h>

void
toggled(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    int which = (int) client_data;
    XmToggleButtonCallbackStruct *state = 
        (XmToggleButtonCallbackStruct *) call_data;

    printf ("%s: %s\n", XtName (widget), state->set? "on" : "off");
}

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, radio_box;
    XtAppContext app;
    XmString one, two, three;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    one  = XmStringCreateLocalized ("One");
    two  = XmStringCreateLocalized ("Two");
    three  = XmStringCreateLocalized ("Three");
    radio_box = XmVaCreateSimpleRadioBox (toplevel, "radio_box",
        0,  /* the inital choice */
        toggled, /* the callback routine */
        XmVaRADIOBUTTON, one,  NULL, NULL, NULL,
        XmVaRADIOBUTTON, two,  NULL, NULL, NULL,
        XmVaRADIOBUTTON, three, NULL, NULL, NULL,
        NULL);
    XmStringFree (one);
    XmStringFree (two);
    XmStringFree (three);

    XtManageChild (radio_box);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
