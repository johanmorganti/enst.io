/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* proc_traversal.c -- demonstrate how to process keyboard traversal
 * from a PushButton's callback routine.  This simple demo contains
 * a RowColumn (a tab group) and three PushButtons.  If any of the
 * PushButtons are activated (selected), the input focus traverses
 * to the "home" item.
 */ 
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, rowcol, pb;
    XtAppContext app;
    void do_it();

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0, 
        &argc, argv, NULL, NULL);

    rowcol = XtVaCreateManagedWidget ("rowcolumn",
        xmRowColumnWidgetClass, toplevel,
        XmNorientation, XmHORIZONTAL,
        NULL);

    (void) XtVaCreateManagedWidget ("OK",
        xmPushButtonWidgetClass, rowcol, NULL);

    pb = XtVaCreateManagedWidget ("Cancel",
        xmPushButtonWidgetClass, rowcol, NULL);
    XtAddCallback (pb, XmNactivateCallback, do_it, NULL);

    pb = XtVaCreateManagedWidget ("Help",
        xmPushButtonWidgetClass, rowcol, NULL);
    XtAddCallback (pb, XmNactivateCallback, do_it, NULL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* callback for pushbuttons */
void
do_it(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    /* do stuff here for PushButton widget */
    (void) XmProcessTraversal(widget, XmTRAVERSE_HOME);
}
