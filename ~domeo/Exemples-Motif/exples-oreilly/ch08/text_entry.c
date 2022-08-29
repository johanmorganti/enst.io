/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* text_entry.c -- This demo shows how the RowColumn widget can be
 * configured to build a text entry form.  It displays a table of
 * right-justified Labels and Text widgets that extend to the right
 * edge of the Form.
 */
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>

char *text_labels[] = {
    "Name:", "Phone:", "Address:", "City:", "State:", "Zip Code:",
};

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, rowcol;
    XtAppContext app;
    char buf[8];
    int i;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    rowcol = XtVaCreateWidget ("rowcolumn",
        xmRowColumnWidgetClass, toplevel,
        XmNpacking,        XmPACK_COLUMN,
        XmNnumColumns,     XtNumber (text_labels),
        XmNorientation,    XmHORIZONTAL,
        XmNisAligned,      True,
        XmNentryAlignment, XmALIGNMENT_END,
        NULL);

    /* simply loop thru the strings creating a widget for each one */
    for (i = 0; i < XtNumber (text_labels); i++) {
        XtVaCreateManagedWidget (text_labels[i],
            xmLabelGadgetClass, rowcol,
            NULL);
        sprintf (buf, "text_%d", i);
        XtVaCreateManagedWidget (buf,
            xmTextWidgetClass, rowcol,
            NULL);
    }

    XtManageChild (rowcol);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
