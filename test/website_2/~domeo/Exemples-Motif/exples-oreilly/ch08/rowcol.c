/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* rowcol.c -- demonstrate a simple RowColumn widget.  Create one
 * with 3 pushbutton gadgets.  Once created, resize the thing in
 * all sorts of contortions to get a feel for what RowColumns can
 * do with its children.
 */
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, rowcol;
    XtAppContext app;
    
    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0, 
        &argc, argv, NULL, NULL);

    rowcol = XtVaCreateManagedWidget ("rowcolumn",
        xmRowColumnWidgetClass, toplevel, NULL);

    (void) XtVaCreateManagedWidget ("One",
        xmPushButtonWidgetClass, rowcol, NULL);

    (void) XtVaCreateManagedWidget ("Two",
        xmPushButtonWidgetClass, rowcol, NULL);

    (void) XtVaCreateManagedWidget ("Three",
        xmPushButtonWidgetClass, rowcol, NULL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
