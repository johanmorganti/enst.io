/* Written by Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* traversal.c -- demonstrate keyboard traversal in a ScrolledWindow 
 * using the XmNtraverseObscuredCallback.
 */
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/ScrolledW.h>
#include <Xm/RowColumn.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, sw, rc;
    XtAppContext app;
    void traverse();
    int i;
    char name[10];

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0, 
        &argc, argv, NULL, NULL);

    sw = XtVaCreateManagedWidget ("scrolled_w",
        xmScrolledWindowWidgetClass, toplevel,
        XmNscrollingPolicy, XmAUTOMATIC,
        NULL);

    XtAddCallback (sw, XmNtraverseObscuredCallback, traverse, NULL);

    /* RowColumn is the work window for the widget */
    rc = XtVaCreateWidget ("rc", 
        xmRowColumnWidgetClass, sw, 
	XmNorientation, XmHORIZONTAL,
        XmNpacking, XmPACK_COLUMN,
	XmNnumColumns, 10,
        NULL);

    for ( i = 0; i < 10; i++ ) {
        sprintf (name, "Toggle %d", i);
        XtVaCreateManagedWidget (name, xmToggleButtonWidgetClass, rc, NULL);
        sprintf (name, "Button %d", i);
        XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, rc, NULL);
    }
    XtManageChild (rc);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

void
traverse(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    XmTraverseObscuredCallbackStruct *cbs = 
        (XmTraverseObscuredCallbackStruct *) call_data;

    XmScrollVisible (widget, cbs->traversal_destination, 10, 10);
}

