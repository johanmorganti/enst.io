/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* set_minimum.c -- demonstrate how to set the minimum size of a
 * window to its initial size.  This method is useful if your program
 * is initially displayed at its minimum size, but it would be too
 * difficult to try to calculate ahead of time what the initial size
 * would be.
 */
#include <Xm/PushB.h>

void getsize(), configure();

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, button;
    XtAppContext app;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL,
        XmNmaxWidth,     150,
        XmNmaxHeight,    100,
        XmNbaseWidth,    5,
        XmNbaseHeight,   5,
        XmNwidthInc,     5,
        XmNheightInc,    5,
        NULL);

    /* Add an event handler to trap the first configure event */
    XtAddEventHandler (toplevel, StructureNotifyMask, False, configure, NULL);

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

void
configure(shell, client_data, event)
Widget shell;
XtPointer client_data;
XEvent *event;
{
    XConfigureEvent *cevent = (XConfigureEvent *) event;

    if (cevent->type != ConfigureNotify)
        return;
    printf ("Width = %d, Height = %d\n", cevent->width, cevent->height);
    XtVaSetValues (shell,
        XmNminWidth, cevent->width,
        XmNminHeight, cevent->height,
        NULL);
    XtRemoveEventHandler (shell, StructureNotifyMask, False, configure, NULL);
}
