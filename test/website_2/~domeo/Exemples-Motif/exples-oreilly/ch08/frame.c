/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* frame.c -- demonstrate the Frame widget by creating 
 * four Labels with Frame widget parents.
 */
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/Frame.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, rowcol, frame;
    XtAppContext app;

    XtSetLanguageProc (NULL, NULL, NULL);

    /* Initialize toolkit and create TopLevel shell widget */
    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    /* Make a RowColumn to contain all the Frames */
    rowcol = XtVaCreateWidget ("rowcolumn",
        xmRowColumnWidgetClass, toplevel,
        XmNspacing, 5,
        NULL);

    /* Create different Frames each containing a unique shadow type */
    XtVaCreateManagedWidget ("Frame Types:",
        xmLabelGadgetClass, rowcol, NULL);
    frame = XtVaCreateManagedWidget ("frame1",
        xmFrameWidgetClass, rowcol,
        XmNshadowType,      XmSHADOW_IN,
        NULL);
    XtVaCreateManagedWidget ("XmSHADOW_IN", 
        xmLabelGadgetClass, frame, 
        NULL);
#ifdef MOTIF_1_2
    XtVaCreateManagedWidget ("XmALIGNMENT_CENTER", 
        xmLabelGadgetClass, frame, 
        XmNchildType, XmFRAME_TITLE_CHILD,
        XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
        NULL);
#endif

    frame = XtVaCreateManagedWidget ("frame2",
        xmFrameWidgetClass, rowcol,
        XmNshadowType,      XmSHADOW_OUT,
        NULL);
    XtVaCreateManagedWidget ("XmSHADOW_OUT", 
        xmLabelGadgetClass, frame, 
        NULL);
#ifdef MOTIF_1_2
    XtVaCreateManagedWidget ("XmALIGNMENT_BASELINE_TOP", 
        xmLabelGadgetClass, frame, 
        XmNchildType, XmFRAME_TITLE_CHILD,
        XmNchildVerticalAlignment, XmALIGNMENT_BASELINE_TOP,
        NULL);
#endif

    frame = XtVaCreateManagedWidget ("frame3",
        xmFrameWidgetClass, rowcol,
        XmNshadowType,      XmSHADOW_ETCHED_IN,
        NULL);
    XtVaCreateManagedWidget ("XmSHADOW_ETCHED_IN", 
        xmLabelGadgetClass, frame, 
	NULL);
#ifdef MOTIF_1_2
    XtVaCreateManagedWidget ("XmALIGNMENT_WIDGET_TOP", 
        xmLabelGadgetClass, frame, 
        XmNchildType, XmFRAME_TITLE_CHILD,
        XmNchildVerticalAlignment, XmALIGNMENT_WIDGET_TOP,
	NULL);
#endif

    frame = XtVaCreateManagedWidget ("frame4",
        xmFrameWidgetClass, rowcol,
        XmNshadowType,      XmSHADOW_ETCHED_OUT,
        NULL);
    XtVaCreateManagedWidget ("XmSHADOW_ETCHED_OUT", 
        xmLabelGadgetClass, frame, 
        NULL);
#ifdef MOTIF_1_2
    XtVaCreateManagedWidget ("XmALIGNMENT_WIDGET_BOTTOM", 
        xmLabelGadgetClass, frame, 
        XmNchildType, XmFRAME_TITLE_CHILD,
        XmNchildVerticalAlignment, XmALIGNMENT_WIDGET_BOTTOM,
        NULL);
#endif

    XtManageChild (rowcol);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

