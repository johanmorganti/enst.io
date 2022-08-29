/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* form_corners.c -- demonstrate form layout management.  Just as
 * in corners.c, there are four widgets each labeled top-left,
 * top-right, bottom-left and bottom-right.  Their positions in the
 * form correspond to their names.  As opposed to the BulletinBoard
 * widget, the Form manages this layout management automatically by
 * specifying attachment types for each of the widgets.
 */
#include <Xm/PushB.h>
#include <Xm/Form.h>

char *corners[] = {
    "Top Left", "Top Right", "Bottom Left", "Bottom Right",
};

main(argc, argv)
char *argv[];
{
    Widget toplevel, form;
    XtAppContext app;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    form = XtVaCreateManagedWidget ("form",
        xmFormWidgetClass, toplevel, NULL);

    /* Attach the edges of the widgets to the Form.  Which edge of
     * the widget that's attached is relative to where the widget is
     * positioned in the Form.  Edges not attached default to having
     * an attachment type of XmATTACH_NONE.
     */
    XtVaCreateManagedWidget (corners[0],
        xmPushButtonWidgetClass, form,
        XmNtopAttachment,        XmATTACH_FORM,
        XmNleftAttachment,       XmATTACH_FORM,
        NULL);

    XtVaCreateManagedWidget (corners[1],
        xmPushButtonWidgetClass, form,
        XmNtopAttachment,        XmATTACH_FORM,
        XmNrightAttachment,      XmATTACH_FORM,
        NULL);

    XtVaCreateManagedWidget (corners[2],
        xmPushButtonWidgetClass, form,
        XmNbottomAttachment,     XmATTACH_FORM,
        XmNleftAttachment,       XmATTACH_FORM,
        NULL);

    XtVaCreateManagedWidget (corners[3],
        xmPushButtonWidgetClass, form,
        XmNbottomAttachment,     XmATTACH_FORM,
        XmNrightAttachment,      XmATTACH_FORM,
        NULL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
