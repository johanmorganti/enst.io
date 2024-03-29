/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* pixmaps.c -- Demonstrate simple label gadgets in a row column.
 * Each command line argument represents a bitmap filename.  Try
 * to load the corresponding pixmap and store in a RowColumn.
 */
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>

main(argc, argv)
int argc;
char *argv[];
{
    XtAppContext app;
    Pixel fg, bg;
    Widget toplevel, rowcol;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    if (argc < 2) {
        puts ("Specify bitmap filenames.");
        exit (1);
    }
    /* create a RowColumn that has an equal number of rows and
     * columns based on the number of pixmaps it is going to
     * display (this value is in "argc").
     */
    rowcol = XtVaCreateWidget ("rowcol",
        xmRowColumnWidgetClass, toplevel,
        XmNnumColumns,  int_sqrt (argc),
        XmNpacking,     XmPACK_COLUMN,
        NULL);

    /* Get the foreground and background colors of the rowcol to make
     * all the pixmaps appear using a consistent color.
     */
    XtVaGetValues (rowcol,
        XmNforeground, &fg,
        XmNbackground, &bg,
        NULL);

    while (*++argv) {
        Pixmap pixmap = XmGetPixmap (XtScreen (rowcol), *argv, fg, bg);
        if (pixmap == XmUNSPECIFIED_PIXMAP)
            printf ("Couldn't load %s\n", *argv);
        else
            XtVaCreateManagedWidget (*argv, xmLabelGadgetClass, rowcol,
                XmNlabelType, XmPIXMAP,
                XmNlabelPixmap, pixmap,
                NULL);
    }

    XtManageChild (rowcol);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* get the integer square root of n -- used to determine the number
 * of rows and columns of pixmaps to use in the RowColumn widget.
 */
int_sqrt(n)
register int n;
{
    register int i, s = 0, t;
    for (i = 15; i >= 0; i--) {
        t = (s | (1 << i));
        if (t * t <= n)
            s = t;
    }
    return s;
}
