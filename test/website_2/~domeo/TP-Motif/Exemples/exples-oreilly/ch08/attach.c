/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* attach.c -- demonstrate how attachments work in Form widgets. */

#include <Xm/PushB.h>
#include <Xm/Form.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, parent, one, two, three;
    XtAppContext app;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    parent = XtVaCreateManagedWidget ("form",
        xmFormWidgetClass, toplevel, NULL);
    one = XtVaCreateManagedWidget ("One",
        xmPushButtonWidgetClass, parent,
        XmNtopAttachment,   XmATTACH_FORM,
        XmNleftAttachment,  XmATTACH_FORM,
        NULL);
    two = XtVaCreateManagedWidget ("Two",
        xmPushButtonWidgetClass, parent,
        XmNleftAttachment,  XmATTACH_WIDGET,
        XmNleftWidget,      one,
        /* attach top of widget to same y coordinate as top of "one" */
        XmNtopAttachment,   XmATTACH_OPPOSITE_WIDGET,
        XmNtopWidget,       one,
        NULL);
    three = XtVaCreateManagedWidget ("Three",
        xmPushButtonWidgetClass, parent,
        XmNtopAttachment,   XmATTACH_WIDGET,
        XmNtopWidget,       one,
        /* attach left of widget to same x coordinate as left side of "one" */
        XmNleftAttachment,  XmATTACH_OPPOSITE_WIDGET,
        XmNleftWidget,      one,
        NULL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
