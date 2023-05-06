/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* resize_shell.c -- demonstrate the max and min heights and widths.
 * This program should be run to really see how mwm displays the
 * size of the window as it is resized.
 */
#include <Xm/PushB.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, button;
    XtAppContext app;
    extern void getsize();

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL,
        XmNminWidth,     75,
        XmNminHeight,    25,
        XmNmaxWidth,     150,
        XmNmaxHeight,    100,
        XmNbaseWidth,    5,
        XmNbaseHeight,   5,
        XmNwidthInc,     5,
        XmNheightInc,    5,
        NULL);

    /* Pushbutton's callback prints the dimensions of the shell. */
    button = XtVaCreateManagedWidget ("Print Size",
        xmPushButtonWidgetClass, toplevel, NULL);
    XtAddCallback (button, XmNactivateCallback, getsize, toplevel);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

void
getsize(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Widget shell = (Widget) client_data;
    Dimension width, height;

    XtVaGetValues (shell, 
        XmNwidth, &width, 
        XmNheight, &height, 
        NULL);
    printf ("Width = %d, Height = %d\n", width, height);
}
