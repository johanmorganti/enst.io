/* Written by Dave Brennan.
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* showuid.c --
 * Program to show the interface defined in a UID file
 * with manage and unmanage callbacks
 */

#include <stdio.h>
#include <Mrm/MrmAppl.h>

void quit();
void print();
void manage();
void unmanage();

static MrmRegisterArg callback_list[] = {
    { "quit",     (XtPointer) quit },
    { "print",    (XtPointer) print },
    { "manage",   (XtPointer) manage },
    { "unmanage", (XtPointer) unmanage },
  /* Add additional callback procedures here... */
};

typedef struct {
    String root_widget_name;
} app_data_t;

static app_data_t app_data;

static XtResource resources[] = {
    { "root", "Root", XmRString, sizeof(String),
        XtOffsetOf (app_data_t,root_widget_name), XmRString, 
        (XtPointer) "root" },
};

static XrmOptionDescRec options[] = {
    { "-root", "root", XrmoptionSepArg, NULL },
};

void
quit (w, client_data, call_data)
Widget    w;
XtPointer client_data;
XtPointer call_data;
{
    exit (0);
}

void
print (w, client_data, call_data)
Widget    w;
XtPointer client_data;
XtPointer call_data;
{
    char *message = (char *) client_data;
    puts (message);
}

void
manage (w, client_data,call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget target = (Widget) client_data;
    XtManageChild (target);
}

void
unmanage (w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget target = (Widget) client_data;
    XtUnmanageChild (target);
}
int
main (argc, argv)
int   argc;
char *argv[];
{
    XtAppContext  app_context;
    Widget        toplevel;
    Widget        root_widget;
    Cardinal      status;
    MrmHierarchy  hierarchy;
    MrmType       class_code;

    XtSetLanguageProc (NULL, NULL, NULL);
  
    MrmInitialize();

    toplevel = XtVaAppInitialize (&app_context, "Demos", options, 
        XtNumber(options), &argc, argv, NULL, NULL);

    XtGetApplicationResources (toplevel, &app_data, resources, 
        XtNumber(resources), NULL, 0);

    /* Check number of args after Xt and App have removed their options. */
    if (argc < 2) {
        fprintf (stderr, 
            "usage: showuid [Xt options] [-root name] uidfiles ...\n");
	exit (1);
    }

    status = MrmOpenHierarchyPerDisplay (XtDisplay (toplevel), argc - 1,
        argv + 1, NULL, &hierarchy);

    if (status != MrmSUCCESS) {
        XtAppError (app_context, "MrmOpenHierarchyPerDisplay failed");
	exit (1);
    }

    MrmRegisterNames (callback_list, XtNumber (callback_list));

    status = MrmFetchWidget (hierarchy, app_data.root_widget_name,
        toplevel, &root_widget, &class_code); 

    if (status != MrmSUCCESS) {
        XtAppError (app_context, "MrmFetchWidget failed");
	exit (1);
    }

    MrmCloseHierarchy (hierarchy);
    
    XtManageChild (root_widget);
    XtRealizeWidget (toplevel);
    
    XtAppMainLoop (app_context);
}
