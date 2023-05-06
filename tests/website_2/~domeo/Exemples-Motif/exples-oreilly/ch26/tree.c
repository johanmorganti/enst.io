/* Written by Dave Brennan.
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* tree.c --
 * Program to show the Tree and Panner widgets.
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/Xaw/Tree.h>
#include <X11/Xaw/Panner.h>
#include <X11/StringDefs.h>
#include <Mrm/MrmAppl.h>

void pan();

static MrmRegisterArg callback_list[] = {
    { "pan",          (XtPointer) pan },
    /* Add additional callback procedures here... */
};

Widget
XawCreateTreeWidget (parent, name, args, num_args)
Widget parent;
String name;
ArgList args;
Cardinal num_args;
{
    return (XtCreateWidget (name, treeWidgetClass, parent, args, num_args));
}

Widget
XawCreatePannerWidget (parent, name, args, num_args)
Widget parent;
String name;
ArgList args;
Cardinal num_args;
{
    return (XtCreateWidget (name, pannerWidgetClass, parent, args, num_args));
}

void
pan (panner, client_data, call_data)
Widget panner;
XtPointer client_data;
XtPointer call_data;
{
    Widget tree = (Widget) client_data;
    XawPannerReport *report = (XawPannerReport *) call_data;

    /* Should use XtSetValues, but DrawingArea bug prevents us */
    XtMoveWidget (tree, -report->slider_x, -report->slider_y);
}

int
main (argc, argv)
int   argc;
char *argv[];
{
    XtAppContext  app_context;
    Widget        toplevel, root_widget;
    Cardinal      status;
    static String uid_file_list[] = { "tree" };
    MrmType       class_code;
    MrmHierarchy  hierarchy;

    XtSetLanguageProc (NULL, NULL, NULL);
  
    MrmInitialize();

    toplevel = XtVaAppInitialize (&app_context, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    status = MrmOpenHierarchyPerDisplay (XtDisplay (toplevel), 
        XtNumber (uid_file_list), uid_file_list, NULL, &hierarchy);

    if (status != MrmSUCCESS) {
        XtAppError (app_context, "MrmOpenHierarchyPerDisplay failed");
        exit (1);
    }

    MrmRegisterNames (callback_list, XtNumber (callback_list));
    MrmRegisterClass (0, NULL, "XawCreateTreeWidget",
        XawCreateTreeWidget, treeWidgetClass);
    MrmRegisterClass (0, NULL, "XawCreatePannerWidget",
        XawCreatePannerWidget, pannerWidgetClass);

    status = MrmFetchWidget (hierarchy, "root", toplevel, &root_widget,
        &class_code);

    if (status != MrmSUCCESS) {
        XtAppError (app_context, "MrmFetchWidget failed");
        exit (1);
    }

    XtManageChild (root_widget);
    XtRealizeWidget (toplevel);

    XtAppMainLoop (app_context);
}
