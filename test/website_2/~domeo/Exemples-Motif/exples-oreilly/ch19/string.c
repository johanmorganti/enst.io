/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* string.c  -- create a compound string with the "MY_TAG" tag.
 * The tag defaults to the "9x15" font.  Create three pushbuttons: 
 * pb1, pb2, and pb3.  The user can specify resources so that each of the
 * widgets has a different font associated with the "MY_TAG" tag
 * specified in the compound string.
 */
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>

String fallbacks[] = { "*fontList:9x15=MY_TAG", NULL };

main(argc, argv)
int argc;
char *argv[];
{
    Widget        toplevel, rowcol;
    XtAppContext  app;
    XmString      text;
    Display      *dpy;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "String", NULL, 0,
        &argc, argv, fallbacks, NULL);

    text = XmStringCreate ("Testing, testing...", "MY_TAG");

    rowcol = XtVaCreateWidget ("rowcol",
        xmRowColumnWidgetClass, toplevel,
        NULL);

    XtVaCreateManagedWidget ("pb1", 
        xmPushButtonGadgetClass, rowcol,
        XmNlabelString, text,
        NULL);

    XtVaCreateManagedWidget ("pb2", 
        xmPushButtonGadgetClass, rowcol,
        XmNlabelString, text,
        NULL);

    XtVaCreateManagedWidget ("pb3", 
        xmPushButtonGadgetClass, rowcol,
        XmNlabelString, text,
        NULL);

    XmStringFree (text);
    XtManageChild (rowcol);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
