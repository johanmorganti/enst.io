/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* file_menu.c -- demonstrate how to create a menu bar and pulldown
 * menu using the Motif creation routines.
 */
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushBG.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, MainWindow, MenuBar, FilePullDown;
    XmString    label_str;
    XtAppContext app;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    MainWindow = XtVaCreateManagedWidget ("main_w",
        xmMainWindowWidgetClass, toplevel,
        XmNscrollingPolicy,  XmAUTOMATIC,
        NULL);

    MenuBar = XmCreateMenuBar (MainWindow, "MenuBar", NULL, 0); 

    /* create the "File" Menu */
    FilePullDown = XmCreatePulldownMenu (MenuBar, "FilePullDown", NULL, 0);

    /* create the "File" button (attach Menu via XmNsubMenuId) */
    label_str = XmStringCreateLocalized ("File");
    XtVaCreateManagedWidget ("File", 
        xmCascadeButtonWidgetClass, MenuBar,
        XmNlabelString,  label_str,
        XmNmnemonic,    'F',
        XmNsubMenuId,    FilePullDown,
        NULL);
    XmStringFree (label_str); 

    /* Now add the menu items */
    XtVaCreateManagedWidget ("Open",
        xmPushButtonGadgetClass, FilePullDown, NULL);

    XtVaCreateManagedWidget ("Save",
        xmPushButtonGadgetClass, FilePullDown, NULL);

    XtVaCreateManagedWidget ("separator",
        xmSeparatorGadgetClass, FilePullDown, NULL);

    XtVaCreateManagedWidget ("Exit",
        xmPushButtonGadgetClass, FilePullDown, NULL);

    XtManageChild (MenuBar);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
